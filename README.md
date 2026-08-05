# wav2pat.lmms

## Current Status: Proposal Only

```
failing to harvest exact samples so far.
May be due to root keys unaffected.
Sample sound feels too low, and do not match with .wav output.
```

Creates *wav2pat* alternative binary in Windows and Linux.
It can convert 16 bits signed, uncompressed PCM [.wav](https://en.wikipedia.org/wiki/WAV) file to [.pat](https://en.wikipedia.org/wiki/Gravis_UltraSound) file. They are meant to be used with [MadEngine](https://github.com/anytizer/MadEngine.lmms/) or [Patman](https://github.com/LMMS/lmms/tree/master/plugins/Patman/) under [LMMS](https://github.com/LMMS/lmms/).

MadEngine is a minified version from Patman, that:

- does not loop over samples
- has auto tune pre-enabled
- Accepts patch file drop-ins
- hence, configurations options are omitted
- engine renamed to `<madengine/>` xml node

This file format conversion utility is useful to both plugins.
You can now create your own .pat file and protect the original .wav sample.
There are some samples from [Madal Instrument](samples/pat/) for revision.

See usage instruction below for details.

## Method 1: Using CMake

### Under Linux

```
cd build
cmake -S .. -B . -DCMAKE_PREFIX_PATH=/home/@USERNAME/Qt/6.11.1/gcc_64/
cmake --build .
```

### Under Windows

```
cd build
cmake -S .. -B . -DCMAKE_PREFIX_PATH=C:\Qt\6.11.1\msvc2022_64
cmake --build .
```

## Method 2: Using qmake

### Under Linux

```
cd build
qmake ../wav2pat.pro
make
```

### Under Windows

Start a Windows terminal using:

```
REM
REM "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
REM "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\cl.exe"
REM
REM Windows > Start Menu > "x64 Native Tools Command Prompt for VS 2022"
REM
```

Then,

```
cd build
qmake ..\wav2pat.pro
nmake
```

## Example

```
./wav2pat -i 01.wav -o 01.pat -m 69
./wav2pat -i 02.wav -o 02.pat -m 69
./wav2pat -i 03.wav -o 03.pat -m 69
./wav2pat -i 04.wav -o 04.pat -m 69
./wav2pat -i 05.wav -o 05.pat -m 69
./wav2pat -i 06.wav -o 06.pat -m 69
./wav2pat -i 07.wav -o 07.pat -m 69
./wav2pat -i 08.wav -o 08.pat -m 69
./wav2pat -i 09.wav -o 09.pat -m 69
./wav2pat -i 10.wav -o 10.pat -m 69
./wav2pat -i 11.wav -o 11.pat -m 69
./wav2pat -i 12.wav -o 12.pat -m 69
./wav2pat -i 13.wav -o 13.pat -m 69
./wav2pat -i 14.wav -o 14.pat -m 69
./wav2pat -i 15.wav -o 15.pat -m 69
./wav2pat -i 16.wav -o 16.pat -m 69
```

## Usage

```
./wav2pat -i instrument.wav -o instrument.pat
./wav2pat -i instrument.wav -o instrument.pat -m 48
./wav2pat --help
./wav2pat
```

## Help

```
Usage: ./wav2pat [options]
Converts uncompressed PCM WAV audio files into .pat patch files.

Options:
  -h, --help              Displays help on commandline options.
  --help-all              Displays help, including generic Qt options.
  -v, --version           Displays version information.
  -i, --input <file>      Input WAV file path [required].
  -o, --output <file>     Output PAT file path [required].
  -m, --midi-note <note>  Root MIDI note (0-127, default: 60 for Middle C).
```

## Credits / AI Usage

Made with the help of free version of Gemini.
