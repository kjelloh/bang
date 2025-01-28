#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <vector>

namespace bang {

    namespace fs = std::filesystem;

    // Function to invoke clang++ with the -H flag and capture its output
    std::string runClangH(const std::string& command_head, const std::string& cppFile) {
    std::string command = command_head + " " + cppFile + " 2>&1";  // Redirect stderr to stdout
    std::string result;
    
    std::cout << "\nExecuting: " << std::quoted(command);
    
    // Open a pipe to the clang++ command
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "\nFailed to open pipe\n";
        exit(1);
    }
    
    // Read the output of the command
    char buffer[128];
    size_t totalBytesRead = 0;
    
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        totalBytesRead += strlen(buffer);
        result += buffer;
    }
    
    if (totalBytesRead == 0) {
        std::cerr << "No output read from the pipe.\n";
    }
    
    pclose(pipe);
    return result;
    }

    // Function to check if the output contains a 'file not found' error
    std::string findMissingHeader(const std::string& output) {
    std::string errorPrefix = "fatal error: '";
    size_t pos = output.find(errorPrefix);
    if (pos != std::string::npos) {
        size_t start = pos + errorPrefix.length();
        size_t end = output.find("' file not found", start);
        if (end != std::string::npos) {
        return output
            .substr(start, end - start);  // Return the missing header filename
        }
    }
    return "";
    }

    // Function to search for the header file in the directory structure
    std::string searchForHeader(const std::string& cppFile,
                                const std::string& missingHeader) {
    fs::path cppPath(cppFile);
    fs::path parentDir = cppPath.parent_path();  // Directory where the .cpp file is located
        
    // Traverse directories around the cpp file (e.g., the parent directory and subdirectories)
    for (const auto& entry : fs::recursive_directory_iterator(parentDir.parent_path())) {
        if (entry.is_regular_file() && entry.path().filename() == missingHeader) {
        return entry.path().string();
        }
    }
    return "";  // Return an empty string if the header is not found
    }

    // Main function that orchestrates the process
    int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_cpp_file>\n";
        return 1;
    }

    std::string cppFile = argv[1];
    
    std::string command_head = "clang++ -H";
    
    while (true) {
        // Step 1: Run clang++ -H to see the header search process
        std::string output = runClangH(command_head,cppFile);

        // Step 2: Find if a header file is missing
        if (auto missingHeader = findMissingHeader(output); false == missingHeader.empty()) {
        std::cout << "\nMissing header: " << missingHeader;
        
        // Step 3: Search for the missing header in the directory structure
        std::string foundHeaderPath = searchForHeader(cppFile, missingHeader);
        if (!foundHeaderPath.empty()) {
            std::cout << "\nFound header at: " << foundHeaderPath;
            
            // Step 4: Re-run clang++ with the correct include path
            command_head += std::string{" -I"} + fs::path(foundHeaderPath).parent_path().string();
            std::cout << "\ntry:" << std::quoted(command_head);
        } else {
            std::cerr << "\nHeader file not found in the project directories.";
            break;
        }
        }
        else {
        std::cout << "\nNo missing headers detected.";
        break;
        }
    }
        
    return 0;
    }

    // conan generated bang() test method (renamed)
    void bang_conan_test(){
        

        #ifdef NDEBUG
        std::cout << "bang/0.1: Hello World Release!\n";
        #else
        std::cout << "bang/0.1: Hello World Debug!\n";
        #endif

        // ARCHITECTURES
        #ifdef _M_X64
        std::cout << "  bang/0.1: _M_X64 defined\n";
        #endif

        #ifdef _M_IX86
        std::cout << "  bang/0.1: _M_IX86 defined\n";
        #endif

        #ifdef _M_ARM64
        std::cout << "  bang/0.1: _M_ARM64 defined\n";
        #endif

        #if __i386__
        std::cout << "  bang/0.1: __i386__ defined\n";
        #endif

        #if __x86_64__
        std::cout << "  bang/0.1: __x86_64__ defined\n";
        #endif

        #if __aarch64__
        std::cout << "  bang/0.1: __aarch64__ defined\n";
        #endif

        // Libstdc++
        #if defined _GLIBCXX_USE_CXX11_ABI
        std::cout << "  bang/0.1: _GLIBCXX_USE_CXX11_ABI "<< _GLIBCXX_USE_CXX11_ABI << "\n";
        #endif

        // MSVC runtime
        #if defined(_DEBUG)
            #if defined(_MT) && defined(_DLL)
            std::cout << "  bang/0.1: MSVC runtime: MultiThreadedDebugDLL\n";
            #elif defined(_MT)
            std::cout << "  bang/0.1: MSVC runtime: MultiThreadedDebug\n";
            #endif
        #else
            #if defined(_MT) && defined(_DLL)
            std::cout << "  bang/0.1: MSVC runtime: MultiThreadedDLL\n";
            #elif defined(_MT)
            std::cout << "  bang/0.1: MSVC runtime: MultiThreaded\n";
            #endif
        #endif

        // COMPILER VERSIONS
        #if _MSC_VER
        std::cout << "  bang/0.1: _MSC_VER" << _MSC_VER<< "\n";
        #endif

        #if _MSVC_LANG
        std::cout << "  bang/0.1: _MSVC_LANG" << _MSVC_LANG<< "\n";
        #endif

        #if __cplusplus
        std::cout << "  bang/0.1: __cplusplus" << __cplusplus<< "\n";
        #endif

        #if __INTEL_COMPILER
        std::cout << "  bang/0.1: __INTEL_COMPILER" << __INTEL_COMPILER<< "\n";
        #endif

        #if __GNUC__
        std::cout << "  bang/0.1: __GNUC__" << __GNUC__<< "\n";
        #endif

        #if __GNUC_MINOR__
        std::cout << "  bang/0.1: __GNUC_MINOR__" << __GNUC_MINOR__<< "\n";
        #endif

        #if __clang_major__
        std::cout << "  bang/0.1: __clang_major__" << __clang_major__<< "\n";
        #endif

        #if __clang_minor__
        std::cout << "  bang/0.1: __clang_minor__" << __clang_minor__<< "\n";
        #endif

        #if __apple_build_version__
        std::cout << "  bang/0.1: __apple_build_version__" << __apple_build_version__<< "\n";
        #endif

        // SUBSYSTEMS

        #if __MSYS__
        std::cout << "  bang/0.1: __MSYS__" << __MSYS__<< "\n";
        #endif

        #if __MINGW32__
        std::cout << "  bang/0.1: __MINGW32__" << __MINGW32__<< "\n";
        #endif

        #if __MINGW64__
        std::cout << "  bang/0.1: __MINGW64__" << __MINGW64__<< "\n";
        #endif

        #if __CYGWIN__
        std::cout << "  bang/0.1: __CYGWIN__" << __CYGWIN__<< "\n";
        #endif
    }

    // conan generated method
    void bang_print_vector(const std::vector<std::string> &strings) {
        for(std::vector<std::string>::const_iterator it = strings.begin(); it != strings.end(); ++it) {
            std::cout << "bang/0.1 " << *it << std::endl;
        }
    }

}


