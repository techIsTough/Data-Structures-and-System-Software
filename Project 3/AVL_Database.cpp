#include "AVL_Database.hpp"
#include <algorithm>
#include <cmath>

// Constructor for Record objects 
Record::Record(const std::string& k, int v) : key(k), value(v) {}

// Constructor for AVLNode objects
AVLNode::AVLNode(Record* r) : record(r), left(nullptr), right(nullptr), height(1) {}

// Constructor for AVLTree objects
AVLTree::AVLTree() : root(nullptr), nodeCount(0), searchComparisonCount(0) {}

// Helper function to return the height of the given node, 0 if it is a nullptr
int AVLTree::height(AVLNode* node) {
    return node ? node->height : 0;
}

// Helper function to update the height of the given node. Checks the heights of the left and right subtrees
// and chooses the larger option
void AVLTree::updateHeight(AVLNode* node) {
    if (node != nullptr) {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }
}

// Helper function to check if the given node is balanced by finding the heights of left and right subtrees
int AVLTree::getBalance(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

// Helper function for restoring the balance of left-heavy target node 
AVLNode* AVLTree::rotateRight(AVLNode* target) {
    if (target == nullptr || target->left == nullptr) {
        return target;
    }

    // Shift nodes to restore balance. target's left child becomes the new subtree root and transfers its right
    // child to target (as its left child) to maintain BST properties 
    AVLNode* left = target->left;
    target->left = left->right;
    left->right = target;

    // Update height of nodes with modified parent-child relationships 
    updateHeight(target);
    updateHeight(left);

    // Return the new root 
    return left;
}

// Helper function for restoring the balance of right-heavy target node 
AVLNode* AVLTree::rotateLeft(AVLNode* target) {
    if (target == nullptr || target->right == nullptr) {
        return target;
    }

    // Shift nodes to restore balance. target's right child becomes the new subtree root and transfers its left
    // child to target (as its right child) to maintain BST properties 
    AVLNode* right = target->right;
    target->right = right->left;
    right->left = target;

    // Update height of nodes with modified parent-child relationships 
    updateHeight(target);
    updateHeight(right);

    // Return the new root
    return right;
}

// Wrapper function for insertion to AVLTree 
void AVLTree::insert(Record* record) {
    root = insertHelper(root, record);
    nodeCount++;
}

// Helper function to isolate recursion and handle the actual insertion 
AVLNode* AVLTree::insertHelper(AVLNode* node, Record* record) {
    if (node == nullptr) {
        AVLNode* newRec = new AVLNode(record);
        return newRec;
    }

    // Use BST properties and recursion to find the proper point of insertion
    // Update heights of nodes with altered parent-child relationships and restore balance
    if (record->value > node->record->value) {
        node->right = insertHelper(node->right, record);
        updateHeight(node);

        node = checkBalance(node, record);
    }
    else if (record->value < node->record->value) {
        node->left = insertHelper(node->left, record);
        updateHeight(node);

        node = checkBalance(node, record);
    }
    
    return node;
}

// Function to check target node's balance and engage appropriate rotations where necessary 
AVLNode* AVLTree::checkBalance(AVLNode* target, Record* record) {
    int newBal = getBalance(target);

    // Case 1: LL insertion, single right rotation 
    if (newBal > 1 && record->value < target->left->record->value) {
        return rotateRight(target);
    }
    // Case 2: RR insertion, single left rotation
    if (newBal < -1 && record->value > target->right->record->value) {
        return rotateLeft(target);
    }
    // Case 3: LR insertion, double rotation. First rotation sets up second typical right rotation
    if (newBal > 1 && record->value > target->left->record->value) {
        target->left = rotateLeft(target->left);
        return rotateRight(target);
    }
    // Case 4: RL insertion, double rotation. First rotation sets up second typical left rotation
    if (newBal < -1 && record->value < target->right->record->value) {
        target->right = rotateRight(target->right);
        return rotateLeft(target);
    }

    return target;
}

// Wrapper function for deletion of node from AVLTree
void AVLTree::deleteNode(const std::string& key, int value) {
    root = deleteHelper(root, key, value);
}

// Helper function to isolate recursion and handle actual deletion 
AVLNode* AVLTree::deleteHelper(AVLNode* node, const std::string& key, int value) {
    if (node == nullptr) {
        return node;
    }

    // Utilize BST properties to search for target node. Else block executes upon finding target
    if (value < node->record->value) {
        node->left = deleteHelper(node->left, key, value);
    }
    else if (value > node->record->value) {
        node->right = deleteHelper(node->right, key, value);
    }
    else {
        // The value is correct but check the key as well in case two nodes have the same value 
        if (node->record->key != key) {
            return node;
        }

        // Handles cases where target node has one or zero children. Replace target node with its left
        // or right child and connect it back to target's parent by travelling back through recursion 
        if (node->left == nullptr) {
            AVLNode* tmp = node->right;
            delete node->record;
            delete node;
            nodeCount--;
            return tmp;
        }
        else if (node->right == nullptr) {
            AVLNode* tmp = node->left;
            delete node->record;
            delete node;
            nodeCount--;
            return tmp;
        }
        
        // Case where target node has two children. Replace target with its in-order successor, the smallest
        // node greater than target, located using BST properties. 
        AVLNode* tmp = node->right;
        while (tmp->left != nullptr) {
            tmp = tmp->left;
        }

        // Overwrite target's data with that of its in-order successor, effectively deleting it
        node->record->key = tmp->record->key;
        node->record->value = tmp->record->value;

        // Traverse deeper into the tree to remove the successor and potentially address further removals 
        node->right = deleteHelper(node->right, tmp->record->key, tmp->record->value);
    }
    // At the end of each recursive call, update the height of the current node and rebalance if necessary
    updateHeight(node);
    node = checkBalance(node, node->record);

    return node;
}

// Wrapper function for searching AVLTree 
Record* AVLTree::search(const std::string& key, int value) {
    searchComparisonCount = 0;      // Needed for db_driver file
    AVLNode* result = searchHelper(root, key, value);

    // If the target wasn't found, create a new record with default values and return 
    if (result == nullptr) {
        Record* blankRecord = new Record("", 0);
        return blankRecord;
    }

    // Otherwise, return the target
    return result->record;
}

// Helper function for isolating recursion and actually finding the target. Utilize recursion and BST properties 
// for traversal and return target if found
AVLNode* AVLTree::searchHelper(AVLNode* node, const std::string& key, int value) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (value == node->record->value && key == node->record->key) {
        searchComparisonCount++;        // Needed for db_driver file
        return node;
    }   
    if (value > node->record->value) {
        searchComparisonCount++;        // Needed for db_driver file
        return searchHelper(node->right, key, value);
    }
    if (value < node->record->value) {
        searchComparisonCount++;        // Needed for db_driver file
        return searchHelper(node->left, key, value);
    }

    return nullptr;     // Not really needed, but silences a compiler warning 
}

// IndexedDatabase implementation. Uses AVLTree class under the hood for all operations 
void IndexedDatabase::insert(Record* record) {
    index.insert(record);
}

Record* IndexedDatabase::search(const std::string& key, int value) {
    return index.search(key, value);
}

void IndexedDatabase::deleteRecord(const std::string& key, int value) {
    index.deleteNode(key, value);
}

// Wrapper function for range query operation that searches AVL tree for nodes in the inclusive range [start,end] based on their value
std::vector<Record*> IndexedDatabase::rangeQuery(int start, int end) {
    std::vector<Record*> result;
    rangeQueryHelper(index.root, start, end, result);

    return result;
}

// Helper function for isolating recursion and accumulating vector of Record objects in the inclusive range [start,end] based on their value
void IndexedDatabase::rangeQueryHelper(AVLNode* node, int start, int end, std::vector<Record*>& result) const {
    if (node == nullptr) {
        return;
    }
    // There may be relevant Record objects to the left, search there as well
    if (start < node->record->value) {
        rangeQueryHelper(node->left, start, end, result);
    }
    // Relevant Record object found, push it to the result vector and continue searching, either engaging in further
    // recursive calls or moving back up the recursive stack 
    if (start <= node->record->value && node->record->value <= end) {
        result.push_back(node->record);
    }
    // There may be relevant Record objects to the right, search there as well
    if (end > node->record->value) {
        rangeQueryHelper(node->right, start, end, result);
    }
}

// Helper function for deleting IndexedDatabase object. Deletes each node and the associated Record object recursively
void IndexedDatabase::clearHelper(AVLNode* node) {
    if (!node) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node->record;
    delete node;
}

// Clears out IndexedDatabase object
void IndexedDatabase::clearDatabase() {
    clearHelper(index.root);
    index.root = nullptr;
}

// Helper function for finding the height of a given node in an IndexedDatabase object
int IndexedDatabase::calculateHeight(AVLNode* node) const {
    if (!node) return 0;
    return 1 + std::max(calculateHeight(node->left), calculateHeight(node->right));
}

// Returns the height of an IndexedDatabase AVL tree
int IndexedDatabase::getTreeHeight() const {
    return calculateHeight(index.root);
}

// Needed for db_driver file
int IndexedDatabase::getSearchComparisons(const std::string& key, int value) {
    search(key, value);
    return index.getLastSearchComparisons();
}