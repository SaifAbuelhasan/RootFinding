#include <iostream>
#include <cmath>
#include "calculate.cpp"
using namespace std;


// double f(double x) {
//     // Define the function f(x) here
//     return x*x -4;
// }

// Return the number to the nearest d decimal places.
double roundOff(double x, int d) {
    x = round(x*pow(10, d));
    x *= pow(10, -d);
    return x;
}

double bisection(double a, double b, int decimalPlaces, string func) {
    double f_a = calc(func, a);
    double f_b = calc(func, b);
    if (f_a * f_b > 0) {
        // print invalid arguments if both f_a and f_b are on the same side of x-axis
        cout << "Invalid arguments" << endl;
        return 0;
    } 
    double m = (a + b) / 2;
    double f_m = calc(func, m);
    // Base case
    if (f_m == 0 || (b - a) < pow(10, -decimalPlaces)) {
        // If a and b are almost equal (difference less than precision), return the midpoint "m". To avoid infinite recursion.
        return roundOff(m, decimalPlaces);
    }
    // Call bisection method recursively on one half of the range. The one with the intersection with x-axis.
    if (f_a * f_m < 0) {
        return bisection(a, m, decimalPlaces, func);
    } else {
        return bisection(m, b, decimalPlaces, func);
    }
}

double getSecant(double a, double b, double f_a, double f_b) {
    return roundOff(b - ((a - b) / (f_a - f_b)) * f_b, 10);
}

double secant(double a, double b, int decimalPlaces, string func) {
    double f_a = calc(func, a);
    double f_b = calc(func, b);
    if (f_a * f_b > 0) {
        // print invalid arguments if both f_a and f_b are on the same side of x-axis
        cout << "Invalid arguments" << endl;
        return 0;
    } 
    double m = getSecant(a, b, f_a, f_b);
    double f_m = calc(func, m);
    // cout << "m = " << m << "f_m = " << f_m << endl;

    // Base case
    if (f_m <= pow(10, -decimalPlaces)) {
        return roundOff(m, decimalPlaces);
    }

    if (f_a * f_m < 0) {
        return secant(a, m, decimalPlaces, func);
    } else if (f_b * f_m < 0) {
        return secant(m, b, decimalPlaces, func);
    } else {
        cout << "No solution" << endl;
        return 0;
    }
}

int getPositiveBoundary(string func) {
    for (int i = 0; i < 10; i++) {
        if (i == 0 && calc(func, i) == 0) {
            continue;
        }
        if (calc(func, i) * calc(func, i+1) <= 0) {
            return i;
        }
    }
    return 10;
}

int getNegativeBoundary(string func) {
    for (int i = -9; i <= 0; i++) {
        cout << "func " << func << " " << calc(func, i) << endl; 
        if (i == 0 && calc(func, i) == 0) {
            continue;
        }
        if (calc(func, i) * calc(func, i+1) <= 0) {
            return i;
        }
    }
    return 10;
}

int main() {
    int method;
    double decimalPlaces;
    double a, b;
    string func;
    cout << "Choose the method:" << endl;
    cout << "1- Bisection Mehtod" << endl;
    cout << "2- Secant Mehtod" << endl;
    cin >> method;
    cin.ignore();
    cout << "Enter the function: ";
    getline(cin, func);
    cout << "function " << func << endl;

    // With range
  
    cout << "The two boundaries should x1, x2 should have values f(x1), f(x2) with different signs for the function to operate" << endl;
    cout << "Enter the two boundaries (seprated by one space): ";
    cin >> a;
    cin >> b;
    cout << a << " " << b << endl;
    cout << "Enter the accuracy with which you want the answer: ";
    cin >> decimalPlaces; 

    // Without range
/* 
    int root;
    cout << "Choose root: " << endl;
    cout << "1. Positive Root" << endl;
    cout << "2. Negative Root" << endl;
    cin >> root;
    if (root == 1) {
        a = getPositiveBoundary(func);
        b = a + 1;
    } else {
        a = getNegativeBoundary(func);
        b = a + 1;
    }
    if (a > 9) {
        cout << "No Solution" << endl;
        return 0;
    }
*/
    if (method == 2) {
        cout << secant(a, b, decimalPlaces, func) << endl;
    } else {
        cout << bisection(a, b, decimalPlaces, func) << endl;
    }

    // cout << bisection(0, 5, 6, "e^x-x-3") << endl;
    // cout << secant(-8, -6.2, 6, "3-x*sin(x)") << endl;
    return 0;
}