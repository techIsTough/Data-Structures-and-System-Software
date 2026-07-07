// FileSystem.hpp
#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>
#include <vector>
#include <sstream>

class FileSystemNode {
public:
    std::string name;
    bool isDirectory;
    std::vector<FileSystemNode*> children;
    FileSystemNode* parent;

    FileSystemNode(std::string name, bool isDir);
    ~FileSystemNode();
};

class FileSystem {
private:
    FileSystemNode* root;
    FileSystemNode* currentDirectory;

    void recursiveRm(FileSystemNode* target);
    FileSystemNode* copyNode(FileSystemNode* source, FileSystemNode* destParent, const std::string& newName);
    FileSystemNode* search(std::string target, FileSystemNode* current);
    void absoluteOrRelative(FileSystemNode*& nodePtr, size_t& index, char start);    
    void traverse(FileSystemNode*& current, size_t& index, std::string path);

public:
    FileSystem();
    ~FileSystem();

    void mkdir(const std::string& name);
    void touch(const std::string& name);
    std::string ls();
    void cd(const std::string& path);
    void rm(const std::string& name);
    std::string pwd();
    void cp(const std::string& source, const std::string& destination);
};

#endif // FILESYSTEM_HPP