# Stack only esoteric programming language

## Usage (MacOS)
```
make
cd bin
./esonick code.en
```

## Backlog
- replace printing with print char value
- change read to read single char value
- loops
    - loop
    - maybe break
    - maybe jump
        - jumps to last loop header "loop"
- conditionals
    - maybe print
    - or pop
    - done
- comments
- undo conditional pop regardless in cond_print
- done - end conditional
- end - end the program


# Simple Project Structure & Template for C/C++/ASM
> Created by Nicholas Ramsay

Fork the repo to start your own project.
Add your C files to `src/` and header files to `src/include` then run `make` to get started.

## Features
- A best practices folder structure.
- A configurable Makefile that automatically builds object files and the executable.
- A gitignore file encompassing common innessential files. This includes common files on Windows and MacOS.

