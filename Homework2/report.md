# 41343125

作業二

### Problem 1
## 解題說明
本題要求實作一個「多項式 (Polynomial) 類別」  
加法 (Add)、乘法 (Mult)、代入求值 (Eval)  

## 解題策略
1. 整個多項式以一個「動態陣列」來存所有項目  
2. Add() : 使用類似合併排序 (merge) 的概念。  
3. Mult() : 對每一個 A 的項，都去乘上 B 的每一項。
4. Eval() : 利用 pow(x, exp) 計算每項的值再加總。

## 程式實作

以下為主要代碼:
```cpp
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
```

## 效能分析
Add() :  
時間複雜度：程式的時間複雜度為 O(m + n)  
空間複雜度：空間複雜度為 O(m + n)  
Mult() :  
時間複雜度：程式的時間複雜度為 O(m + n)  
空間複雜度：空間複雜度為 O(m + n)  
Eval() :  
時間複雜度：程式的時間複雜度為 O(n)  
空間複雜度：空間複雜度為 O(1)  

## 測試與驗證

### 測試案例
在Problem 2中  

### 編譯與執行指令
在Problem 2中  

## 申論及開發報告
此作業讓我學習到如何封裝資料與操作方法，若未考慮容量擴充或指數重複的情況，程式容易出現記憶體錯誤或重複項。  
### 採用類別 (Class) 的方式設計兩個主要結構  
1. Term 類別 : 用於儲存每一項的係數（coef）與指數（exp）。  
2. Polynomial 類別 : 當容量不足時，透過 newTerm() 進行自動擴充。  
   此設計可讓多項式大小彈性調整，節省記憶體並維持運算效率。
### 主要功能
1. newTerm() : 此函式負責建立新項。若係數為 0 則忽略，若容量不足則自動擴充。  
2. Add() : 透過兩個指標同步掃描，若遇到相同指數則將係數相加；  
   否則取指數較大的項加入結果多項式。
3. Mult() : 採用雙層迴圈，逐項相乘。  
   若乘積後出現相同次方，則需進行係數合併。  
   
### Problem 2
## 解題說明
本題要寫出  
istream& operator>>(istream&, Polynomial&)  
ostream& operator<<(ostream&, const Polynomial&)  

## 解題策略
1. 輸入 >> : 先輸入多項式的項數，依序輸入每項係數與指數。  
2. 輸出 << : 依序列印每項，格式：coef x^exp，中間加上 +（除了最後一項）。  

## 程式實作

以下為主要代碼:
```cpp
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
```

## 效能分析
operator>> : 
時間複雜度：程式的時間複雜度為 O(n)  
空間複雜度：空間複雜度為 O(n)  
operator<< : 
時間複雜度：程式的時間複雜度為 O(n)  
空間複雜度：空間複雜度為 O(1)  
   
## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 (n) | 輸入參數 元素 | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|----------|
| 測試一   | 0            |          | ()       | ()
| 測試二   | 1            | a        | () (a)   | () (a)
| 測試三   | 2            | a b      | () (a) (b) (a,b)   | () (a) (b) (a,b)
| 測試四   | 3            | a b c    | () (a) (b) (c) (a,b) (c) (a,c) (b,c) (a,b,c)    | () (a) (b) (c) (a,b) (c) (a,c) (b,c) (a,b,c)

### 編譯與執行指令
```cpp
C:\Users\weish\datastructure>g++ -std=c++17 -o Problem2 Problem2.cpp

C:\Users\weish\datastructure>Problem2
請輸入集合大小 n: 3
請輸入 3 個元素:
a b c
Power set:
()
(c)
(b)
(b,c)
(a)
(a,c)
(a,b)
(a,b,c)
```

## 申論及開發報告
### 選擇遞迴的原因
1. 遞迴最適合處理「二分岔」的情況  
   在每一步，都有兩種（或幾種）可能的選擇，每一種選擇都會再產生下一層的選擇。  
2. 直覺清楚，每次只考慮「要不要選這個元素」  
   在產生冪集（Power Set）這類問題時，每一層遞迴都只做一件事：決定當前這個元素：要不要選。

