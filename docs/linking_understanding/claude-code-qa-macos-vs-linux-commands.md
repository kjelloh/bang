# macOS vs Linux Commands for Binary Analysis - Q&A with Claude Code Assistant

## Question

On Linux there seems to be a command 'readelf'? But on my macOS I fail to execute such a command. Is there a macOS-command with the same function as readelf on Linux?

---

## Answer

On macOS, the equivalent of Linux's `readelf` is `otool`. Here are the common equivalents:

**Linux → macOS equivalents:**
- `readelf -h file` → `otool -h file` (header info)
- `readelf -l file` → `otool -l file` (load commands/segments)
- `readelf -S file` → `otool -S file` (sections)
- `readelf -s file` → `nm file` (symbols)
- `readelf -d file` → `otool -L file` (dynamic dependencies)

**Additional macOS tools:**
- `otool -tv file` - disassemble text section
- `otool -arch arm64 file` - specify architecture
- `file file` - basic file type (you already have this)

**Example with your executable:**
```bash
otool -L build/Release/bang    # Show dynamic libraries
otool -h build/Release/bang    # Show Mach-O header
otool -l build/Release/bang    # Show load commands
```

macOS uses Mach-O format instead of ELF, so `otool` is the native tool for inspecting Mach-O binaries, just like `readelf` is for ELF binaries on Linux.