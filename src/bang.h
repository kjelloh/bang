#pragma once

#include <vector>
#include <string>


#ifdef _WIN32
  #define BANG_EXPORT __declspec(dllexport)
#else
  #define BANG_EXPORT
#endif

namespace bang {
  BANG_EXPORT int main(int argc, char* argv[]);
  BANG_EXPORT void bang_conan_test();
  BANG_EXPORT void bang_print_vector(const std::vector<std::string> &strings);
}
