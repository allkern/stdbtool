# stdbtool
A tool that generates ST.DB files and their corresponding file structures for the Xbox dashboard's music menu

# Usage
`stdbtool <options> <input>`

| Setting | Effect |
| ------------- | ------------- |
| `-a` | Use all folders in the current directory as input (ignoring `fffe0000`) |
| `-c` | Cleanup directories (delete `fffe0000`) |
| `-s` | Output `ST.DB` only, don't generate a filesystem |
| `-o <names>` | Specify soundtrack names for every input folder |


## Details
 - Using `-a` will ignore folders specified as input
 - Using `-c` will only cleanup directories, nothing else
 - Output names specified with `-o` are mapped one-to-one with input folders, for example `stdbtool A B C -o X Y`; In this case, the soundtrack generated from folder `A` will be named `X`, `B` will be named `Y`. And because only two output names were specified, the soundtrack generated from `C` will be named `C`.

# Output
The tool generates a filesystem in the following structure:
```
fffe0000/
└── music/
    ├── 0000/
    │   ├── 00000000.wma
    │   ├── 00000001.wma
    |        . . .
    |   └── nnnnnnnn.wma
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

# Building
Thankfully, this is simple enough using a C++17-compatible compiler.
```
git clone https://github.com/Lycoder/stdbtool
cd stdbtool
c++ main.cpp -o main -std=c++17 -Istdb
```

I'm trying to comment the code as much as possible because I haven't been able to find a lot of documentation on this format, I'm aiming to make this code good enough to serve as a reference or documentation.
