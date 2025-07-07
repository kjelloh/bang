#include "test_fixtures.hpp"
#include "test_runner.hpp"  // For access to g_testEnv
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <ctime>

namespace tests::fixtures {
        
    void BangingTestFixture::SetUp() {
            // Access the global test environment we set up
            if (tests::g_testEnv) {
                workspaceDir = tests::g_testEnv->workspaceDir;
                bangExecutable = tests::g_testEnv->bangExecutable;
            } else {
                // Fallback to current directory
                workspaceDir = std::filesystem::current_path().string();
                bangExecutable = std::filesystem::current_path() / "bang";
            }
            
            // Create unique test directory for this test
            testDir = workspaceDir + "/test_scenarios_" + std::to_string(std::time(nullptr));
            std::filesystem::create_directories(testDir);
            
            std::cout << "Created test directory: " << testDir << std::endl;
        }
        
    void BangingTestFixture::TearDown() {
            // Clean up test directory
            if (std::filesystem::exists(testDir)) {
                std::filesystem::remove_all(testDir);
                std::cout << "Cleaned up test directory: " << testDir << std::endl;
            }
        }
        
    void BangingTestFixture::createScenario1() {
            // Create directory structure: scenario1/main.cpp includes missing header
            std::string scenario = testDir + "/scenario1";
            std::filesystem::create_directories(scenario + "/libs/include");
            
            // Create main.cpp with missing include
            writeFile(scenario + "/main.cpp", R"(#include <iostream>
#include "myheader.h"

int main() { 
    std::cout << "Hello from scenario1!" << std::endl;
    return 0; 
})");
            
            // Create the header file in a subdirectory that bang should find
            writeFile(scenario + "/libs/include/myheader.h", R"(#pragma once
#define FOUND_HEADER 1
// This header was found by bang!
)");
        }
        
    void BangingTestFixture::createScenario2() {
            // More complex scenario with nested paths
            std::string scenario = testDir + "/scenario2";
            std::filesystem::create_directories(scenario + "/deep/nested/path");
            std::filesystem::create_directories(scenario + "/another/location");
            
            writeFile(scenario + "/source.cpp", R"(#include <iostream>
#include "utils.h"
#include "config.h"

int main() {
    std::cout << "Hello from scenario2!" << std::endl;
    return 0;
})");
            
            writeFile(scenario + "/deep/nested/path/utils.h", "#pragma once\n// Utils header");
            writeFile(scenario + "/another/location/config.h", "#pragma once\n// Config header");
        }
        
    bool BangingTestFixture::runBangOn(const std::string& cppFile) {
            std::string command = bangExecutable + " \"" + cppFile + "\"";
            std::cout << "Executing: " << command << std::endl;
            int result = std::system(command.c_str());
            return result == 0;
        }
        
    void BangingTestFixture::writeFile(const std::string& path, const std::string& content) {
            std::ofstream file(path);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to create file: " + path);
            }
            file << content;
            file.close();
        }
}