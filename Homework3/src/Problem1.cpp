#include <iostream>
#include <vector>
using namespace std;

template <class T> class Chain;
template <class T> class ChainIterator;
template <class T> class AvailableList;

template <class T>
class ChainNode {
    friend class Chain<T>;
    friend class ChainIterator<T>;
    friend class AvailableList<T>;
private:
    T element;
    ChainNode<T>* next;
public:
    ChainNode() : next(nullptr) {}
    ChainNode(const T& elem) : element(elem), next(nullptr) {}
    ChainNode(const T& elem, ChainNode<T>* nextNode) : element(elem), next(nextNode) {}

    void setElement(const T& elem) { element = elem; }
    void setNext(ChainNode<T>* nextNode) { next = nextNode; }
};

template <class T>
class AvailableList {
private:
    static ChainNode<T>* avHead; 
public:
    static ChainNode<T>* getNode() {
        if (!avHead) return nullptr;
        ChainNode<T>* nodeToReturn = avHead;
        avHead = avHead->next;
        nodeToReturn->next = nullptr;
        return nodeToReturn;
    }
    static void getBack(ChainNode<T>* node) {
        if (!node) return;
        ChainNode<T>* temp = node;
        while (temp->next) temp = temp->next;
        temp->next = avHead;
        avHead = node;
    }
};
template <class T> ChainNode<T>* AvailableList<T>::avHead = nullptr;

template <class T>
class ChainIterator {
private:
    ChainNode<T>* current;
public:
    ChainIterator(ChainNode<T>* node = nullptr) : current(node) {}
    T& operator*() const { return current->element; }
    T* operator->() const { return &current->element; }
    ChainIterator& operator++() { current = current->next; return *this; }
    bool operator!=(const ChainIterator& right) const { return current != right.current; }
};

template <class T>
class Chain {
    friend class ChainIterator<T>;
    friend class Polynomial;
private:
    ChainNode<T>* head;
public:
    Chain() : head(nullptr) {}
    ~Chain() { 
        ChainNode<T>* current = head;
        while (current != nullptr) {
            ChainNode<T>* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
    }

    ChainNode<T>* release() { // [cite: 44]
        ChainNode<T>* oldHead = head;
        head = nullptr;
        return oldHead;
    }

    ChainIterator<T> begin() const { return ChainIterator<T>(head); }
    ChainIterator<T> end() const { return ChainIterator<T>(nullptr); }

    void insert(int idx, const T& element) { // [cite: 47-48]
        ChainNode<T>* newNode = AvailableList<T>::getNode();
        if (!newNode) newNode = new ChainNode<T>(element);
        else newNode->setElement(element);

        if (idx == 0) {
            newNode->next = head;
            head = newNode;
        } else {
            ChainNode<T>* prev = head;
            for (int i = 0; i < idx - 1 && prev != nullptr; ++i)
                prev = prev->next;
            if (prev != nullptr) {
                newNode->next = prev->next;
                prev->next = newNode;
            } else {
                AvailableList<T>::getBack(newNode);
            }
        }
    }
};

struct Term {
    double coef;
    int exp;
    Term(double c = 0, int e = 0) : coef(c), exp(e) {}
};

class Polynomial {
    friend ostream& operator<<(ostream& os, const Polynomial& poly);
    friend istream& operator>>(istream& is, Polynomial& poly);
private:
    Chain<Term> terms;
public:
    Polynomial() {}
    
    Polynomial(const Polynomial& other) {
        int index = 0;
        for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it) {
            terms.insert(index++, *it);
        }
    }

    ~Polynomial() {
        AvailableList<Term>::getBack(terms.release());
    }

    ChainIterator<Term> begin() const { return terms.begin(); }
    ChainIterator<Term> end() const { return terms.end(); }

    void newTerm(double coef, int exp) {
        if (coef == 0) return;
        int index = 0;
        for (ChainIterator<Term> it = terms.begin(); it != terms.end(); ++it, ++index) {
            if (it->exp < exp) break;
            if (it->exp == exp) {
                it->coef += coef;
                return;
            }
        }
        terms.insert(index, Term(coef, exp));
    }

    Polynomial operator+(const Polynomial& other) const {
        Polynomial result;
        ChainIterator<Term> it1 = this->begin();
        ChainIterator<Term> it2 = other.begin();
        while (it1 != this->end() && it2 != other.end()) {
            if (it1->exp > it2->exp) {
                result.newTerm(it1->coef, it1->exp);
                ++it1;
            } else if (it1->exp < it2->exp) {
                result.newTerm(it2->coef, it2->exp);
                ++it2;
            } else {
                double newCoef = it1->coef + it2->coef;
                if (newCoef != 0) result.newTerm(newCoef, it1->exp);
                ++it1; ++it2;
            }
        }
        while (it1 != this->end()) { result.newTerm(it1->coef, it1->exp); ++it1; }
        while (it2 != other.end()) { result.newTerm(it2->coef, it2->exp); ++it2; }
        return result;
    }

    Polynomial operator-(const Polynomial& other) const {
        Polynomial negOther;
        for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it) {
            negOther.newTerm(-(it->coef), it->exp);
        }
        return (*this) + negOther;
    }

    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;
        for (ChainIterator<Term> it1 = this->begin(); it1 != this->end(); ++it1) {
            for (ChainIterator<Term> it2 = other.begin(); it2 != other.end(); ++it2) {
                result.newTerm(it1->coef * it2->coef, it1->exp + it2->exp);
            }
        }
        return result;
    }
};

ostream& operator<<(ostream& os, const Polynomial& poly) {
    bool first = true;
    for (auto it = poly.begin(); it != poly.end(); ++it) {
        if (!first && it->coef > 0) os << " + ";
        os << it->coef << "x^" << it->exp;
        first = false;
    }
    if (first) os << "0";
    return os;
}

int main() {
    Polynomial p1, p2;
    p1.newTerm(3, 2); // 3x^2
    p1.newTerm(5, 1); // 5x
    p2.newTerm(2, 2); // 2x^2
    p2.newTerm(4, 0); // 4

    cout << "P1: " << p1 << endl;
    cout << "P2: " << p2 << endl;
    cout << "P1 + P2: " << (p1 + p2) << endl;
    cout << "P1 * P2: " << (p1 * p2) << endl;
    return 0;
}
