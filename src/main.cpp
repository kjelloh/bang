#include "bang.h"
#include "test/test_runner.hpp"
#include <vector>
#include <string>
#include <cstring>

int main(int argc, char *argv[]) {
    // Check if --test argument is passed
    if (argc > 1 && std::strcmp(argv[1], "--test") == 0) {
        return tests::run_all();
    }
    
    if (true) {
        // call conan generated test
        bang::bang_conan_test();

        std::vector<std::string> vec;
        vec.push_back("test_package");

        bang::bang_print_vector(vec);
    }
    return bang::main(argc,argv);
}
