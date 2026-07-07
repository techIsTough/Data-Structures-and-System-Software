#include <iostream>
#include <cassert>
#include "linked_calc.cpp" // Include the implementation file
using namespace std;

void runEvaluateExpressionTests() {
    // Test 1: Simple addition
    LinkedCalc<char> calc1;
    calc1.insert('1');
    calc1.insert('+');
    calc1.insert('2');
    assert(calc1.validateExpression());
    assert(calc1.evaluateExpression() == 3.0f);
    cout<<"Test 1 passed"<<endl;

    // Test 2: Simple subtraction
    LinkedCalc<char> calc2;
    calc2.insert('5');
    calc2.insert('-');
    calc2.insert('2');
    assert(calc2.validateExpression());
    assert(calc2.evaluateExpression() == 3.0f);
    cout<<"Test 2 passed"<<endl;

    // Test 3: Simple multiplication
    LinkedCalc<char> calc3;
    calc3.insert('3');
    calc3.insert('*');
    calc3.insert('4');
    assert(calc3.validateExpression());
    assert(calc3.evaluateExpression() == 12.0f);
    cout<<"Test 3 passed"<<endl;

    // Test 4: Simple division
    LinkedCalc<char> calc4;
    calc4.insert('8');
    calc4.insert('/');
    calc4.insert('2');
    assert(calc4.validateExpression());
    assert(calc4.evaluateExpression() == 4.0f);
    cout<<"Test 4 passed"<<endl;

    // // Test 5: Simple division 2
    LinkedCalc<char> calc5;
    calc5.insert('5');
    calc5.insert('/');
    calc5.insert('2');
    assert(calc5.validateExpression());
    assert(calc5.evaluateExpression() == 2.5f);
    cout<<"Test 5 passed"<<endl;

    // Test 6: Expression with decimal numbers
    LinkedCalc<char> calc6;
    calc6.insert('1');
    calc6.insert('.');
    calc6.insert('5');
    calc6.insert('+');
    calc6.insert('2');
    calc6.insert('.');
    calc6.insert('5');
    assert(calc6.validateExpression());
    assert(calc6.evaluateExpression() == 4.0f);
    cout<<"Test 6 passed"<<endl;

    // Test 7: Expression with decimal numbers
    LinkedCalc<char> calc7;
    calc7.insert('1');
    calc7.insert('.');
    calc7.insert('5');
    calc7.insert('+');
    calc7.insert('1');
    calc7.insert('.');
    calc7.insert('5');
    assert(calc7.validateExpression());
    assert(calc7.evaluateExpression() == 3.0f);
    cout<<"Test 7 passed"<<endl;

    // Test 8: Invalid expression (consecutive operators)
    LinkedCalc<char> calc8;
    calc8.insert('3');
    calc8.insert('+');
    calc8.insert('*');
    calc8.insert('2');
    assert(!calc8.validateExpression());
    cout<<"Test 8 passed"<<endl;
   
    // Test 9: Invalid expression (consecutive decimals)
    LinkedCalc<char> calc9;
    calc9.insert('3');
    calc9.insert('.');
    calc9.insert('.');
    calc9.insert('2');
    assert(!calc9.validateExpression());
    cout<<"Test 9 passed"<<endl;

    // Test 10: Expression ending with an operator
    LinkedCalc<char> calc10;
    calc10.insert('3');
    calc10.insert('+');
    calc10.insert('5');
    calc10.insert('*');
    assert(!calc10.validateExpression());
    cout<<"Test 10 passed"<<endl;
}

int main() {
    // Run evaluateExpression tests
    runEvaluateExpressionTests();

    // If all assertions pass
    std::cout << "All evaluateExpression tests passed!" << std::endl;

    return 0;
}