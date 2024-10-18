# myshell - Basic linux shell with additional shortcuts

Compilation

Run gcc command to compile
```
gcc myshell.c -o myshell
```
Execution

Run shell
```
./myshell
```

Explanation

This shell can run normal linux commands such as ls, rm, mv, and cat. It also is capable of additional shortcuts.

Shortcuts

These are case-sensitive.

```
C file1 file2 - Copy file1 to file2. Creates or overwrites file2. file1 is untouched.
D file - Delete the file.
E comment - Echo. Display comment on screen.
H - Help. Display user manual. What you're viewing right now.
L - List. Modified version of ls. Prints working directory and lists contents in long form.
M file - Make. Open or create a text file with nano editor.
P file - Print file contents on screen with the more command.
Q - Quit and terminate shell.
W - Wipe and clear the screen.
X program - Executes the program. Return to shell after exiting program.
```

Resources Used

Linux manual pages - https://linux.die.net/man/ - getcwd() and getpwuid() syntax.
