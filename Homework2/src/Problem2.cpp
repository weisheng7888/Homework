#include <iostream>
#include <cmath> 
#include <algorithm> 
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream& os, const Polynomial& poly);
private:
    float coef;
    int exp;
};

class Polynomial {
private:
    Term* termArray;
    int capacity;
    int terms;

    void expandCapacity() {
        capacity *= 2;
        Term* temp = new Term[capacity];
        for (int i = 0; i < terms; i++)
            temp[i] = termArray[i];
        delete[] termArray;
        termArray = temp;
    }

public:
    Polynomial() : capacity(2), terms(0) {
        termArray = new Term[capacity];
    }

    ~Polynomial() {
        delete[] termArray;
    }

    Polynomial(const Polynomial& other) : capacity(other.capacity), terms(other.terms) {
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++) {
            termArray[i] = other.termArray[i];
        }
    }

    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            delete[] termArray;
            capacity = other.capacity;
            terms = other.terms;
            termArray = new Term[capacity];
            for (int i = 0; i < terms; i++) {
                termArray[i] = other.termArray[i];
            }
        }
        return *this;
    }

    void newTerm(const float coef, const int exp) {
        if (coef == 0) return;
        if (terms == capacity) expandCapacity();
        termArray[terms].coef = coef;
        termArray[terms].exp = exp;
        terms++;
    }

    Polynomial Add(const Polynomial& b) const {
        Polynomial c;
        int aPos = 0, bPos = 0;
        while (aPos < terms && bPos < b.terms) {
            if (termArray[aPos].exp == b.termArray[bPos].exp) {
                float sum = termArray[aPos].coef + b.termArray[bPos].coef;
                if (sum != 0)
                    c.newTerm(sum, termArray[aPos].exp);
                aPos++; bPos++;
            }
            else if (termArray[aPos].exp > b.termArray[bPos].exp) {
                c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            }
            else {
                c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
                bPos++;
            }
        }
        while (aPos < terms)
            c.newTerm(termArray[aPos].coef, termArray[aPos++].exp);
        while (bPos < b.terms)
            c.newTerm(b.termArray[bPos].coef, b.termArray[bPos++].exp);
        return c;
    }

    Polynomial Mult(const Polynomial& b) const {
        Polynomial c;
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < b.terms; j++) {
                float newCoef = termArray[i].coef * b.termArray[j].coef;
                int newExp = termArray[i].exp + b.termArray[j].exp;

                bool found = false;
                for (int k = 0; k < c.terms; k++) {
                    if (c.termArray[k].exp == newExp) {
                        c.termArray[k].coef += newCoef;
                        found = true;
                        break;
                    }
                }
                if (!found)
                    c.newTerm(newCoef, newExp);
            }
        }
        return c;
    }

    float Eval(float x) const {
        float result = 0;
        for (int i = 0; i < terms; i++) {
            result += termArray[i].coef * pow(x, termArray[i].exp);
        }
        return result;
    }

    friend istream& operator>>(istream& is, Polynomial& poly) {
        delete[] poly.termArray;
        poly.capacity = 2;
        poly.terms = 0;
        poly.termArray = new Term[poly.capacity];

        int n;
        cout << "Enter number of terms: ";
        if (!(is >> n)) return is;

        for (int i = 0; i < n; i++) {
            float c; int e;
            cout << "Enter coefficient and exponent: ";
            if (!(is >> c >> e)) {
                cout << "Invalid input. Enter two numbers: ";
                is.clear();
                is.ignore(1000, '\n');
                i--;
                continue;
            }
            poly.newTerm(c, e);
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& poly) {
        if (poly.terms == 0) {
            os << "0";
            return os;
        }
        for (int i = 0; i < poly.terms; i++) {
            os << poly.termArray[i].coef << "x^" << poly.termArray[i].exp;
            if (i != poly.terms - 1) os << " + ";
        }
        return os;
    }
};

int main() {
    Polynomial p1, p2, sum, product;
    float x;

    cout << "Enter first polynomial:\n";
    cin >> p1;
    cout << "Enter second polynomial:\n";
    cin >> p2;

    cout << "\nP1(x) = " << p1 << endl;
    cout << "P2(x) = " << p2 << endl;

    sum = p1.Add(p2);
    cout << "\nP1(x) + P2(x) = " << sum << endl;

    product = p1.Mult(p2);
    cout << "P1(x) * P2(x) = " << product << endl;

    cout << "\nEnter a value of x to evaluate P1: ";
    cin >> x;
    cout << "P1(" << x << ") = " << p1.Eval(x) << endl;

    return 0;
}
