/*

    CAN ÖZLEMÝÞ - 170997

*/

#include <iostream>

#include <iomanip> //for setprecision

#include <map>

#include <math.h>

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
    int num = static_cast <int> (c) - '0';
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

template < class T >
T popBackandDelete(std::vector < T >& v) {
    T temp = v.back();
    v.pop_back();
    return temp;
}

void variables(std::string& tempName, std::string& s, std::string::iterator& it, std::map < std::string, std::string >& m) {
    while (!isNum(*it) && !priority(*it)) { //While it is a letter
        if (*it == ' ' || *it == '=') //we know we are at the end of variable
            break;
        tempName.push_back(*it); //push the letter ( combined it will be a whole name)
        it++;
    }
    while (!isNum(*it)) //Push the pointer until to the number
        ++it;

    std::string tempNumber; //Temp to hold our multi digit numbers
    tempNumber.push_back(*it);
    while (it + 1 != end(s) && (isNum(*(it + 1)) || *(it + 1) == '.')) { //Repeat until next character is not a number or end of the vector
        tempNumber.push_back(*(++it));

    }

    m[tempName] = (tempNumber); //Place the variable with value
    std::cout << tempNumber << std::endl;
}

void fixString(std::string& input, std::map < std::string, std::string > ::iterator& mapIt, int ind) {
    //example : input = " 5 + 8 + var - 7"  . 
    //map["var"] = "7"
    std::string temp;
    temp = input.substr(0, ind); //temp =  5 + 8 + 
    ind = ind + mapIt -> first.size();
    temp += mapIt -> second; //temp =  5 + 8 + 7
    std::string inputTemp = input.substr(ind); //inputTemp =  - 7

    temp += inputTemp; //temp =  5 + 8 + 7 - 7

    input = temp;

}

bool isFunction(std::string& input, std::string::iterator tempIt) { //Check to see if it a function
    while (tempIt != end(input)) {

        if (!isNum(*tempIt) && !priority(*tempIt)) { //If it is not a number or an operator

            if (*tempIt == '(') {
                return true;
                break;
            }

        }
        else {
            break;
        }
        tempIt++;
    }
    return false;
}

void getNumberFromString(std::string::iterator& tempIt, std::string& input, std::vector < std::string >& output, std::string::iterator& it) {
    std::string temp; //Temp to hold our multi digit numbers
    temp.push_back(*tempIt);

    while (tempIt + 1 != end(input) && (isNum(*(tempIt + 1)) || *(tempIt + 1) == '.')) { //Repeat until next character is not a number or end of the vector
        temp.push_back(*(++tempIt));
    }

    output.push_back(temp); //Push our place holder to output stack
    temp.empty();
    it = tempIt;
}

int main() {

    std::vector < std::string > operatorStack;
    std::vector < std::string > output;
    std::string input;

    std::map < std::string, std::string > map;

    while (true) {

        output.clear();
        input.clear();
        map.clear();
        operatorStack.clear();
        map["pi"] = "3.141593";
        map["e"] = "2.718282";

        std::getline(std::cin, input);
        bool changed = false;

        //Fix the input for the shunting yard. This part does the work for things like x = 5    and after that  8 + x . So this part will convert it to 8 + 5
        do {
            changed = false;

            for (auto it = begin(input); it != end(input); it++) {

                for (auto mapIt = begin(map); mapIt != end(map); mapIt++) { //Search the input the replace them with their values

                    int ind = input.find(mapIt -> first);
                    if (ind != -1) {
                        fixString(input, mapIt, ind);
                        changed = true;
                        break;
                    }

                }
                if (changed == true)
                    break;

                if (*it == ' ');
                else if (!isNum(*it) && !priority(*it) && *it != '(' && *it != ')' && *it != '.') { //checking for new variable names to add on the map
                    std::string temp;
                    auto tempIt = it;
                    bool assignment = false;

                    while (tempIt != end(input)) { //Go through the entire string to see if there is = to determine that this is a variable
                        if (*tempIt == '=') {
                            assignment = true;
                            break;
                        }
                        tempIt++;
                    }

                    if (assignment) { //It is an assignment , take a new input
                        variables(temp, input, it, map);

                        std::getline(std::cin, input);
                        changed = true;
                        break;
                    }

                }
            }
        } while (changed);

        //------------------------------------------------------------SHUNTING YARD -------------------------------------------------

        int ind = -1;
        for (auto it = begin(input); it != end(input); it++) {
            ++ind;
            if (*it == ' '); //Skip white spaces
            else if (*it == ',') { //If we reach ',' we know that we need to first complete these calculations, then move on

                while (!operatorStack.empty() && !(operatorStack.back() == "(")) { //Act like ',' is an closing paranthesis but don't pop the openning paranthesis
                    output.push_back(popBackandDelete(operatorStack));
                }

            }
            else if (isNum(*it)) { //Push the numbers to output vector
                auto tempIt = it;
                getNumberFromString(tempIt, input, output, it);

            }
            else if (*it == '(') {
                std::string convertOperator; //Need to convert the char to a string so it can be stored in  vector containing strings
                convertOperator.push_back(*it); //The reason for this that vector.push_back(std::to_string(*it))  MAKES THE OPERATOR INTO AN ASCII VALUE ????
                operatorStack.push_back(convertOperator);
            }
            else if (*it == ')') {

                while (!operatorStack.empty() && !(operatorStack.back() == "(")) {
                    output.push_back(popBackandDelete(operatorStack));
                }

                operatorStack.pop_back(); //Pop the openning paranthesis
                if ((!operatorStack.empty()) && (operatorStack.back() == "cos" || operatorStack.back() == "sin" || operatorStack.back() == "abs" || operatorStack.back() == "pow")) { //If before the paranthesis is a function, do the calculation
                    output.push_back(popBackandDelete(operatorStack));
                }
            }
            else if (isFunction(input, it)) {
                std::string tempName;

                while (!isNum(*it) && !priority(*it)) { //While it is a letter
                    if (*it == ' ' || *it == '(') //we know we are at the end of function name
                        break;
                    tempName.push_back(*it); //push the letter ( combined it will be a whole name)
                    it++;
                }
                --it;
                operatorStack.push_back(tempName);
            }
            else if (priority(*it)) { //Using priority to check if it is an operator

                auto tempIt = it;
                if ((tempIt == begin(input)) || (tempIt != begin(input) && (priority(*(tempIt - 1)) || (*(tempIt - 1) == '(')))) { //SHORT-CIRCUIT. Deal with understanding if this is operand or negative number, ex =  +- , -+, *-, /-, ^-, --
                    getNumberFromString(tempIt, input, output, it);
                }
                else {
                    if (!operatorStack.empty()) { //If the stack is empty
                        if (operatorStack.back() == "^" && *it == '^'); //If it is power of power, ignore while check
                        else {

                            while (!(operatorStack.empty()) && priority(operatorStack.back()[0]) >= priority(*it)) {
                                output.push_back(popBackandDelete(operatorStack));

                            }
                        }
                    }

                    std::string convertOperator; //Need to convert the char to a string so it can be stored in  vector containing strings
                    convertOperator.push_back(*it); //The reason for this that vector.push_back(std::to_string(*it))  MAKES THE OPERATOR INTO AN ASCII VALUE ????
                    operatorStack.push_back(convertOperator);
                }

            }
        }

        if (!operatorStack.empty()) { //Empty the operator stack to output

            while (operatorStack.size()) {
                output.push_back(popBackandDelete(operatorStack));
            }
        }

        //---------------------------------------POSTFIX END----------------------------------------

        double number1, number2;
        std::string check;
        bool isNumber = false;
        std::vector < double > result;
        result.clear();

        for (int i = 0; i < output.size(); i++) { //Calculation area

            check += (output[i]); //Check will be used to determine the difference between -2 and - operator
            if (check.size() > 0) { 
                if (isNum(check[1])) { //The string is more than 1 charecter. It is either a function or number
                    isNumber = true;
                }
            }
            else {
                if (isNum(check[0]))
                    isNumber = true;
            }

            if (!isNumber && priority(check[0])) { //Using the priority to see if it is an operator , just by checking first charecter we can decide if its a number or operator

                number2 = popBackandDelete(result);
                number1 = popBackandDelete(result);

                if (output[i] == "+") {
                    result.push_back(number1 + number2);
                }
                else if (output[i] == "-") {
                    result.push_back(number1 - number2);
                }
                else if (output[i] == "*") {
                    result.push_back(number1 * number2);
                }
                else if (output[i] == "/") {
                    result.push_back(number1 / number2);
                }
                else if (output[i] == "^") {
                    if ((i + 1 < output.size()) && (output[i + 1][0] == '^')) //If it is x ^2 ^2 , multiply the 2's(Short-circuit the AND operator to see if index is out of bound)
                        result.push_back(number1 * number2);
                    else
                        result.push_back(pow(number1, number2));
                }

            }
            else if (!isNumber && !isNum(output[i][0])) { //if it is not a number, it could be function
                number1 = popBackandDelete(result);

                if (output[i] == "cos") {
                    result.push_back(cos(number1 * stod(map["pi"]) / 180));
                }
                else if (output[i] == "sin") {
                    result.push_back(sin(number1 * stod(map["pi"]) / 180));
                }
                else if (output[i] == "abs") {
                    result.push_back(abs(number1));
                }
                else if (output[i] == "pow") {
                    number2 = popBackandDelete(result);
                    result.push_back(pow(number2, number1));
                }
            }
            else {
                result.push_back(std::stod(output[i]));
            }
            check.clear();
            isNumber = false;
        }

        std::cout << std::fixed;
        std::cout << std::setprecision(5);
        for (auto it = begin(result); it != end(result); it++) //Display the result
            std::cout << *it;
        std::cout << std::endl;
    }
    return 0;
}