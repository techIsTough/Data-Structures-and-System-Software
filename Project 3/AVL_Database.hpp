#ifndef AVL_DATABASE_HPP
#define AVL_DATABASE_HPP

#include <string>
#include <vector>
#include <queue>    // UNUSED

class Record {
public:
    std::string key;
    int value;
    
    Record(const std::string& k, int v);
};

class AVLNode {
public:
    Record* record;
    AVLNode* left;
    AVLNode* right;
    int height;
    
    AVLNode(Record* r);
};

class AVLTree {
private:
    AVLNode* root;
    int nodeCount;
    mutable int searchComparisonCount;  // For measuring search complexity
    
    int height(AVLNode* node);
    int getBalance(AVLNode* node);
    void updateHeight(AVLNode* node);
    
    AVLNode* rotateRight(AVLNode* target);
    AVLNode* rotateLeft(AVLNode* target);
    
    AVLNode* insertHelper(AVLNode* node, Record* record);
    AVLNode* checkBalance(AVLNode* target, Record* record);
    AVLNode* deleteHelper(AVLNode* node, const std::string& key, int value);
    AVLNode* searchHelper(AVLNode* node, const std::string& key, int value) const;
    AVLNode* minValueNode(AVLNode* node); // UNUSED
    
    friend class IndexedDatabase;

public:
    AVLTree();
    void insert(Record* record);
    Record* search(const std::string& key, int value);
    void deleteNode(const std::string& key, int value);
    int getNodeCount() const { return nodeCount; }
    int getLastSearchComparisons() const { return searchComparisonCount; }
};

class IndexedDatabase {
private:
    AVLTree index;
    
    void inorderHelper(AVLNode* node, std::vector<Record*>& result) const; // UNUSED
    void rangeQueryHelper(AVLNode* node, int start, int end, std::vector<Record*>& result) const;
    void clearHelper(AVLNode* node);
    int calculateHeight(AVLNode* node) const;

public:
    void insert(Record* record);
    Record* search(const std::string& key, int value);
    void deleteRecord(const std::string& key, int value);
    std::vector<Record*> rangeQuery(int start, int end);
    std::vector<Record*> findKNearestKeys(int key, int k); // UNUSED
    std::vector<Record*> inorderTraversal(); // UNUSED
    void clearDatabase();
    int countRecords() { return index.getNodeCount(); }
    
    // New methods for testing
    int getSearchComparisons(const std::string& key, int value);
    int getTreeHeight() const;
};

#endif // AVL_DATABASE_HPP