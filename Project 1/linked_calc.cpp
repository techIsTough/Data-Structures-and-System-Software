#include "linked_calc.hpp"

// Default constructor definition
template <typename T>
LinkedCalc<T>::LinkedCalc() : head(nullptr) {}

// Destructor to deallocate memory 
template <typename T>
LinkedCalc<T>::~LinkedCalc() {
    Node<T>* current = head;
    while (current != nullptr) {
        Node<T> *tmp = current->next;
        delete current;
        current = tmp;
    }
}

// Function to insert a new node at the end of the linked list
template <typename T>
void LinkedCalc<T>::insert(const T& value) {
    Node<T>* newNode = new Node<T>(value);
    if (head == nullptr) {
        head = newNode;
    } else {
        Node<T>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Helper function to determine if a character is a digit
template <typename T>
bool LinkedCalc<T>::isDigit(const T& c) {
    return (c >= '0' && c <= '9') ;
}

// Helper function to determine if a character is an operator
template <typename T>
bool LinkedCalc<T>::isOperator(const T& c) {
    switch (c) {
        case '+':
            return true;
        case '-':
            return true;
        case '*':
            return true;
        case '/':
            return true;
        default:
            return false;
    }
}

// Helper function to convert a vector<char> to a float. Variables 'integer_start' and 'i' are declared as size_t since size() 
// returns size_t, seemingly an unsigned long long in this case. This is to silence compilation warnings
template <typename T>
float LinkedCalc<T>::convertToFloat(const std::vector<char> &buffer) {
    float final_number = 0.0;           // The final result converted from a vector of chars to a float
    size_t integer_start = 0;           // Index of the ones place in the integer portion 
    int integer_place = 1;              // Controls the integer place (ones place, tens place, etc.) of buffer data
    float fractional_place = 0.1;       // Controls the fractional place (tenths place, hundredths place, etc.) of buffer data
    bool decimal_point = false;         // Flag for presence of fractional portion 

    // Find the start of the integer portion (ones place), stop at the decimal point if present and note it
    while (integer_start < buffer.size()) {
        if (buffer[integer_start] == '.') {
            decimal_point = true;
            break;
        }
        integer_start++;
    }

    // Initialize the integer portion of the number starting at the ones place, iterating left.
    // Each number is given the proper magnitude based on its place in the original number by  
    // multiplying by integer_place
    for (int i = integer_start - 1; i >= 0; i--) {
        final_number += (buffer[i] - '0') * integer_place;   // convert char to int using ASCII values
        integer_place *= 10;    // 1 --> 10 --> 100 --> 1000 --> etc.
    }

    // Initialize the fractional portion of the number if it exists. Follows a similar approach as above
    // but divide by 10 to find each number's magnitude 
    if (decimal_point == true) {
        for (size_t i = integer_start + 1; i < buffer.size(); i++) {
            final_number += (buffer[i] - '0') * fractional_place;
            fractional_place /= 10;     // 0.1 --> 0.01 --> 0.001 --> etc.
        }
    }
    return final_number;
}

template <typename T>
bool LinkedCalc<T>::validateExpression() {
    Node<T> *current = head;
    char previous = '\0';           // Stores the previously seen character
    bool decimal_point = false;     // Used to ensure numbers do not contain multiple decimal points

    if (current == nullptr) {
        std::cout << "No expression given." << std::endl;
        return false;
    }

    while (current != nullptr) {
        // This ensures the current character is a number, decimal point, or operator 
        if (isDigit(current->data) == false && isOperator(current->data) == false && current->data != '.') {
            return false;
        }
        // This catches consecutive operators, consecutive decimal points, and operators or decimal points not preceded AND succeeded by numbers
        // If the checks succeed, a new number has been reached and decimal_point is set back to false
        if (isOperator(current->data) == true) {
            if (current->next == nullptr || isDigit(previous) == false || isDigit(current->next->data) == false) {
                return false;
            }
            decimal_point = false;
        }
        // This catches multiple decimal points in a single number
        if (current->data == '.') {
            if (decimal_point == true) {
                return false;
            }
            decimal_point = true;
        }

        previous = current->data;
        current = current->next;
    }

    return true;    // The function returns true if nothing illegal was found
}

template <typename T>
float LinkedCalc<T>::evaluateExpression() {
    float total = 0.0;                  // Holds the result of the expression
    float arg1 = 0.0, arg2 = 0.0;       // Each holds one argument of the expression
    char oper = '\0';                   // The operator used on the arguments
    std::vector<char> buffer;           // Used to read in arguments before converting to floats
    Node<T>* current = head;            // Used to iterate over linked list and read node data

    // Loop to read over linked list and collect the first argument
    while (current != nullptr && (isDigit(current->data) == true || current->data == '.')) {
        buffer.push_back(current->data);
        current = current->next;
    }

    // Convert buffer to a float then flush it
    arg1 = convertToFloat(buffer);
    buffer.clear();

    // Assuming the expression is not just 1 argument, current is moved to the next node, operator is recorded
    // It is assumed the expression has run through validateExpression() and is well-formed
    if (current != nullptr) {
        oper = current->data;
        current = current->next;

        // Loop to read the rest of the linked list and collect the second argument
        while (current != nullptr && (isDigit(current->data) == true || current->data == '.')) {
            buffer.push_back(current->data);
            current = current->next;
        }

        arg2 = convertToFloat(buffer);
        buffer.clear();
    }

    // Total is calculated based on recorded operator. If the expression is just 1 argument, that argument is returned
    switch (oper) {
        case '+':
            total = arg1 + arg2;
            break;
        case '-':
            total = arg1 - arg2;
            break;
        case '*':
            total = arg1 * arg2;
            break;
        case '/':
            total = arg1 / arg2;
            break;
        default:
            total = arg1;
            break;
    }

    return total;
}

