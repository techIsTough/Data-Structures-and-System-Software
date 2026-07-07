// db_driver.cpp
#include "AVL_Database.hpp"
#include <cassert>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int main() {
    IndexedDatabase db;
    int totalTests = 0;
    int passedTests = 0;

    auto printTest = [&totalTests, &passedTests](const string& testName, bool passed) {
        totalTests++;
        if (passed) {
            passedTests++;
            cout << "✓ Test " << setw(2) << totalTests << ": " << testName << " - PASSED" << endl;
        } else {
            cout << "✗ Test " << setw(2) << totalTests << ": " << testName << " - FAILED" << endl;
        }
    };

    // Test Group 1: Basic AVL Tree Properties
    cout << "\nTesting AVL Tree Properties:" << endl;
    {
        // Insert classic books in non-sorted order to test AVL balancing
        auto r1 = new Record("1984", 40);                    // Orwell
        auto r2 = new Record("Pride and Prejudice", 20);     // Austen
        auto r3 = new Record("War and Peace", 70);          // Tolstoy
        auto r4 = new Record("The Great Gatsby", 10);       // Fitzgerald
        auto r5 = new Record("To Kill a Mockingbird", 50);  // Lee

        // Add books to show insertion order affects tree balance
        db.insert(r1);         // Should become root
        db.insert(r2);         // Goes left
        db.insert(r3);         // Goes right
        db.insert(r4);         // Goes left-left
        db.insert(r5);         // Goes right-left

        // Test 1: Record Count
        printTest("Record Count", db.countRecords() == 5);

        // Test 2: Search Complexity Tests
        int comparisons = db.getSearchComparisons("1984", 40);
        printTest("Search Complexity (Root)", comparisons == 1);

        comparisons = db.getSearchComparisons("The Great Gatsby", 10);
        printTest("Search Complexity (Leaf)", comparisons <= ceil(log2(6)));

        // Test 3: Height Verification
        printTest("Tree Height", db.getTreeHeight() <= ceil(log2(6)));
    }

    // Test Group 2: Search Operations
    cout << "\nTesting Search Operations:" << endl;
    {
        // Test 4: Existing Record Search
        Record* found = db.search("1984", 40);
        printTest("Existing Record Search", found->key == "1984" && found->value == 40);

        // Test 5: Non-existent Record Search
        found = db.search("Don Quixote", 100);
        printTest("Non-existent Record Search", found->key == "" && found->value == 0);

        // Test 6: Boundary Value Search
        found = db.search("The Great Gatsby", 10);
        printTest("Minimum Value Search", found->key == "The Great Gatsby" && found->value == 10);
    }

    // Test Group 3: Delete Operations
    cout << "\nTesting Delete Operations:" << endl;
    {
        int initialCount = db.countRecords();
        
        // Test 7: Delete Leaf Node
        db.deleteRecord("The Great Gatsby", 10);
        printTest("Delete Leaf Node", db.countRecords() == initialCount - 1);

        // Test 8: Delete Internal Node
        db.deleteRecord("Pride and Prejudice", 20);
        printTest("Delete Internal Node", db.countRecords() == initialCount - 2);

        // Test 9: Delete Non-existent Node
        int countBefore = db.countRecords();
        db.deleteRecord("Moby Dick", 100);
        printTest("Delete Non-existent Node", db.countRecords() == countBefore);
    }

    // Test Group 4: Range Queries
    cout << "\nTesting Range Queries:" << endl;
    {
        // Test 10: Valid Range
        auto range = db.rangeQuery(40, 70);
        printTest("Valid Range Query", range.size() == 3);

        // Test 11: Empty Range
        range = db.rangeQuery(100, 200);
        printTest("Empty Range Query", range.empty());

        // Test 12: Single Value Range
        range = db.rangeQuery(50, 50);
        printTest("Single Value Range", range.size() == 1 && range[0]->value == 50);
    }

    // Test Group 5: Stress Test with Many Classic Books
    cout << "\nTesting Edge Cases and Stress:" << endl;
    {
        const int STRESS_SIZE = 1000;
        vector<string> classicBooks = {
            "Oliver Twist", "Jane Eyre", "Wuthering Heights", 
            "Les Misérables", "Crime and Punishment", "Anna Karenina",
            "The Odyssey", "Don Quixote", "The Divine Comedy"
            // Add more as needed for larger tests
        };

        // Generate many records with classic book names
        for (int i = 0; i < STRESS_SIZE; i++) {
            string bookName = classicBooks[i % classicBooks.size()] + " Vol." + to_string(i/classicBooks.size() + 1);
            db.insert(new Record(bookName, i));
        }
        
        // Verify logarithmic search time
        int maxComparisons = 0;
        for (int i = 0; i < 10; i++) {
            int randVal = rand() % STRESS_SIZE;
            string bookName = classicBooks[randVal % classicBooks.size()] + " Vol." + to_string(randVal/classicBooks.size() + 1);
            int comp = db.getSearchComparisons(bookName, randVal);
            maxComparisons = max(maxComparisons, comp);
        }
        
        printTest("Stress Test - Search Complexity", 
                 maxComparisons <= 2 * ceil(log2(STRESS_SIZE)));

        // Clean up
        db.clearDatabase();
    }

    // Print Summary
    cout << "\nTest Summary:" << endl;
    cout << "Tests Passed: " << passedTests << "/" << totalTests 
         << " (" << fixed << setprecision(2) 
         << (passedTests * 100.0 / totalTests) << "%)" << endl;

    return 0;
}