#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

#include "wav2pat.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("wav2pat");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Converts uncompressed PCM WAV audio files into Gravis UltraSound (.PAT) patch files.");
    parser.addHelpOption();
    parser.addVersionOption();

    // Define command line options
    QCommandLineOption inputOption(
        QStringList() << "i" << "input",
        QCoreApplication::translate("main", "Input WAV file path [required]."),
        QCoreApplication::translate("main", "file")
    );
    parser.addOption(inputOption);

    QCommandLineOption outputOption(
        QStringList() << "o" << "output",
        QCoreApplication::translate("main", "Output PAT file path [required]."),
        QCoreApplication::translate("main", "file")
    );
    parser.addOption(outputOption);

    QCommandLineOption midiNoteOption(
        QStringList() << "m" << "midi-note",
        QCoreApplication::translate("main", "Root MIDI note (0-127, default: 60 for Middle C)."),
        QCoreApplication::translate("main", "note"),
        "60"
    );
    parser.addOption(midiNoteOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    QString wavPath = parser.value(inputOption);
    QString patPath = parser.value(outputOption);
    
    // Validate required arguments
    if (wavPath.isEmpty() || patPath.isEmpty()) {
        qWarning() << "Error: Both input (-i) and output (-o) paths must be specified.";
        parser.showHelp(1);
    }

    // Parse and validate the MIDI note parameter
    bool ok = false;
    int midiNoteInt = parser.value(midiNoteOption).toInt(&ok);
    if (!ok || midiNoteInt < 0 || midiNoteInt > 127) {
        qWarning() << "Error: Invalid MIDI note specified. Must be an integer between 0 and 127.";
        return 1;
    }
    quint8 midiNote = static_cast<quint8>(midiNoteInt);

    qDebug() << "Converting:" << wavPath;
    qDebug() << "Target PAT:" << patPath;
    qDebug() << "Root MIDI Note:" << midiNote;

    // Execute conversion
    bool success = convertWavToGusPat(wavPath, patPath, midiNote);

    if (success) {
        qDebug() << "Conversion completed successfully.";
        return 0;
    } else {
        qWarning() << "Conversion failed.";
        return 1;
    }
}