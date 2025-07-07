#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <vector>
#include <map>

namespace bang {

    namespace fs = std::filesystem;

    // Tweak class - aggregates arguments and options
    class Tweak {
    private:
        std::vector<std::string> arguments;  // like "-I/path/to/headers"
        std::map<std::string, bool> options; // like {"-H": true, "-v": false}
        
    public:
        Tweak() {
            // Default options for C++ compilation
            options["-H"] = true;  // Show header includes
        }
        
        void addArgument(const std::string& arg) {
            arguments.push_back(arg);
        }
        
        void setOption(const std::string& opt, bool value) {
            options[opt] = value;
        }
        
        std::string buildCommandLine() const {
            std::string cmd;
            
            // Add options
            for (const auto& [option, enabled] : options) {
                if (enabled) {
                    cmd += option + " ";
                }
            }
            
            // Add arguments
            for (const auto& arg : arguments) {
                cmd += arg + " ";
            }
            
            return cmd;
        }
        
        const std::vector<std::string>& getArguments() const { return arguments; }
        const std::map<std::string, bool>& getOptions() const { return options; }
    };

    // ErrorTransformer - implements t(error, tweaks) → tweaks
    class ErrorTransformer {
    public:
        Tweak transform(const std::string& error, const Tweak& currentTweak, const std::string& inputFile) {
            Tweak newTweak = currentTweak;
            
            // Look for missing header errors
            std::string missingHeader = findMissingHeader(error);
            if (!missingHeader.empty()) {
                std::string headerPath = searchForHeader(inputFile, missingHeader);
                if (!headerPath.empty()) {
                    fs::path includePath = fs::path(headerPath).parent_path();
                    newTweak.addArgument("-I" + includePath.string());
                }
            }
            
            return newTweak;
        }
        
    private:
        std::string findMissingHeader(const std::string& output) {
            std::string errorPrefix = "fatal error: '";
            size_t pos = output.find(errorPrefix);
            if (pos != std::string::npos) {
                size_t start = pos + errorPrefix.length();
                size_t end = output.find("' file not found", start);
                if (end != std::string::npos) {
                    return output.substr(start, end - start);
                }
            }
            return "";
        }
        
        std::string searchForHeader(const std::string& cppFile, const std::string& missingHeader) {
            fs::path cppPath(cppFile);
            fs::path parentDir = cppPath.parent_path();
            
            for (const auto& entry : fs::recursive_directory_iterator(parentDir.parent_path())) {
                if (entry.is_regular_file() && entry.path().filename() == missingHeader) {
                    return entry.path().string();
                }
            }
            return "";
        }
    };

    // Banger class - encapsulates the banging strategy
    class Banger {
    private:
        std::string command;
        Tweak currentTweak;
        ErrorTransformer transformer;
        
    public:
        Banger(const std::string& cmd) : command(cmd) {}
        
        bool bang(const std::string& inputFile, std::string& output) {
            std::string fullCommand = command + " " + currentTweak.buildCommandLine() + inputFile + " 2>&1";
            
            std::cout << "\nExecuting: " << std::quoted(fullCommand);
            
            // Execute command
            output = executeCommand(fullCommand);
            
            // Check if successful (no missing header errors)
            if (transformer.transform(output, currentTweak, inputFile).getArguments().size() == 
                currentTweak.getArguments().size()) {
                // No new tweaks needed - success!
                return true;
            }
            
            // Transform error into new tweaks
            Tweak newTweak = transformer.transform(output, currentTweak, inputFile);
            if (newTweak.getArguments().size() > currentTweak.getArguments().size()) {
                currentTweak = newTweak;
                return false; // Try again with new tweaks
            }
            
            // No more tweaks possible
            return true;
        }
        
    private:
        std::string executeCommand(const std::string& command) {
            std::string result;
            FILE* pipe = popen(command.c_str(), "r");
            if (!pipe) {
                std::cerr << "\nFailed to open pipe\n";
                return result;
            }
            
            char buffer[128];
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                result += buffer;
            }
            
            pclose(pipe);
            return result;
        }
    };

    // Refactored main function using the new banging strategy
    int main(int argc, char* argv[]) {
        if (argc != 2) {
            std::cerr << "Usage: " << argv[0] << " <path_to_cpp_file>\n";
            return 1;
        }

        std::string cppFile = argv[1];
        
        // Create a banger for C++ compilation
        Banger cppBanger("clang++");
        
        std::string output;
        int maxAttempts = 10;  // Prevent infinite loops
        int attempts = 0;
        
        std::cout << "\nBanging C++ compilation for: " << cppFile;
        
        while (attempts < maxAttempts) {
            attempts++;
            
            if (cppBanger.bang(cppFile, output)) {
                std::cout << "\nBanging successful after " << attempts << " attempts!";
                std::cout << "\nNo missing headers detected.";
                break;
            }
            
            // Check if we found a missing header to continue
            if (output.find("' file not found") != std::string::npos) {
                size_t pos = output.find("fatal error: '");
                if (pos != std::string::npos) {
                    size_t start = pos + 14; // length of "fatal error: '"
                    size_t end = output.find("' file not found", start);
                    if (end != std::string::npos) {
                        std::string missingHeader = output.substr(start, end - start);
                        std::cout << "\nMissing header: " << missingHeader;
                        // The banger will try to find and add the include path
                    }
                }
            } else {
                std::cout << "\nNo more missing headers found, but compilation may have other issues.";
                break;
            }
        }
        
        if (attempts >= maxAttempts) {
            std::cerr << "\nStopped after " << maxAttempts << " attempts.";
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


