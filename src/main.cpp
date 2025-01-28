#include "bang.h"
#include <vector>
#include <string>

int main(int argc, char *argv[]) {
    if (true) {
        // call conan generated test
        bang::bang_conan_test();

        std::vector<std::string> vec;
        vec.push_back("test_package");

        bang::bang_print_vector(vec);
    }
    return bang::main(argc,argv);
}
