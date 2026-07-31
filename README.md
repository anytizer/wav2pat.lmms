# wav2pat.lmms

16 bits signed, uncompressed PCM .wav to .pat converter for MadEngine or Patman for LMMS.

## Method 1: Using CMake

### Under Linux

```
cmake -S . -B build -DCMAKE_PREFIX_PATH=/home/@USERNAME/Qt/6.11.1/gcc_64/
cmake --build .
```

### Under Windows

```
cmake -S . -B build -DCMAKE_PREFIX_PATH=C:\Qt\6.11.1\msvc2022_64
cmake --build build
```

## Method 2: Using qmake

### Under Windows

```
REM "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
REM "D:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC\14.51.36231\bin\Hostx64\x64\cl.exe"

Windows > Start Menu > "x64 Native Tools Command Prompt for VS 2022"
```

```
cd build
qmake ..\wav2pat.pro
nmake
```

### Under Linux
```
cd build
qmake ../wav2pat.pro
make
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
Converts uncompressed PCM WAV audio files into Gravis UltraSound (.PAT) patch files.

Options:
  -h, --help              Displays help on commandline options.
  --help-all              Displays help, including generic Qt options.
  -v, --version           Displays version information.
  -i, --input <file>      Input WAV file path [required].
  -o, --output <file>     Output PAT file path [required].
  -m, --midi-note <note>  Root MIDI note (0-127, default: 60 for Middle C).
```

## Credits / AI Usage

Made with the help of Gemini.
