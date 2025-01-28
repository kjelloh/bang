cmake_minimum_required(VERSION 3.23)
project(bang CXX)



add_executable(bang src/bang.cpp src/main.cpp)



install(TARGETS bang DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
