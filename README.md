# C file bundle
This is a bundle of minimalistic file programs written in C.
Each programs purpose is to do one task with files. For example bring only certain amount of characters from head / tail and etc.
Everything else about compilation is written in Makefile, so just run
```
make
```
and read the output.
Beaware that Makefile will try to sudo gcc every source file, so either call 'compile-all' option
```
make compile-all
```
and then sudo cp it by yourself whenever you want, or call 'compile-and-send' options and it will move everything in one go to /usr/local/bin foldef.
```
make compile-and-send
```

## Sinopsis for each program
### getchars
prints first bytes from file.
If file is smaller than require number of bytes, program will print to stdout until the end of file.
### charset
print set of bytes that is used in file.
bytes are separated by ' ' char.
**Options**
 - -h : Hex.  Instead of unsigned value (0..255) hex variant will be printed (00..FF).
