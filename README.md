# stdbtool
A tool that generates ST.DB files and their corresponding file structures for the Xbox dashboard's music menu

This project is at an extremely early development stage, it is barely functional.

The issues tab is open for you to slather me with things to fix or add!

# Usage
The tool needs a name for your soundtrack, and a folder containing the songs (**in WMA format**) you would like to have in it.

As of now, there isn't a CLI, let alone a GUI, so, changing input directories and soundtrack names must be done in source.

Otherwise, just put your WMA music files in a folder named `test-soundtrack`. The name of your soundtrack will be `test`.

## Output
The tool generates a filesystem in the following structure:
```
fffe0000/
└── music
    ├── 0000
    │   ├── 00000000.wma
    │   ├── 00000001.wma
        . . .
    ├── 0001
    │   ├── 00000000.wma
    │   ├── 00000001.wma
    |   . . .
    . . .
    └── ST.DB
```

`fffe0000` is the root directory, followed by `music`. This folder contains the `ST.DB` file, and folders with 4-digit zero padded names. Each one of these folders is a soundtrack.

Inside the soundtrack folders, you will find your music files. Please do not rename any of these files/folders as their names are deliberate, and needed for the Xbox to find them.

## Transferring to a real Xbox or emulator
Now that you have your file structure ready, you will need to transfer it to an Xbox.

I've only tested this tool in an emulator, but the process should be the same in a real Xbox.

### xemu
Follow [this guide](https://xemu.app/docs/ftp/) to setup an FTP server on the emulated Xbox, then, copy the generated `fffe0000` folder to `/E/TDATA/`.

### Cxbx-R
Cxbx-R has its filesystem exposed to the OS, no need to setup an FTP server, just copy the generated `fffe0000` folder to `/EmuDisk/Partition1`

### Real Xbox
The process should be the same as in xemu, setup an FTP server on your Xbox, connect to it, and copy the generated `fffe0000` folder to `/E/TDATA/`.

# Known issues
This is the first iteration of this tool, expect things to break. However, issues are actively being investigated, here is a list of known issues:
- The tool will not generate more than one soundtrack
- A lot of songs don't get recognized by the Xbox

# Building
Thankfully, this is simple enough using a C++17-compatible compiler.
```
git clone https://github.com/Lycoder/stdbtool
cd stdbtool
c++ main.cpp -o main -std=c++17
```
