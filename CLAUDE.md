# CLAUDE.md - Project Status

## Project Overview

**Bang** is a command-line tool that "bangs" system commands - automatically searching for the right tweaks to make any command succeed.

### Core Concept

Bang implements the paradigm: `f(in, tweaks) → out` where:
- `f` = any operating system command/function
- `in` = input file(s)  
- `tweaks` = (arguments, options) that modify how `f` operates
- `out` = output file(s)

**The "Banging" Process:**
1. Bang tries `f(in, tweaks)` with initial tweaks
2. If it fails, Bang analyzes the error and intelligently updates tweaks using: `t(error, tweaks) → tweaks`
3. Bang keeps "banging" (trying different tweak combinations) until the command succeeds

## Current Implementation Status

### ✅ Completed Features

**Core Architecture:**
- `Tweak` class - Aggregates command arguments and boolean options
- `ErrorTransformer` class - Implements intelligent error analysis `t(error, tweaks) → tweaks`
- `Banger` class - Encapsulates the banging strategy with iterative execution
- Refactored codebase using proper "banging" terminology

**C++ Compilation Support:**
- Currently supports "banging" C++ compilation with `clang++`
- Automatically finds missing header files in project directory tree
- Adds `-I` include paths until compilation succeeds
- Handles multiple missing headers in sequence

**Build System:**
- Conan/CMake framework with proper preset support
- Cross-platform build scripts (`init_tool_chain.zsh`, `run.zsh`)
- Support for Debug/Release/RelWithDebInfo/MinSizeRel build types

**Testing Framework:**
- GoogleTest integration with `--test` flag
- GoogleTest Environment for cross-platform test execution
- Comprehensive test structure:
  - Atomic tests for unit testing components
  - Integration tests for end-to-end "banging" scenarios
  - Test fixtures for creating controlled test environments
- Integration tests can create C++ files with missing headers and verify bang finds them

**Documentation System:**
- Structured Q&A documentation with separate markdown files
- Cross-referenced insights from ChatGPT and Claude Code Assistant
- YouTube video note-taking with timestamped references
- Platform comparison documentation (macOS vs Linux commands)

### 🚧 Current Architecture

```
src/
├── bang.cpp          # Core implementation with Tweak, Banger, ErrorTransformer classes
├── bang.h            # Header declarations
├── main.cpp          # Entry point with --test flag support
└── test/
    ├── test_runner.*     # GoogleTest Environment and test orchestration
    ├── test_fixtures.*   # BangingTestFixture for integration testing
    ├── test_integrations.* # Integration tests for banging scenarios
    └── test_atomics.*    # Unit tests for individual components
```

### 🎯 Current Capabilities

**C++ Compilation Banging:**
- Input: C++ source file with missing `#include` statements
- Process: Automatically searches project directory tree for headers
- Output: Successfully compiled executable with correct `-I` paths

**Example Usage:**
```bash
# Bang a C++ file until it compiles
./bang path/to/source.cpp

# Run comprehensive test suite
./bang --test
```

### 🔬 Testing Capabilities

**Integration Test Framework:**
- Creates temporary test scenarios with known file structures
- Tests bang's ability to find headers in nested directory structures
- Verifies complete compilation workflow
- Cross-platform compatible test environment

**Example Test Scenarios:**
1. Single missing header in subdirectory
2. Multiple missing headers in different nested paths
3. Complex project structures with deep nesting

### 📋 Future Expansion Potential

The architecture is designed to extend beyond C++ compilation:

**Potential Commands to "Bang":**
- `ffmpeg` - Find correct codec/format combinations
- `pandoc` - Resolve template and format dependencies  
- `git` - Try different merge/rebase strategies
- Any command that requires trial-and-error parameter tuning

### 🛠 Development Tools

**Build Commands:**
```bash
./init_tool_chain.zsh [Debug|Release]  # Initialize build environment
./run.zsh                              # Build and run main functionality
./run.zsh --test                       # Build and run test suite
```

**Documentation:**
- `docs/README.md` - Main project documentation
- `docs/linking_understanding/` - C++ linking knowledge base
- Cross-referenced Q&A sessions with AI assistants

### 🧬 Code Architecture Highlights

**Error-Driven Intelligence:**
- ErrorTransformer analyzes command output to determine next tweaks
- Learns from each failed attempt to make increasingly informed decisions
- Prevents infinite loops with maximum attempt limits

**Cross-Platform Design:**
- Uses std::filesystem for portable file operations
- GoogleTest Environment for consistent test execution
- Proper CMake/Conan integration for dependency management

### 🎉 Key Achievements

1. **Conceptual Evolution** - Transformed from simple C++ helper to generalized command optimizer
2. **Robust Architecture** - Clean separation of concerns with testable components  
3. **Integration Testing** - Real-world testing of banging functionality
4. **Documentation System** - Systematic knowledge capture and organization
5. **Cross-Platform Support** - Works on macOS and Linux with appropriate tooling

This project demonstrates intelligent command-line automation that learns from failures to achieve success through systematic parameter exploration.