#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QtEndian>

struct WavHeader {
    char chunkId[4];      // "RIFF"
    quint32 chunkSize;    // Size of whole file - 8
    char format[4];       // "WAVE"
    char subchunk1Id[4];  // "fmt "
    quint32 subchunk1Size;// 16 for PCM
    quint16 audioFormat;  // 1 = PCM
    quint16 numChannels;  // 1 = Mono, 2 = Stereo
    quint32 sampleRate;   // e.g., 44100
    quint32 byteRate;     // sampleRate * numChannels * bitsPerSample/8
    quint16 blockAlign;   // numChannels * bitsPerSample/8
    quint16 bitsPerSample;// 8 or 16
};

bool convertWavToGusPat(const QString &wavFilePath, const QString &patFilePath, quint8 midiNote) {
    QFile wavFile(wavFilePath);
    if (!wavFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open input WAV file:" << wavFilePath;
        return false;
    }

    WavHeader header;
    if (wavFile.read(reinterpret_cast<char*>(&header), sizeof(WavHeader)) != sizeof(WavHeader)) {
        qWarning() << "Failed to read standard WAV header.";
        return false;
    }

    if (QByteArray(header.chunkId, 4) != "RIFF" || QByteArray(header.format, 4) != "WAVE") {
        qWarning() << "File is not a valid RIFF/WAVE file.";
        return false;
    }

    if (header.audioFormat != 1) {
        qWarning() << "Only uncompressed PCM WAV files are supported.";
        return false;
    }

    QByteArray rawAudioData;
    while (!wavFile.atEnd()) {
        char subchunkId[4];
        quint32 subchunkSize;
        if (wavFile.read(subchunkId, 4) != 4) break;
        if (wavFile.read(reinterpret_cast<char*>(&subchunkSize), 4) != 4) break;

        if (QByteArray(subchunkId, 4) == "data") {
            rawAudioData = wavFile.read(subchunkSize);
            break;
        } else {
            wavFile.seek(wavFile.pos() + subchunkSize);
        }
    }

    if (rawAudioData.isEmpty()) {
        qWarning() << "No 'data' chunk found in WAV file.";
        return false;
    }

    // Downmix stereo to mono if needed
    if (header.numChannels == 2) {
        QByteArray monoData;
        monoData.resize(rawAudioData.size() / 2);
        if (header.bitsPerSample == 16) {
            const qint16 *in = reinterpret_cast<const qint16*>(rawAudioData.constData());
            qint16 *out = reinterpret_cast<qint16*>(monoData.data());
            int samples = rawAudioData.size() / 4;
            for (int i = 0; i < samples; ++i) {
                out[i] = (in[i * 2] + in[i * 2 + 1]) / 2;
            }
        } else {
            const quint8 *in = reinterpret_cast<const quint8*>(rawAudioData.constData());
            quint8 *out = reinterpret_cast<quint8*>(monoData.data());
            int samples = rawAudioData.size() / 2;
            for (int i = 0; i < samples; ++i) {
                out[i] = (static_cast<int>(in[i * 2]) + in[i * 2 + 1]) / 2;
            }
        }
        rawAudioData = monoData;
    }

    QByteArray processedSamples = rawAudioData;
    quint32 sampleCount = processedSamples.size() / (header.bitsPerSample / 8);

    if (header.bitsPerSample == 16) {
        qint16 *samples = reinterpret_cast<qint16*>(processedSamples.data());
        for (quint32 i = 0; i < sampleCount; ++i) {
            samples[i] = qFromLittleEndian<qint16>(samples[i]);
        }
    } else {
        quint8 *samples = reinterpret_cast<quint8*>(processedSamples.data());
        for (quint32 i = 0; i < sampleCount; ++i) {
            samples[i] = static_cast<quint8>(samples[i] ^ 0x80); 
        }
    }

    QFile patFile(patFilePath);
    if (!patFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to create output .PAT file:" << patFilePath;
        return false;
    }

    QDataStream out(&patFile);
    out.setByteOrder(QDataStream::LittleEndian);

    // --- 1. Global Patch Header (239 bytes structure alignment matching loader checks) ---
    out.writeRawData("GF1PATCH110", 11);
    out << static_cast<quint8>(0);           // Null terminator at byte 11
    out.writeRawData("ID#000002\0", 10);     // Offset 12-21 matches loader check memcmp
    
    char description[60];
    memset(description, 0, sizeof(description));
    qstrncpy(description, "Converted via @anytizer's wav2pat (Qt based)", sizeof(description));
    out.writeRawData(description, 60);

    // Byte offsets matching header verification indexes in MadEngineInstrument::loadPatch:
    // header[82] -> Number of instruments (must be 1)
    // padding up to index 82: 12 + 10 + 60 = 82 bytes.
    out << static_cast<quint8>(1);           // Offset 82: Instruments = 1
    
    // Fill up to layer check index header[151]
    QByteArray pad1(151 - 83, 0);
    out.writeRawData(pad1.constData(), pad1.size());

    out << static_cast<quint8>(1);           // Offset 151: Layers = 1

    // Fill up to sample count index header[198]
    QByteArray pad2(198 - 152, 0);
    out.writeRawData(pad2.constData(), pad2.size());

    out << static_cast<quint8>(1);           // Offset 198: Sample count = 1

    // Fill remainder of the 239-byte header block
    QByteArray pad3(239 - 199, 0);
    out.writeRawData(pad3.constData(), pad3.size());

    // --- 2. Waveform Payload Block (Matching exact parser skips) ---
    // skip wave name (7) + fractions (1)
    out.writeRawData("sample", 6);
    out << static_cast<quint8>(0) << static_cast<quint8>(0);

    // data_length (4 bytes)
    out << static_cast<quint32>(processedSamples.size());

    // loop_start & loop_end (4 bytes each)
    quint32 loopStart = 0;
    quint32 loopEnd = sampleCount - 1;
    out << loopStart;
    out << loopEnd;

    // sample_rate (2 bytes short)
    out << static_cast<quint16>(header.sampleRate);

    // skip low_freq (4) + high_freq (4)
    out << static_cast<quint32>(100) << static_cast<quint32>(20000);

    // root_freq (4 bytes long) -> Calculated in Hz * 1000 to line up with loader division (root_freq / 1000.0f)
    // Standard MIDI note to frequency formula approximation or default middle C (261.63 Hz -> 261630)
    quint32 calculatedRootFreqHz = 261630; 
    out << calculatedRootFreqHz;

    // skip tuning (2) + panning (1) + envelope (12) + tremolo (3) + vibrato (3) = 21 bytes
    QByteArray skipBlock(21, 0);
    // Set envelope bytes explicitly to active sustain values instead of 0xFF to prevent activation crashes
    skipBlock[3] = 0x7F; skipBlock[4] = 0x7F; skipBlock[5] = 0x7F;
    out.writeRawData(skipBlock.constData(), skipBlock.size());

    // modes (1 byte) -> MADENGINE_MODES_16BIT (0x01) | MADENGINE_MODES_LOOPING (0x04)
    quint8 modes = 0x04; // Looping on
    if (header.bitsPerSample == 16) {
        modes |= 0x01;   // 16-bit flag
        modes |= 0x08;   // MADENGINE_MODES_UNSIGNED flag if matching little-endian signed adjustments
    }
    out << modes;

    // skip scale frequency (2) + scale factor (2) + reserved space (36) = 40 bytes
    QByteArray tailPadding(40, 0);
    out.writeRawData(tailPadding.constData(), tailPadding.size());

    // --- 3. Raw PCM Sample Payload Data ---
    out.writeRawData(processedSamples.constData(), processedSamples.size());

    patFile.close();
    qDebug() << "Successfully generated .pat file."; // matching LMMS parser layout
    return true;
}