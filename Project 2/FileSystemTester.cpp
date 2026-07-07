// FileSystemTester.cpp
#include "FileSystem.hpp"
#include <cassert>
#include <iostream>
#include <sstream>

class FileSystemTester {
private:
    int totalScore = 0;
    int totalTests = 0;
    int passedTests = 0;
    std::stringstream testOutput;

    void logTest(const std::string& testName, bool passed, int points) {
        totalTests++;
        if (passed) {
            passedTests++;
            totalScore += points;
            testOutput << "✓ " << testName << " [+" << points << " points]\n";
        } else {
            testOutput << "✗ " << testName << " [+0/" << points << " points]\n";
        }
    }

    bool testMkdir(FileSystem& fs, int points = 5) {
        bool success = true;
        try {
            fs.mkdir("test_dir");
            std::string result = fs.ls();
            if (result.find("test_dir/") == std::string::npos) {
                success = false;
            }

            fs.cd("test_dir");
            std::string pwd_result = fs.pwd();
            if (pwd_result != "/test_dir/") {
                success = false;
            }
            fs.cd("..");

            fs.mkdir("parent");
            fs.cd("parent");
            fs.mkdir("child");
            fs.cd("child");
            pwd_result = fs.pwd();
            if (pwd_result != "/parent/child/") {
                success = false;
            }
            fs.cd("/");

            try {
                fs.mkdir("parent");
                success = false;
            } catch (const std::runtime_error&) {
                // Expected behavior
            }
        } catch (const std::exception& e) {
            success = false;
        }
        logTest("mkdir functionality", success, points);
        return success;
    }

    bool testTouch(FileSystem& fs, int points = 5) {
        bool success = true;
        try {
            fs.touch("test.txt");
            std::string result = fs.ls();
            if (result.find("test.txt") == std::string::npos) {
                success = false;
            }

            fs.mkdir("docs");
            fs.cd("docs");
            fs.touch("doc1.txt");
            fs.touch("doc2.txt");
            result = fs.ls();
            if (result.find("doc1.txt") == std::string::npos || 
                result.find("doc2.txt") == std::string::npos) {
                success = false;
            }
            fs.cd("..");

            try {
                fs.touch("test.txt");
                success = false;
            } catch (const std::runtime_error&) {
                // Expected behavior
            }
        } catch (const std::exception& e) {
            success = false;
        }
        logTest("touch functionality", success, points);
        return success;
    }

    bool testCd(FileSystem& fs, int points = 15) {
        bool success = true;
        try {
            fs.mkdir("dir1");
            fs.cd("dir1");
            fs.mkdir("dir2");
            fs.cd("dir2");
            std::string pwd_result = fs.pwd();
            if (pwd_result != "/dir1/dir2/") {
                success = false;
            }

            fs.cd("..");
            pwd_result = fs.pwd();
            if (pwd_result != "/dir1/") {
                success = false;
            }

            fs.cd("/");
            pwd_result = fs.pwd();
            if (pwd_result != "/") {
                success = false;
            }

            try {
                fs.cd("nonexistent");
                success = false;
            } catch (const std::runtime_error&) {
                // Expected behavior
            }

            fs.touch("file.txt");
            try {
                fs.cd("file.txt");
                success = false;
            } catch (const std::runtime_error&) {
                // Expected behavior
            }
        } catch (const std::exception& e) {
            success = false;
        }
        logTest("cd functionality", success, points);
        return success;
    }

    bool testLs(FileSystem& fs, int points = 10) {
        bool success = true;
        try {
            fs.mkdir("test_ls");
            fs.cd("test_ls");
            fs.touch("file1.txt");
            fs.touch("file2.txt");
            fs.mkdir("subdir");

            std::string result = fs.ls();
            if (result.find("file1.txt") == std::string::npos ||
                result.find("file2.txt") == std::string::npos ||
                result.find("subdir/") == std::string::npos) {
                success = false;
            }
            fs.cd("/");
        } catch (const std::exception& e) {
            success = false;
        }
        logTest("ls functionality", success, points);
        return success;
    }

    bool testPwd(FileSystem& fs, int points = 10) {
        bool success = true;
        try {
            if (fs.pwd() != "/") {
                success = false;
            }

            fs.mkdir("level1");
            fs.cd("level1");
            if (fs.pwd() != "/level1/") {
                success = false;
            }

            fs.mkdir("level2");
            fs.cd("level2");
            if (fs.pwd() != "/level1/level2/") {
                success = false;
            }

            fs.cd("/");
            if (fs.pwd() != "/") {
                success = false;
            }
        } catch (const std::exception& e) {
            success = false;
        }
        logTest("pwd functionality", success, points);
        return success;
    }


    bool testCpOperation(FileSystem& fs, int points = 20) {
    bool success = true;
    try {
        // Setup test environment
        fs.mkdir("source_dir");
        fs.cd("source_dir");
        fs.touch("file1.txt");
        fs.touch("file2.txt");
        fs.mkdir("subdir");
        fs.cd("subdir");
        fs.touch("subfile.txt");
        fs.cd("/");
        
        // Test file copy
        fs.cp("/source_dir/file1.txt", "/copied_file.txt");
        std::string result = fs.ls();
        if (result.find("copied_file.txt") == std::string::npos) {
            success = false;
        }
        
        // Test directory copy
        fs.cp("/source_dir", "/dest_dir");
        fs.cd("/dest_dir");
        result = fs.ls();
        if (result.find("file1.txt") == std::string::npos || 
            result.find("file2.txt") == std::string::npos ||
            result.find("subdir/") == std::string::npos) {
            success = false;
        }
        
        // Test recursive copy
        fs.cd("/dest_dir/subdir");
        result = fs.ls();
        if (result.find("subfile.txt") == std::string::npos) {
            success = false;
        }
        
        // Test error for non-existent source
        try {
            fs.cp("/nonexistent", "/some_dest");
            success = false;
        } catch (const std::runtime_error&) {
            // Expected behavior
        }
        
        // Test error for existing destination
        try {
            fs.cp("/source_dir/file1.txt", "/copied_file.txt");
            success = false;
        } catch (const std::runtime_error&) {
            // Expected behavior
        }
        
        // Reset the current directory
        fs.cd("/");
    } catch (const std::exception& e) {
        success = false;
    }
    logTest("cp operation functionality", success, points);
    return success;
}
    bool testRm(FileSystem& fs, int points = 25) {
        bool success = true;
        try {
            fs.touch("rm_test_file.txt");
            fs.rm("rm_test_file.txt");
            std::string result = fs.ls();
            if (result.find("rm_test_file.txt") != std::string::npos) {
                success = false;
            }

            fs.mkdir("rm_test_dir");
            fs.rm("rm_test_dir");
            result = fs.ls();
            if (result.find("rm_test_dir") != std::string::npos) {
                success = false;
            }

            try {
                fs.rm("nonexistent");
                success = false;
            } catch (const std::runtime_error&) {
                // Expected behavior
            }
        } catch (const std::exception& e) {
            success = false;
        }
        logTest("rm functionality", success, points);
        return success;
    }

public:
    void runTests() {
        std::cout << "Starting FileSystem Tests...\n\n";
        
        FileSystem fs;
        
        testMkdir(fs);    // 5 points
        testTouch(fs);    // 5 points
        testCd(fs);       // 15 points
        testLs(fs);       // 10 points
        testPwd(fs);      // 10 points
        testRm(fs);       // 25 points
        testCpOperation(fs);//20 pts
        
        std::cout << testOutput.str() << "\n";
        std::cout << "Test Summary:\n";
        std::cout << "============\n";
        std::cout << "Total Tests: " << totalTests << "\n";
        std::cout << "Passed Tests: " << passedTests << "\n";
        std::cout << "Total Score: " << totalScore << "/90 points\n";
    }
};

int main() {
    FileSystemTester tester;
    tester.runTests();
    return 0;
}