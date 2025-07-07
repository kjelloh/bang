# bang

A command-line tool that **"bangs" system commands** - automatically searching for the right tweaks to make any command succeed.

*NOTE: Currently experimental for 'banging' C++ compilation regarding include search paths*

## What is "Banging" a Command?

`bang` implements the concept: `f(in, tweaks) → out` where:
- `f` = any operating system command/function
- `in` = input file(s)  
- `tweaks` = (arguments, options) that modify how `f` operates
- `out` = output file(s)

**The "Banging" Process:**
1. `bang` tries `f(in, tweaks)` with initial tweaks
2. If it fails, `bang` analyzes the error and intelligently updates tweaks using: `t(error, tweaks) → tweaks`
3. `bang` keeps "banging" (trying different tweak combinations) until the command succeeds

**Current Implementation:**
- `bang` can "bang" C++ compilation - it keeps trying different compiler tweaks until your code compiles successfully

**Future Vision:**
- `bang` could "bang" any system command (ffmpeg, pandoc, etc.) until it produces the desired output

Instead of manually struggling with complex command-line tools, you just tell `bang` what you want to achieve, and it "bangs" the command until it figures out how to make it work.

bang © 2025 by Kjell-Olov Högdal is licensed under Creative Commons Attribution 4.0 International. To view a copy of this license, visit https://creativecommons.org/licenses/by/4.0/

# banging the C++ compiler

I would like 'bang' or additional tooling for C++ development to model and understand the C++ executable binary build and run in tiers of 'transformation'?

* C++ preprocessor
* C++ translation unit
* C++ compilation (svere sub-steps including template instantiation, overload resolution, ...)
* Build static and dynamic linking
* Runtime environment linking (operating system execution of binary) 

idea: A functional model of 'C++ source code' -> 'Operating system execution' should encapsulate each step as a function f: in -> out?

## Investigations

* [Understanding linking](linking_understanding/README.md)

# build

## conan/cmake framework

This project supports Cmake and Conan package manager.

If you have those installed the following is the proposed build method.

```
conan install . --build=missing
cmake --preset conan-release
cmake --build build/Release
```

NOTE: Also see init_tool_chain.zsh if applicable to your platform

* Cmake support through CMakeLists.txt
* Conan package manager support and directory structure (from 'conan new cmake_exe ...' template)

## Project tracked files

```
.
├── CMakeLists.txt
├── LICENSE
├── README.md
├── _config.yml
├── conanfile.py
├── docs
│   ├── README.md
│   └── linking_understanding
│       ├── README.md
│       ├── chatgpt-qa-linking-phases.md
│       ├── claude-code-qa-linking-stages.md
│       └── image.png
├── index.md
├── init_tool_chain.zsh
├── src
│   ├── bang.cpp
│   ├── bang.h
│   └── main.cpp
```

The cmake support is produced by conan in the build folder with a sub-folder for the build type.

## build folder example (Release built type)

```
├── build
│   └── Release
│       ├── CMakeCache.txt
│       ├── CMakeFiles
│       │   ├── 3.30.5
│       │   │   ├── CMakeCXXCompiler.cmake
│       │   │   ├── CMakeDetermineCompilerABI_CXX.bin
│       │   │   ├── CMakeSystem.cmake
│       │   │   └── CompilerIdCXX
│       │   │       ├── CMakeCXXCompilerId.cpp
│       │   │       ├── CMakeCXXCompilerId.o
│       │   │       └── tmp
│       │   ├── CMakeConfigureLog.yaml
│       │   ├── CMakeDirectoryInformation.cmake
│       │   ├── CMakeScratch
│       │   ├── Makefile.cmake
│       │   ├── Makefile2
│       │   ├── TargetDirectories.txt
│       │   ├── bang.dir
│       │   │   ├── DependInfo.cmake
│       │   │   ├── build.make
│       │   │   ├── cmake_clean.cmake
│       │   │   ├── compiler_depend.internal
│       │   │   ├── compiler_depend.make
│       │   │   ├── compiler_depend.ts
│       │   │   ├── depend.make
│       │   │   ├── flags.make
│       │   │   ├── link.txt
│       │   │   ├── progress.make
│       │   │   └── src
│       │   │       ├── bang.cpp.o
│       │   │       ├── bang.cpp.o.d
│       │   │       ├── main.cpp.o
│       │   │       └── main.cpp.o.d
│       │   ├── cmake.check_cache
│       │   ├── pkgRedirects
│       │   └── progress.marks
│       ├── Makefile
│       ├── bang
│       ├── cmake_install.cmake
│       └── generators
│           ├── CMakePresets.json
│           ├── cmakedeps_macros.cmake
│           ├── conan_toolchain.cmake
│           ├── conanbuild.sh
│           ├── conanbuildenv-release-armv8.sh
│           ├── conandeps_legacy.cmake
│           ├── conanrun.sh
│           ├── conanrunenv-release-armv8.sh
│           ├── deactivate_conanbuild.sh
│           └── deactivate_conanrun.sh

```

## 0.1

Initial proof of concept bare bone C++ console app. It takes a cpp-file as argument and calls clang++ to compile it. It then parses the output for missing header files. If the compiler reports a missing header file, then bang searches all sub-folders two levels up from the cpp-file for the missing file. If it finds it, it adds it as argument '-I<file>' and tries to compile again.

This process repeats until compiler does not report missing header file, or the missing header file is not found.

NOTE: This is a proof of concept and quick and dirty C++ console app code (although in conan package manager file and directory structure).

Example:
```
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/bang % ./build/Release/bang /Users/kjell-olovhogdahl/Documents/GitHub/cratchit/src/main.cpp
bang/0.1: Hello World Release!
  bang/0.1: __aarch64__ defined
  bang/0.1: __cplusplus201703
  bang/0.1: __GNUC__4
  bang/0.1: __GNUC_MINOR__2
  bang/0.1: __clang_major__16
  bang/0.1: __apple_build_version__16000026
bang/0.1 test_package

Executing: "clang++ -H /Users/kjell-olovhogdahl/Documents/GitHub/cratchit/src/main.cpp 2>&1"
Missing header: lua.hpp
Found header at: /Users/kjell-olovhogdahl/Documents/GitHub/cratchit/Xcode/lua-src/src/lua.hpp
try:"clang++ -H -I/Users/kjell-olovhogdahl/Documents/GitHub/cratchit/Xcode/lua-src/src"
Executing: "clang++ -H -I/Users/kjell-olovhogdahl/Documents/GitHub/cratchit/Xcode/lua-src/src /Users/kjell-olovhogdahl/Documents/GitHub/cratchit/src/main.cpp 2>&1"
Missing header: dummy.hpp
Header file not found in the project directories.%                                                                                                                                                                    kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/bang % 

```

