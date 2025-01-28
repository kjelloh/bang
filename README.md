# bang
C++ compiler utiliser to iterate cpp-file-compile command with new arguments until it compiles

bang © 2025 by Kjell-Olov Högdal is licensed under Creative Commons Attribution 4.0 International. To view a copy of this license, visit https://creativecommons.org/licenses/by/4.0/

* Cmake support through CMakeLists.txt
* Conan package manager support and directory structure (from 'conan new cmake_exe ...' template)

```
.
├── CMakeLists.txt
├── CMakeUserPresets.json
├── LICENSE
├── README.md
├── conanfile.py
├── src
│   ├── bang.cpp
│   ├── bang.h
│   └── main.cpp
└── test_package
    └── conanfile.py
```

# build

This project supports Cmake and Conan package manager.

If you have those installed the following is the proposed build method.

```
conan install . --build=missing
cmake --preset conan-release
cmake --build build/Release
```

NOTE: Also see init_tool_chain.zsh if applicable to your platform

## 0.1

Initial proof of concept bare bone C++ console app. It takes a cpp-file as argument and calls clang++ to compile it. It then parses the output for missing header files. If the compiler reports a missing header file, then bang searches all sub-folders two levels up from the cpp-file for the missing file. If it finds it, it adds it as argument '-I<file>' and tries to compile again.

This process repeats until compiler does not report missing header file, or the missing header file is not found.

NOTE: This is a proof of concept and quick and dirty C++ console app code (although in conan package manager file and directory structure).

