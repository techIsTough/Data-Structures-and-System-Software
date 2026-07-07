#include "FileSystem.hpp"
#include <stdexcept>
#include <sstream>

FileSystemNode::FileSystemNode(std::string name, bool isDir) 
    : name(name), isDirectory(isDir), parent(nullptr) {}

FileSystemNode::~FileSystemNode() {
    for (auto child : children) {
        delete child;
    }
}

FileSystem::FileSystem() {
    root = new FileSystemNode("/", true);
    currentDirectory = root;
}

FileSystem::~FileSystem() {
    delete root;
}

void FileSystem::mkdir(const std::string& name) {
    // Check for existing directory. If it already exists, throw std::runtime_error exception.
    // I wasn't sure, but I made the assumption that a file and directory in the same parent directory 
    // can't have the same name like in a real Unix system. If not, just add && curr->isDirectory == true
    for (FileSystemNode* curr : currentDirectory->children) {
        if (curr->name == name) {
            throw std::runtime_error("File already exists.");
        }
    }

    // Create the new node with the appropriate values and establish parent/child links
    FileSystemNode* newNode = new FileSystemNode(name, true);
    newNode->parent = currentDirectory;
    currentDirectory->children.push_back(newNode);
}

void FileSystem::touch(const std::string& name) {
    // Check for existing file, if it already exists, throw std::runtime_error exception
    // Again, assuming a file and directory can't have the same name, otherwise add
    // && curr->isDirectory == false
    for (FileSystemNode* curr : currentDirectory->children) {
        if (curr->name == name) {
            throw std::runtime_error("File already exists.");
        }
   }

   // Create the new node with the appropriate values and establish parent-child links
   FileSystemNode* newNode = new FileSystemNode(name, false);
   newNode->parent = currentDirectory;
   currentDirectory->children.push_back(newNode);
}

std::string FileSystem::ls() {
    // Builds a list of directories - suffixed with / - and files in the current directory 
    // using a string stream object where each element is separated by a newline
    std::stringstream ss;
    for (FileSystemNode* curr : currentDirectory->children) {
        ss << curr->name << (curr->isDirectory ? "/" : "") << "\n";
    }

    return ss.str();
}

void FileSystem::cd(const std::string& path) {
    if (path == "/") {
        currentDirectory = root;
        return;
    }
    
    FileSystemNode* current = nullptr;
    size_t index = 0;

    // Check if path is absolute or relative
    absoluteOrRelative(current, index, path[0]);
    // Navigate to the appropriate destination node specified by path
    traverse(current, index, path);

    // Ensure directory exists and isn't actually a file
    if (current == nullptr || current->isDirectory == false) {
        throw std::runtime_error("Directory not found.");
    }

    currentDirectory = current;
}

void FileSystem::rm(const std::string& name) {   
    // Iterators were used because erase() demands it and is the simplest method for removing the child node.
    // Search the current directory for the target and begin the deletion process if it exists
    for (auto target = currentDirectory->children.begin(); target != currentDirectory->children.end(); target++) {
        if ((*target)->name == name) {
            recursiveRm(*target);
            currentDirectory->children.erase(target);
            return;
        }
    }

    throw std::runtime_error("File or directory not found.");
}

// Helper function to recursively delete a target node and, in the case of a directory, its contents
void FileSystem::recursiveRm(FileSystemNode* target) {
    // Target is a file, no subdirectories to handle
    if (target->isDirectory == false) {
        delete target;
        return;
    }

    // Target is a directory, handle subdirectories
    for (FileSystemNode* curr : target->children) {
        recursiveRm(curr);
    }

    delete target;
}

std::string FileSystem::pwd() {
    // Prints the absolute path of the current directory by starting at the current node
    // and moving upwards along the tree until reaching root
    std::string workingDirectory = "";
    for (FileSystemNode* curr = currentDirectory; curr != root; curr = curr->parent) {
        workingDirectory = curr->name + "/" + workingDirectory;
    }
    // Ensures root is always included and handles the edge case where currentDirectory == root
    return "/" + workingDirectory;
}

void FileSystem::cp(const std::string& source, const std::string& destination) {
    // First, obtain a pointer to the source file or directory
    FileSystemNode* sourcePtr = nullptr;
    size_t index = 0;

    // Determine if source is an absolute or relative path
    absoluteOrRelative(sourcePtr, index, source[0]);
    // Navigate to the appropriate node specified by source
    traverse(sourcePtr, index, source);

    // Source can be a file or directory, only check that it exists
    if (sourcePtr == nullptr) {
        throw std::runtime_error("Source not found.");
    }

    // Second, obtain a pointer to the destination parent and the new name of the copied contents
    FileSystemNode* destParent = nullptr;
    std::string newName = "";
    index = 0;

    absoluteOrRelative(destParent, index, destination[0]);
    
    // The traverse() helper function wasn't used here because the logic is slightly different
    for (std::string tmpName = ""; index <= destination.size(); index++) {
        // Loop has reached the end of the string, tmpName holds the new name and destParent 
        // points to the parent node. Break before clearing tmpName or reassigning destParent again
        if (index == destination.size()) {
            newName = tmpName;
            break;
        }
        else if (destination[index] == '/') {
            if (tmpName == ".." && destParent->parent != nullptr) {
                destParent = destParent->parent;
            }
            else {
                // Begin searching for tmpName in the current directory.
                destParent = search(tmpName, destParent);
                // The loop breaks upon reaching the end of the path argument, so the final portion, which 
                // hopefully doesn't exist already, will never trigger this error 
                if (destParent == nullptr) {
                    throw std::runtime_error("Destination path not found.");
                }
            }
            tmpName.clear();
        }
        // tmpName is not fully populated, keep iterating
        else {
            tmpName += destination[index];
        }
    }

    // Check if destination already exists
    for (FileSystemNode* child : destParent->children) {
        if (child->name == newName) {
            throw std::runtime_error("Destination already exists.");
        }
    }    

    // Copy the source file or directory and add the result to the destination parent's children vector
    FileSystemNode* newNode = copyNode(sourcePtr, destParent, newName);
    destParent->children.push_back(newNode);
}

// Helper method to recursively copy a file or a directory and its contents
FileSystemNode* FileSystem::copyNode(FileSystemNode* source, FileSystemNode* destParent, const std::string& newName) {
    // Base case for files, no subdirectories to copy
    if (source->isDirectory == false) {
        FileSystemNode* newNode = new FileSystemNode(newName, false);
        newNode->parent = destParent;
        return newNode;
    }

    // Handles directories, checks for subdirectories and recursively copies them as well
    FileSystemNode* newNode = new FileSystemNode(newName, true);
    newNode->parent = destParent;
    for (FileSystemNode* child : source->children) {
        FileSystemNode* newChild = copyNode(child, newNode, child->name);
        newNode->children.push_back(newChild);
    }

    return newNode;
}

// Helper function to search the current directory
FileSystemNode* FileSystem::search(std::string target, FileSystemNode* current) {
    for (FileSystemNode* child : current->children) {
        if (child->name == target) {
            return child;
        }
    }
    return nullptr;
}

// If path starts with a '/', it must be an absolute path. Otherwise it's a relative path
void FileSystem::absoluteOrRelative(FileSystemNode*& nodePtr, size_t& index, char start) {
    if (start == '/') {
        nodePtr = root;
        index++;
    }
    else {
        nodePtr = currentDirectory;
    }
}

// Helper function to navigate the file system and return a pointer to the desired file or directory
void FileSystem::traverse(FileSystemNode*& current, size_t& index, std::string path) {
    for (std::string tmpDirec = ""; index <= path.size(); index++) {
        // tmpDirec is fully populated, begin processing the stored directory
        if (index == path.size() || path[index] == '/') {
            if (tmpDirec == ".." && current->parent != nullptr) {
                current = current->parent;
            }
            else {
                // Begin searching for tmpDirec in the current directory
                current = search(tmpDirec, current);
                // Ensure the directory exists and isn't actually a file
                if (current == nullptr) {
                    return;
                }
            }
            tmpDirec.clear();
        }
        // tmpDirec is not fully populated, keep iterating
        else {
            tmpDirec += path[index];
        }
    }
}