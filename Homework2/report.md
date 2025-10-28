# 41343125

作業二

### Problem 1
## 解題說明
本題要求將Ackermann函數轉寫成C++程式

## 解題策略
1. 使用遞迴函式
2. 當 m=0 時,返回 n + 1 作為遞迴的結束條件。
3. 主程式呼叫遞迴函式，並輸出計算結果。

## 程式實作

以下為主要代碼:
```cpp
#include <iostream>
#include <cmath>  
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
private:
    float coef; // 係數
    int exp;    // 指數
};

class Polynomial {
private:
    Term* termArray;
    int capacity;
    int terms;
public:
    Polynomial() : capacity(2), terms(0) {
        termArray = new Term[capacity];
    }
    ~Polynomial() {
        delete[] termArray;
    }

    void newTerm(const float coef, const int exp);
    Polynomial Add(const Polynomial& b);
    Polynomial Mult(const Polynomial& b);
    float Eval(float x);
};

void Polynomial::newTerm(const float coef, const int exp) {
    if (coef == 0) return;
    if (terms == capacity) {
        capacity *= 2;
        Term* temp = new Term[capacity];
        copy(termArray, termArray + terms, temp);
        delete[] termArray;
        termArray = temp;
    }
    termArray[terms].coef = coef;
    termArray[terms++].exp = exp;
}

Polynomial Polynomial::Add(const Polynomial& b) {
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

// 多項式乘法
Polynomial Polynomial::Mult(const Polynomial& b) {
    Polynomial c;

    for (int i = 0; i < terms; i++) {
        for (int j = 0; j < b.terms; j++) {
            float newCoef = termArray[i].coef * b.termArray[j].coef;
            int newExp = termArray[i].exp + b.termArray[j].exp;

            // 檢查 c 是否已經有相同次方
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

// 代入 x 計算多項式值
float Polynomial::Eval(float x) {
    float result = 0;
    for (int i = 0; i < terms; i++) {
        result += termArray[i].coef * pow(x, termArray[i].exp);
    }
    return result;
}
```

## 效能分析
1. 時間複雜度：程式的時間複雜度為 Ω(A(m,n))(極度龐大)
2. 空間複雜度：空間複雜度為 O(A(m,n))(導致堆疊溢位)

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 (m,n) | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | (0,0)        | 1        | 1        |
| 測試二   | (0,1)        | 2        | 2        |
| 測試三   | (1,0)        | 2        | 2        |
| 測試四   | (1,1)        | 3        | 3        |
| 測試五   | (4,2)        | Stack overflow | Stack overflow |

### 編譯與執行指令
```cpp
C:\Users\weish\datastructure>g++ -std=c++17 -o Problem1 Problem1.cpp
C:\Users\weish\datastructure>Problem1
Enter m n: 1 0
Ackermann(1,0) = 2
```

## 申論及開發報告
### 選擇遞迴的原因
1. 遞迴版程式非常短，幾行就能直接對應數學公式  
   因為每一步都要先呼叫前一步的結果，每個呼叫又要再呼叫更裡面一層，用遞迴會更好處理
2. 更好的除錯與控制流程  
   遞迴的結構清晰，邏輯貼近問題本身，幾乎是照著數學定義翻譯，可讀性與邏輯清晰度非常高
3. 遞迴容易造成 Stack Overflow，重複計算、效率低  
   改善 : 可以用記憶化遞迴，減少重複計算
   
### Problem 2
## 解題說明
本題要寫出  
istream& operator>>(istream&, Polynomial&)  
ostream& operator<<(ostream&, const Polynomial&)  

## 解題策略
1. 使用遞迴函式
2. 當 idx == n 時，表示所有元素都處理完了，輸出當前的子集。

## 程式實作

以下為主要代碼:
```cpp
#include <iostream>
#include <cmath>   // for pow()
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
private:
    float coef; // 係數
    int exp;    // 指數
};

class Polynomial {
private:
    Term *termArray;
    int capacity;
    int terms;  // 實際項數
public:
    Polynomial() : capacity(2), terms(0) {
        termArray = new Term[capacity];
    }
    ~Polynomial() {
        delete[] termArray;
    }

    void newTerm(const float coef, const int exp) {
        if (coef == 0) return;
        if (terms == capacity) {
            capacity *= 2;
            Term *temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
        }
        termArray[terms].coef = coef;
        termArray[terms++].exp = exp;
    }

    // 加法
    Polynomial Add(const Polynomial &b) const {
        Polynomial c;
        int aPos = 0, bPos = 0;
        while (aPos < terms && bPos < b.terms) {
            if (termArray[aPos].exp == b.termArray[bPos].exp) {
                float sum = termArray[aPos].coef + b.termArray[bPos].coef;
                if (sum != 0)
                    c.newTerm(sum, termArray[aPos].exp);
                aPos++; bPos++;
            } else if (termArray[aPos].exp > b.termArray[bPos].exp) {
                c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            } else {
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

    // 乘法
    Polynomial Mult(const Polynomial &b) const {
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

    // 代入 x 計算值
    float Eval(float x) const {
        float result = 0;
        for (int i = 0; i < terms; i++) {
            result += termArray[i].coef * pow(x, termArray[i].exp);
        }
        return result;
    }

    // 多項式輸入
    friend istream& operator>>(istream& is, Polynomial& poly) {
        int n;
        cout << "Enter number of terms: ";
        is >> n;
        for (int i = 0; i < n; i++) {
            float c;
            int e;
            cout << "Enter coefficient and exponent: ";
            is >> c >> e;
            poly.newTerm(c, e);
        }
        return is;
    }

    // 多項式輸出
    friend ostream& operator<<(ostream& os, const Polynomial& poly) {
        for (int i = 0; i < poly.terms; i++) {
            os << poly.termArray[i].coef << "x^" << poly.termArray[i].exp;
            if (i != poly.terms - 1)
                os << " + ";
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
1. 時間複雜度：程式的時間複雜度為 O(n⋅2n)
2. 空間複雜度：空間複雜度為 O(n⋅2n)
   
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

