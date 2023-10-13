#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <cctype>

// Function to perform arithmetic operations
double applyOp(char op, double b, double a) {
    switch (op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b == 0) {
            std::cout << "Runtime Error: Division by zero." << std::endl;
            exit(3);
        }
        return a / b;
    }
    return 0;
}

// Function to evaluate arithmetic expressions
double evaluateExpression(const std::string& expression) {
    std::istringstream iss(expression);
    std::stack<double> values;
    std::stack<char> ops;

    char current;
    while (iss >> current) {
        if (isdigit(current) || current == '.') {
            iss.putback(current);
            double value;
            iss >> value;
            values.push(value);
        }
        else if (current == '(') {
            ops.push(current);
        }
        else if (current == ')') {
            while (ops.top() != '(') {
                double val2 = values.top();
                values.pop();

                double val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(op, val2, val1));
            }
            ops.pop();
        }
        else if (current == '+' || current == '-' || current == '*' || current == '/') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top();
                values.pop();

                double val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(op, val2, val1));
            }
            ops.push(current);
        }
    }

    while (!ops.empty()) {
        double val2 = values.top();
        values.pop();

        double val1 = values.top();
        values.pop();

        char op = ops.top();
        ops.pop();

        values.push(applyOp(op, val2, val1));
    }

    return values.top();
}

int main() {
    std::string input;
    getline(std::cin, input);
    double result = evaluateExpression(input);
    std::cout << result << std::endl;

    return 0;
}