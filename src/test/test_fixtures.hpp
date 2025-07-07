#pragma once
#include <gtest/gtest.h>
#include <string>

namespace tests::fixtures {
    // BangingTestFixture for integration testing
    class BangingTestFixture : public ::testing::Test {
    protected:
        std::string testDir;
        std::string workspaceDir;
        std::string bangExecutable;
        
        void SetUp() override;
        void TearDown() override;
        void createScenario1();
        void createScenario2();
        bool runBangOn(const std::string& cppFile);
        
    private:
        void writeFile(const std::string& path, const std::string& content);
    };
}