#include "rationalbase.h"

int main() {
    try {
        Rational a, b;
        std::cout << "Enter a (numerator denominator): ";
        a.Read();
        std::cout << "Enter b (numerator denominator): ";
        b.Read();
        std::cout << "a = "; a.Display();
        std::cout << "\nb = "; b.Display();
        std::cout << "\na+b = " << a.add(b).toString();
        std::cout << "\na-b = " << a.sub(b).toString();
        std::cout << "\na*b = " << a.mul(b).toString();
        if (b.getNumerator() == 0)
            std::cout << "\na/b: division by zero";
        else
            std::cout << "\na/b = " << a.div(b).toString();
        std::cout << std::boolalpha
            << "\na==b: " << a.equal(b)
            << "\na>b: " << a.greate(b)
            << "\na<b: " << a.less(b) << '\n';
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}