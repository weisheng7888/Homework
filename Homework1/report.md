# 41343125

作業一

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
using namespace std;

long long ackermann(long long m, long long n) {
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return ackermann(m - 1, 1);
    else
        return ackermann(m - 1, ackermann(m, n - 1));
}

int main() {
    long long m, n;
    cout << "Enter m n: ";
    cin >> m >> n;
    cout << "Ackermann(" << m << "," << n << ") = "
        << ackermann(m, n) << endl;
    return 0;
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
本題要求求一個集合 S 的所有子集

## 解題策略
1. 使用遞迴函式
2. 當 idx == n 時，表示所有元素都處理完了，輸出當前的子集。

## 程式實作

以下為主要代碼:
```cpp
#include <iostream>
#include <string>
using namespace std;

void PowerSet(string S[], int n, int idx, string current) {
    if (idx == n) {
        cout << "(" << current << ")" << endl;
        return;
    }

    // 不選擇 S[idx]
    PowerSet(S, n, idx + 1, current);

    // 選擇 S[idx]
    string next = current;
    if (!next.empty()) next += ",";
    next += S[idx];
    PowerSet(S, n, idx + 1, next);
}

int main() {
    int n;
    cout << "請輸入集合大小 n: ";
    cin >> n;

    string S[100];  // 假設最大 100 個元素
    cout << "請輸入 " << n << " 個元素:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> S[i];
    }

    cout << "Power set:" << endl;
    PowerSet(S, n, 0, "");
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
