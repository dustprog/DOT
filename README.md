# DOT
## The open source version of the DOT AI Engine.

If you have any questions on how to use DOT, or its editor, feel free to contact the studio at projectferomine@gmail.com

The Windows EXE is located under Editor+Engine/Debug/DOT_Editor

## Building the project

### Windows
In order to compile DOT, you need the newest version of Qt (As of November 2014) [5.3.2] for Windows.
Both 32 and 64 bit versions are required. A minimum of Visual Studio 2013 is also required for compilation.

Also required:
* OpenGL
* JRE (Oddly... Never could figure out why)

Later versions of DOT will require Visual Studio 2013 Profesional or better, as well as a full
directX 11 installation.

### Linux
In order to compile DOT, you will need a Qt version >= 5.3.1 ( earlier 5.x releases might work too ) and cmake, plus either relatively recent gcc/clang compiler.
The build process after acquiring/installing those dependencies should be straightforward:

```bash
    cd <cloned_repository_dir>
    mkdir build_dir
    cd build_dir
    cmake ..
    make
```

### Notes
* If you wish to use DOT in a commercial (INDIE!) product, please contact me before integrating DOT into your product (We want to make sure that its integrated well into most if not all solutions).

* If you wish to use DOT in a commercial non-indie product, please contact me and we will schedule a meeting.
