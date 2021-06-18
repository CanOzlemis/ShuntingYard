#include <iostream>
#include <vector>
#include <string>


int priority(char c) {
    switch (c) {
    case '+':
    case '-':
        return 1;
        break;
    case '*':
    case '/':
        return 2;
        break;
    case '^':
        return 3;
        break;
    default:
        return 0;
        break;
    }

}

int isNum(char c) {
    int num = static_cast<int>(c) - '0';
    switch (num) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        return 1;
        break;
    default:
        return 0;
        break;
    }
}

int chartoNumber(const char& c) { return static_cast<int>(c) - '0'; }
char numbertoChar(const int& n) { return static_cast<char>(n) + '0'; }
template <class T>
T popBackandDelete(std::vector<T> &v) {
    T temp = v.back();
    v.pop_back();
    return temp;
}

int main() {


    std::vector<char> operatorStack;
    std::vector<char> output;
    std::string input;


    std::getline(std::cin, input);
    for (auto it = begin(input); it != end(input); it++) {
        if (isNum(*it))      {   output.push_back(*it); }                       //Push the numbers to output vector
        else if (*it == '(') {   operatorStack.push_back(*it); } 
        else if (*it == ')') {
            while (!operatorStack.empty() && !(operatorStack.back() == '(')) {
                output.push_back(popBackandDelete(operatorStack));

            }
            operatorStack.pop_back();                                           //Pop the openning paranthesis
        }
        else if (priority(*it)) {                                               //Using priority to check if it is an operator

            if (!operatorStack.empty()) {                                       //If the stack is empty
                if (operatorStack.back() == '^' && *it == '^');                 //If it is power of power, ignore while check
                else {
                    while (!(operatorStack.empty()) && priority(operatorStack.back()) >= priority(*it)) {
                        output.push_back(popBackandDelete(operatorStack));
                    }
                }
                
            }
            operatorStack.push_back(*it);
        }
    }

    if (!operatorStack.empty()) {                                               //Empty the operator stack to output
        while(operatorStack.size()) {
            output.push_back(popBackandDelete(operatorStack));
        }
    }

    for (auto i = begin(output); i != end(output); i++) {                       //Display the postfix
        std::cout << *i;
    }



    //---------------------------------------POSTFIX END----------------------------------------


    int number1, number2;
    std::vector<int> result;

    for (int i = 0; i < output.size(); i++) {                                   //Calculation area
        if (priority(output[i])) {                                              //Using the priority to see if it is an operator
            number2 = popBackandDelete(result);
            number1 = popBackandDelete(result);

            switch (output[i]) {
            case '+':
                result.push_back(number1 + number2);
                break;
            case '-':
                result.push_back(number1 - number2);
                break;
            case '*':
                result.push_back(number1 * number2);
                break;
            case '/':
                result.push_back(number1 / number2);
                break;
            case '^':
                if((i + 1 < output.size()) && (output[i + 1] == '^'))           //If it is x ^2 ^2 , multiply the 2's(Short-circuit the AND operator to see if index is out of bound)
                    result.push_back(number1 * number2);
                else
                    result.push_back(pow(number1,number2));
                break;
            }
        }
        else {
            result.push_back(chartoNumber(output[i]));
        }
    }

    std::cout << std::endl;
    for (auto it = begin(result); it != end(result); it++)                      //Display the result
        std::cout << *it;
}