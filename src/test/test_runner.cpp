#include "test_runner.hpp"
#include "test_atomics.hpp"
#include "test_integrations.hpp"
#include "test_fixtures.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <filesystem>

namespace tests {
    // GoogleTest Environment implementation - extends the base class from header
    class TestEnvironmentImpl : public TestEnvironment, public ::testing::Environment {
    public:
        
        void SetUp() override {
            // Get current directory (workspace where bang is running)
            workspaceDir = std::filesystem::current_path().string();
            
            // Construct path to bang executable (in same directory)
            std::filesystem::path bangPath = std::filesystem::current_path() / "bang";
            bangExecutable = bangPath.string();
            
            std::cout << "=== Test Environment Setup ===" << std::endl;
            std::cout << "Workspace directory: " << workspaceDir << std::endl;
            std::cout << "Bang executable: " << bangExecutable << std::endl;
            
            // Verify bang executable exists
            if (!std::filesystem::exists(bangExecutable)) {
                std::cerr << "WARNING: Bang executable not found at: " << bangExecutable << std::endl;
            }
            std::cout << "===============================" << std::endl;
        }
        
        void TearDown() override {
            std::cout << "=== Test Environment Cleanup ===" << std::endl;
        }
    };
    
    // Global test environment instance
    TestEnvironment* g_testEnv = nullptr;
    
    bool run_all() {
        std::cout << "Running all tests..." << std::endl;
        
        // Create and register global test environment
        auto* envImpl = new TestEnvironmentImpl();
        g_testEnv = envImpl;  // Store in global pointer
        ::testing::AddGlobalTestEnvironment(envImpl);
        
        // Initialize GoogleTest once at the top level
        ::testing::InitGoogleTest();
        
        bool atomics_pass = atomics::run_all();
        bool integrations_pass = integrations::run_all();
        
        bool all_pass = atomics_pass && integrations_pass;
        
        std::cout << "All tests " << (all_pass ? "PASSED" : "FAILED") << std::endl;
        return all_pass;
    }
}