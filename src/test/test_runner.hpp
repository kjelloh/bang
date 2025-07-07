#pragma once
#include <string>

namespace tests {
    // TestEnvironment class definition
    class TestEnvironment {
    public:
        std::string workspaceDir;
        std::string bangExecutable;
    };
    
    // Global test environment instance
    extern TestEnvironment* g_testEnv;
    
    bool run_all();
}