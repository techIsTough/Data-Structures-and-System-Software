#ifndef LINKED_CALC_HPP
#define LINKED_CALC_HPP

#include <iostream>
#include <vector>

// Node structure
template <typename T>
struct Node {
    T data;
    Node* next;

    Node(const T& data) : data(data), next(nullptr) {}
};

// LinkedCalc class
template <typename T>
class LinkedCalc {
public:
    LinkedCalc();
    ~LinkedCalc();
    void insert(const T& value);
    bool validateExpression();
    float evaluateExpression();

private:
    Node<T>* head;
    bool isDigit(const T& c);
    float convertToFloat(const std::vector<char> &buffer);
    bool isOperator(const T& c);
};

#endif // LINKED_CALC_HPP 
