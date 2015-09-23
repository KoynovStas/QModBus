## This version of the library has been compiled in the system:
- Microsoft Windows Seven x64 Professional Edition Version 6.1 (build 7601) Service pack 1 
- Thread model: win32
- gcc version 4.9.2 (GCC)

**Command for configure and make library in cygwin64:**

```console
$ ./configure --host=x86_64-w64-mingw32 --prefix=/libmodbus-3.1.2/lib CFLAGS=-m64 CPPFLAGS=-m64
$ make
$ make install
```

# If you use another system or another compiler, for greater compatibility rebuild the library in your system
