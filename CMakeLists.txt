cmake_minimum_required(VERSION 3.23)
project(bang CXX)

# Find required packages
find_package(GTest REQUIRED)

add_executable(bang 
    src/bang.cpp 
    src/main.cpp
    src/test/test_runner.cpp
    src/test/test_atomics.cpp
    src/test/test_fixtures.cpp
    src/test/test_integrations.cpp
)
target_link_libraries(bang GTest::gtest)

install(TARGETS bang DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
