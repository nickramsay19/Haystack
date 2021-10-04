# Haystack - Esoteric programming language
> Created by Nicholas Ramsay

## Overview
Haystack is an esoteric programming language that disallows the storage of memory outside of a stack data structure. That is, the language doesn't support variables and all data is stored on a stack. Conditional statments, loop conditions and all operations are performed on items at the top of the stack.

### Hello,  world! - In Haystack
In the following Hello world example, notice how items are pushed in reverse order. This is of course due the fact that printing prints the item on top i.e. the last item added.
```
push 50     ; "!" in ASCII.
push 100    ; "d"
push 108    ; "l"
push 114    ; "r"
push 111    ; "o"
push 119    ; "w"
push 32     ; " "
push 44     ; ","
push 111    ; "o"
push 108    ; "l"
push 108    ; "l"
push 101    ; "e"
push 72     ; "H"
print
print
print
print
print
print
print
print
print
print
print
print
print
```



## Usage & Compilation (MacOS)
```
make
cd bin
./haystack <YOUR_CODE>.hay
```

## Basic usage and commands
- Comments - Any text written after a semicolon (";") and on the same line will not be parsed by Haystack.
- **read** - Reads a single character from stdin, stores it's ASCII value as an integer to the top of the stack.
- **print** - Pops from the stack, prints the value's ASCII interpretation as a character.
- **push X** - Pushes the integer X to the top of the stack.
- **pop** - Pops and deletes the integer at the top of the stack.
- **copy** - Copies the item at the top of stack, leaving two copies.
- **add** - Pop's the top two integers on the stack, then pushes the addition of both.
- **sub** - Pop's the top two integers on the stack, say B, then A (where B was originally on top of A). Pushes A - B.
- **mult** -  Pop's the top two integers on the stack, then pushes the product of both.
- **div** - Pop's the top two integers on the stack, say B, then A (where B was originally on top of A). Pushes A / B. A division by zero error will be thrown if B is zero.
- **mod** - Pop's the top two integers on the stack, say B, then A (where B was originally on top of A). Pushes A % B. A division by zero error will be thrown if B is zero.
- **maybe** - Pops the item at the top of the stack, if it is interpreted as boolean true (i.e. it isn't equal to zero) then the command directly after it (on the same line) will be executed.
    - For example, this program will print an exclamation mark(!), since 1 will be interpreted as true.
        ```
            push 50         ; Exclamation mark(!) in ASCII.
            push 1
            maybe print     ; Equivalent to if (1) { print }
        ```
- **then** - If preceded by a **maybe** or an **or** statement, the command directly next to it (on the same line) will be executed when the preceding conditional statement triggers true.
    - For example, this program will print an exclamation mark(!) since the **maybe** statement will trigger, immediately followed by the `then print`.
        ```
            push 1
            maybe push 50   ; Exclamation mark(!) in ASCII.
            then print
        ```
    
- **or** - If preceded by a **maybe** or an **or** statement, the command directly next to it (on the same line) will be executed if and only if:
    1. None of the preceding conditionals triggered.
    2. The integer at the top of the stack is interpreted as true (i.e. it is not zero).
    - NOTE: If both 1 & 2 hold, then, like in the **maybe** command, the integer at the top of the stack is popped and subsequnetly interpreted. However, this is not true if, in particular, 1 does not hold. That is, if a conditional sequence directly above an **or** is triggered, the **or** statement will NOT pop a value from the stack, since regardless, it will NOT execute its ajoining command.
- **loop** - Marks a loop position in the code. The next **jump** command will jump to this line.
- **jump** - Moves execution to the previous loop header.
    - NOTE: If a **jump** (see the **jump** command below) command is to be executed conditionally, that is, it is preceded by a **maybe** or an **or** command (e.g `maybe jump`) AND it fails to execute (i.e. the condition does not pass), then Haystack will recognise this as a failed jump. Any further jumps will move the program to a prior loop.
        - For example, this program will infinitely print exclamation marks and dollar signs since the second **jump** statement will jump to the FIRST loop, since a **jump** command precedes it. This is true regardless of whether or not a preceding **jump** triggered.
            ```
                loop
                print 50    ; Exclamation mark(!) in ASCII.
                loop
                print 36    ; Dollar sign($) in ASCII.
                push 0
                maybe jump  ; This jump will NEVER trigger, since 0 precedes it.
                jump        ; This jump will jump to the FIRST loop, despite being the first jump triggered after the beginning of the SECOND loop.
            ```

## Backlog
- Char support.
    - Store and print ASCII values to/from the stack.
- Improve comment parsing.
- Division by zero error.