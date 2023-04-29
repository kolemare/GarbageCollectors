## Mark and Sweep Algorithm

The Mark and Sweep algorithm is a garbage collection method used to manage memory in programming languages. It operates in two phases: the mark phase and the sweep phase.

## Mark Phase

In the mark phase, the algorithm traverses the object graph, starting from the root objects (objects directly accessible by the application). The algorithm marks each object it encounters as "reachable" or "live." Reachable objects are those that can be accessed through some chain of references from the root objects.

The mark phase is usually implemented using a depth-first or breadth-first search traversal. The search starts from the root objects and follows object references, marking each object it encounters as live. This process continues until all reachable objects have been marked.

## Sweep Phase

Once the mark phase is complete, the algorithm proceeds to the sweep phase. In this phase, the garbage collector scans through the entire memory heap, looking for objects that have not been marked as live. These unmarked objects are considered "unreachable" or "garbage," as they can no longer be accessed by the application.

The garbage collector then frees the memory occupied by these unreachable objects, returning it to the system's memory pool for future allocation. After the sweep phase is complete, the garbage collector unmarks all live objects, resetting their status for the next garbage collection cycle.

The Mark and Sweep algorithm ensures that only memory occupied by unreachable objects is freed, preventing memory leaks and enabling more efficient memory management in the application.

## About project itself
This is a simple implementation representing how Mark & Sweep algorithm works.


## Table of Contents

1. [Getting Started](#introduction)
    1. [Prerequisites](#prerequisites)
        1. [Windows prereqs](#windows-prereqs)
        2. [Linux prereqs](#linux-prereqs)
    2. [Repository structure](#repository-structure)
    3. [Cloning](#cloning)
2. [Project Files Overview](#project-files-overview)
3. [Scripts Explained](#scripts-explained)
4. [Building](#building)
5. [Running](#running)
6. [Pictures](#pictures)


## Getting Started

These instructions will help you set up and run the project on your local machine for development and testing purposes.
This repository contains build scripts for both linux and windows, which will be explained further below.



### Prerequisites
IDE is irrelevant. Recommendation is VSCode.   
Windows: Download Visual Studio Code from [here](https://code.visualstudio.com/download).  
Linux: `$ sudo apt-get install code`
Recommendation for plugins:  
**C/C++ IntelliSense**  
**C/C++ Extensions for C++**    
**CMake Tools**    

#### Windows prereqs
> **Note**: Windows build is quite ungrateful. Linux build is much more intuitive and easier.  
Its not advised to use **nmake**.

Although this project can be built by using MinGW, Clang, MSYS2, or other environments it is not recommended to approach in that way.
This application was only tested by using Cygwin environment. Why?
Cygwin is a Unix-like environment and command-line interface for Windows. It includes a wide range of Unix tools, including GCC, which can be used to compile C and C++ programs.
It was necessary for building graphviz submodule and linking it to one exe file alongside M&S algorithm itself and googletest library for writing unit tests for C and C++ language. Cygwin's make is designed to work on Windows using Cygwin's emulation layer, which provides a Unix-like environment.

Here are necessary libraries that you shall install with Cygwin:

| Tool       | Description                                                             |
|------------|-------------------------------------------------------------------------|
| gcc        | The GNU Compiler Collection, which includes C and C++ compilers.        |
| make       | The GNU make utility for building programs.                             |
| cmake      | The cross-platform build system generator.                              |
| libtool    | A generic library support script for building shared libraries.         |
| pkg-config | A helper tool for managing library compile and link flags.              |
| gettext    | Internationalization (i18n) and localization (l10n) support for messages.|
| libiconv   | A character set conversion library.                                     |
| pango      | A library for layout and rendering of text, with an emphasis on i18n.   |
| cairo      | A 2D graphics library with support for multiple output devices.          |
| libpng     | A library for reading and writing PNG images.                            |
| libjpeg    | A library for reading and writing JPEG images.                           |
| libfreetype| A library for rendering text with TrueType, OpenType, and other font formats. |
| fontconfig | A library for configuring and customizing font access.                   |
| expat      | A stream-oriented XML parser library.                                   |
| zlib       | A general-purpose lossless data-compression library.                     |
| swig       | A tool for connecting C/C++ programs with scripting languages.           |
| flex       | A fast lexical analyzer generator.                                      |
| bison      | A parser generator that's compatible with Yacc. 

Python shall be installed on your system, download Python from [here](https://www.python.org/downloads/windows/).  
Install OpenCV with command: `pip install opencv-python`

#### Linux prereqs

| Command                      | Description                                                             |
|------------------------------|-------------------------------------------------------------------------|
| `sudo apt-get install cmake make` | Installs cmake and make, used for build system generation and program building. |
| `sudo apt-get install build-essential` | Installs essential build tools, including gcc, g++, and others. |
| `sudo apt-get install bison` | A parser generator that's compatible with Yacc.                         |
| `sudo apt-get install flex` | A fast lexical analyzer generator.                                      |
| `sudo apt-get install qtbase5-dev` | Qt 5 base development files.                                           |
| `sudo apt-get install libann-dev` | Approximate Nearest Neighbor (ANN) library development files.          |
| `sudo apt-get install libcairo2-dev` | A 2D graphics library with support for multiple output devices (Cairo). |
| `sudo apt-get install libexpat1-dev` | A stream-oriented XML parser library (Expat).                          |
| `sudo apt-get install libgts-dev` | GNU Triangulated Surface library development files (GTS).              |
| `sudo apt-get install libltdl-dev` | A system independent dlopen wrapper for GNU libtool (LTDL).           |
| `sudo apt-get install zlib1g-dev` | A general-purpose lossless data-compression library (Zlib).            |
| `sudo apt-get install libgd-dev` | A graphics drawing library for creating PNG, JPEG, and other images (GD). |
| `sudo apt-get install libdevil-dev` | A full-featured cross-platform image loading and manipulation library (DevIL). |
| `sudo apt-get install ghostscript` | An interpreter for the PostScript language and for PDF (GS).          |
| `sudo apt-get install libgtk2.0-dev` | Development files for the GTK+2 graphical user interface library.     |
| `sudo apt-get install libfreetype6-dev` | A library for rendering text with TrueType, OpenType, and other font formats (Freetype). |
| `sudo apt-get install libpango1.0-dev` | A library for layout and rendering of text, with an emphasis on i18n (PANGOCAIRO). |
| `sudo apt-get install pkg-config` | A helper tool for managing library compile and link flags (PkgConfig). |
| `sudo apt-get install libxkbcommon-dev` | XKB API common to servers and clients (required version >= 0.5.0).   |


### Repository structure

```
.
├── application
│   ├── include
│   │   ├── googletest (submodule)
│   │   ├── graphviz (submodule)
│   │   └── models.hpp
│   ├── src
│   │   ├── GarbageCollector.hpp
│   │   ├── GCDisplay.py
│   │   ├── GCFactory.hpp
│   │   ├── GCInvokeDisplaySingleton.hpp
│   │   ├── GCObserver.hpp
│   │   ├── GCVisualizer.hpp
│   │   └── GCWrapObject.hpp
│   └── main.cpp
├── tests
│   ├── testGarbageCollector.hpp
│   └── testGarbageCollector.cpp
├── .gitignore
├── .gitmodules
├── build.bat
├── build.sh
├── clean.bat
├── clean.sh
├── CMakeLists.txt
├── LICENCE
├── linux_patch.sh
├── README.md
├── run_app.bat
├── run_app.sh
└── windows_patch.bat
```


### Cloning
Makse sure you have git installed.  
Windows: Download Git from [here](https://git-scm.com/download/win), verify installation with `git --version`  
Linux: `$ sudo apt-get install git`, verify installation with `$ git --version`  
Position terminal in desired directory and use command: `git clone git@github.com:kolemare/GarbageCollectors.git`
This project uses 2 submodules graphviz and googletest so they need to be cloned as well, use command below:  
`git submodule update --init --recursive` after terminal's position is inside root directory of the project.



## Project Files Overview
| Name of File                    | Explanation                                                                                           |
|---------------------------------|-------------------------------------------------------------------------------------------------------|
| GarbageCollector.hpp            | This header file contains general implementation logic of ***Mark & Sweep*** garbage collecting algorithm.                                       |
| GCDisplay.py                    | This Python script is responsible for real time display garbage collector's visualizations. Every 50ms it is trying to load image `gc_visualizer.png` which is constantly updated by `GCVisualizer`              |
| GCFactory.hpp                   | This header file contains the definition for dynamic allocation of various types, classes and fundamental types.                            |
| GCInvokeDisplaySingleton.hpp    | This header file contains the definition of the `GCInvokeDisplaySingleton` class, which responsibility is when called run the python script. Singleton design pattern is used here to make sure python script won't be run multiple times.  |
| GCObserver.hpp                  | This header file contains the definition of the `GCObserver` class, which is a base class for `GCVisualizer`                         |
| GCVisualizer.hpp                | This header file contains the definition of the `GCVisualizer` class, which is responsible for visualizing the garbage collector's current state. Generating the `gc_visualizer.png` based on GarbageCollector's current root set. Observer design patter was used here.                                         |
| GCWrapObject.hpp                | This header file contains the definition of the `GCWrapObject` class, which is responsible for wrapping objects to be managed by the garbage collector. It also makes use of managing references by implementing add_child and remove_child methods.                              |
| main.cpp                        | This file contains the main entry point of the application. Here setup can be created for Garbage Collector to work with.                                            |
| models.hpp                      | This header file contains the definitions of user defined classes.     |
| gc_visualizer.png               | This image represents the current state (realtime state) of Garbage Collector.            |
| testGarbageCollector.hpp        | This header file contains the test suite for the `GarbageCollector` class.                            |
| testGarbageCollector.cpp        | This file contains the implementation of the test suite for the `GarbageCollector` class.             |


## Scripts Explained
| Name of Script    | Examples        | Explanation                                                                                   |
|-------------------|-------------------------------------|--------------------------------------------------------------------------------------------------|
| build.bat         | `./build.bat` `./build.bat --clean`    | This script is used to build the application on Windows machines. It creates bin and build directory. `--clean flag` does the clean build (deletes bin and build directory and all of its contents). This script creates copys (backups) of ***GCInvokeDisplaySingleton.hpp*** and ***GCVisualizer.hpp.*** Then makes changes to ***GCInvokeDisplaySingleton.hpp*** (replacing `python3 GCDisplay.py` with `py GCDisplay.py`) if `python3 GCDisplay.py` was found. Changes ***GCVisualizer.hpp*** by adding `#define WINDOWS` if its not found. ***GCDisplay.py*** is copied inside bin directory, so it can be started from C++ without worrying about paths. After build procedure finishes changes are reverted to original state.                             |
| build.sh          | `./build.sh`   `./build.sh --clean`    | This script is used to build the application on Linux machines. It creates bin and build directory. `--clean flag` does the clean build (deletes bin and build directory and all of its contents). This script creates copys (backups) of ***GCInvokeDisplaySingleton.hpp*** and ***GCVisualizer.hpp.*** Then makes changes to ***GCInvokeDisplaySingleton.hpp*** (replacing `py GCDisplay.py` with `python3 GCDisplay.py`) if `py GCDisplay.py` was found. Changes ***GCVisualizer.hpp*** by removing `#define WINDOWS` if its found. ***GCDisplay.py*** is copied inside bin directory, so it can be started from C++ without worrying about paths. After build procedure finishes changes are reverted to original state.                       |
| clean.bat         | `clean.bat`     | This script is used to clean the build directory on Windows machines. Deletes bin and build directory and all of its contents                          |
| clean.sh          | `./clean.sh`    | This script is used to clean the build directory on Linux machines. Deletes bin and build directory and all of its contents                  |
| windows_patch.sh  | `windows_patch.sh -lasi` `windows_patch.sh -webp` `windows_patch.sh -revert`    | This script is used to apply changes to Graphviz build system (CMakeLists.txt)  Reason: Plugins such lasi and webp are not buildable even with full Cygwin installation on Windows(~100GB). So this script will disable build for them in  graphviz's CMakeLists.txt and `-revert` flag will return graphviz's integrity to original state.                  |
| linux_patch.sh    | `./linux_patch.sh` | Same as for windows, but its not necessary to apply this patch. It is buildable without it.                       |
| run_app.bat       | `run_app.bat`   | This script is used to run the application on Windows machines after building it.              |
| run_app.sh        | `./run_app.sh`  | This script is used to run the application on Linux machines after building it.       |

## Building
Windows: command `./build.bat`  
Linux: command `$ ./build.sh`  

## Running
Windows: command `./run_app.bat`  
Linux: command `$ ./run_app.sh`
> **Important**: Make sure to type `q` to python window when app finishes.  
if you try to close it it will re-open itself after 50ms.

## Future plans:

1. Write unit tests.
2. Detect circular references in Mark & Sweep.
3. Enable move semantics for user defined classes so M&S works.

## Pictures:
> **Note**: These pictures are provided based on current implementation of main.cpp.  
Future changes to main.cpp can produce more n-ary trees with possibility of them becoming one. Everything depends on references. Number of elements in root set represents the number of n-ary trees.

### Root set is created out of all objects: 
![Sequence1](https://i.imgur.com/GgLuoJC.png)

### N-ary tree is formed(only 1 in this example, there can be multiple based on the root set): 
![Sequence2](https://i.imgur.com/RO7yFNL.png)

### N-ary tree after dark blue Node is removed:
![Sequence3](https://i.imgur.com/cavKoxV.png)

### N-ary tree after pink node is removed:
![Sequence4](https://i.imgur.com/O2qHFLi.png)

### Root removed from root set:
![Sequence5](https://i.imgur.com/SBvIX0L.png)


