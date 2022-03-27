#include <iostream>
#include <cmath>
#include <string>
#include <regex>
using namespace std;

double calc(string func, double x);
void split(string str, string* parts, char* signs);
string solveBrackets(string str, double x);

double mul(double x, double y) {
    return x*y;
}

double div1(double x, double y) {
    return x/y;
}

unordered_map<char, double (*)(double, double)> operations = {{'*', mul}, {'/', div1}, {'^', pow}};

unordered_map<string, double (*)(double)> trigFunctions = {{"sin", sin}, {"cos", cos}, {"tan", tan}};

void split(string str, string* parts, char* signs) {
    int currIndex = 0, i = 0;  
    int startIndex = 0, endIndex = 0;
    int numSigns = 0;
    string possibleSigns = "+-*/^";
    while (i <= str.length())  
    {  
        if ((possibleSigns.find(str[i]) < possibleSigns.length()) || i == str.length())  
        { 

            if ((i-1) >= 0 && (str[i] == '-') && (possibleSigns.find(str[i-1]) < possibleSigns.length())) {
                if (str[i-1] == '-') str[i-1] = '+';
                if (str[i-1] == '+') str[i-1] = '-';
                i++;
                continue;
            }
            // Check if sign comes after a sin, cos or tan
            if ((i-1) >= 0 && (str[i-1] == 'n' || str[i-1] == 's') && str[i] == '-') {
                i++;
                continue;
            }
            if (i != str.length()) {
                signs[currIndex] = str[i]; 
                numSigns++;
            } 
            endIndex = i;  
            string subStr = "";  
            subStr.append(str, startIndex, endIndex - startIndex);  
            parts[currIndex] = subStr;  
            currIndex += 1;  
            startIndex = endIndex + 1;  
        }  
        i++;  
    } 
    signs[9] = numSigns;
}

void solveBrackets(string* str, double x) {
    int i = 0;
    string copy = *str;
    int numBrackets = 0;
    string newStr = "";
    string subStr = "";
    while (i < copy.length()) {
        if (copy[i] == '(') {
            numBrackets++;
            if (numBrackets == 1)
                i++;
        }
        if (copy[i] == ')') {
            numBrackets--;
            if (numBrackets == 0) {
                newStr.append(to_string(calc(subStr, x)));
                subStr = "";
                i++;
                continue;
            }
        }
        if (numBrackets > 0) {
            subStr.push_back(copy[i]);
        }
        if (numBrackets == 0) {
            newStr.push_back(copy[i]);
        }
        i++;
    }
    *str = newStr;
}

double solveTrigTerm(string term) {
    int angleStartIndex;
    double angle;
    string f = "";
    for (int i = 0; i < term.length(); i++) {
        // Check for letters
        if (term[i] > '9') {
            f.push_back(term[i]);
        } else {
            angleStartIndex = i;
            break;
        }
    }
    angle = stod(term.substr(angleStartIndex, term.length()-1));
    return (trigFunctions.find(f))->second(angle);
}

void solve(string* parts, char* signs, double x, string lookup) {
    string* newParts = new string[10];
    char* newSigns = new char[10];
    int counter = 0;
    for (int i = 0; i < (int)signs[9]; i++) {
        if (lookup.find(signs[i]) < lookup.length()) {
            double product = (operations.find(signs[i]))->second(calc(parts[i], x), calc(parts[i+1], x));
            parts[i+1] = to_string(product);
            newParts[counter] = parts[i+1];
        } else {
            newParts[counter] = parts[i];
            newSigns[counter] = signs[i];
            counter += 1;
        }
        if ((i+1) == (int)signs[9]) {
            newParts[counter] = parts[i+1];
            copy(newParts, newParts+counter+1, parts);
            copy(newSigns, newSigns+counter, signs);
            signs[9] = counter;
        }
    }
}

void solveExp(string* parts, char* signs, double x) {
    solve(parts, signs, x, "^");
}

void SolveMulDiv(string* parts, char* signs, double x) {
    solve(parts, signs, x, "*/");
}

void fix(string* func) {
    string copy = *func;
    string numbers = "x0123456789)e";
    string numSigns = "0123456789+-*/^).";
    string out = "";
    for (int i = 0; i < copy.length(); i++) {
        out.push_back(copy[i]);
        if ((i+1) < copy.length() && numbers.find(copy[i]) < numbers.length() && numSigns.find(copy[i+1]) > numSigns.length()) {
            out.push_back('*');
        }
    }
    *func = out;
}

double calc(string func, double x) {
    // Add '*' for multiplication
    fix(&func);
    // Change the function string to a string with solved brackets.
    solveBrackets(&func, x);
    // Insert '+' if first term isn't negative. Always have a leading sign.
    if (func[0] != '-') func.insert(0, 1, '+');
    // Split function into two arrays. An array for signs, and an array for terms
    string* terms = new string[10];
    char* signs = new char[10];
    split(func, terms, signs);
    solveExp(terms, signs, x);
    // Solve all multiplication and division signs.
    SolveMulDiv(terms, signs, x);
    double result = 0;
    for (int i = 0; i < (int)signs[9]; i++) {
        // Check if term includes x, solve for x and replace it in the array.
        if (terms[i+1] == "x") {
            terms[i+1] = to_string(x);
        }
        // Check if term includes e.
        if (terms[i+1] == "e") {
            terms[i+1] = to_string(2.718281);
        }
        // Check if term includes trig func, solve and replace. 
        if (terms[i+1].find("sin") < terms[i+1].length() || terms[i+1].find("cos") < terms[i+1].length() || terms[i+1].find("tan") < terms[i+1].length()) {
            terms[i+1] = to_string(solveTrigTerm(terms[i+1])); 
        } // Term can't possibly include an x and a trig func, since all brackets are solved
        switch (signs[i])
        {
        case '+':
            result += stod(terms[i+1]);
            break;
        case '-':
            result -= stod(terms[i+1]);
            break;
        }
    }
    return result;
}

// int main() {
//     // cout << "calc: " << calc("3*-2", 2) << endl;
//     // cout << "calc: " << calc("-x^2*3*-2", 2) << endl;
//     // cout << "calc: " << calc("3/x^2", -2) << endl;
//     // cout << "calc: " << calc("3*sin(-x)^2", -2) << endl;
//     // cout << "calc: " << calc("3*x^2-sin(x^2)", 2) << endl;
//     // cout << "calc: " << calc("3-(3*x)*-sin(x)", -2) << endl;
//     // cout << "calc: " << calc("xtan(x^2/3)", 1) << endl; 
//     // cout << "calc: " << calc("(x^2-1)(tan(x))", 0.631) << endl; 
//     // cout << "calc: " << calc("(x^2-1)(tan(3x))", 0.631) << endl; 
//     // cout << "calc: " << calc("3^(2x)", 3) << endl; 
//     // cout << "calc: " << calc("2xe", 2) << endl;
//     // cout << "calc: " << calc("2e^x", 2) << endl;
//     // cout << "calc: " << calc("2-x", 2) << endl;
    
// }