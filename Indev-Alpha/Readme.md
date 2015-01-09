== BUILDING ==
*Windows*

Prerequisities:

* CMake >= 2.8.10
* Visual Studio >=2012 or QtCreator + Mingw >= 4.8.0

Linux:

    -Eigen3 (apt-get install libeigen3-dev)
    -FLANN (apt-get install libflann-dev)
    -OpenCL Drivers MUST be installed
Build process:

```
    run cmake-gui
    in "Where is the source code" enter/select the directory where the project has been cloned to.
    in "Where to build the binaries" create/select some directory like DOT_build
    press "Configure", "Generate"
    In Visual Studio - File/Open Solution -> navigate to DOT_build directory, and open the sln file
    In Qt Creator - File/Open File or Project -> navigate to sourc directory, select CMakeLists.txt, then in the CMake window select the DOT_build directory.

    Build
```

