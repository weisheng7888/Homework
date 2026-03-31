# 41343125
作業一

### Problem 1
## 解題說明
本題實作一個單純的「最小堆積 (Min Heap) 類別」，直接定義所有必要的操作方法，不使用抽象類別繼承。  

## 解題策略  
**容量管理**：當 `heapSize` 達到 `capacity` 時，呼叫 `ChangeSize1D` 進行空間倍增，這點與多項式類別的設計邏輯相同。  
**上浮機制**：新元素插入末端後，若值小於父節點則與之交換。  
**下沉機制**：移除根節點後，將最後一項移到根部，並不斷與子節點中較小者交換。  

## 程式實作

```cpp
#include <iostream>
#include <algorithm>

using namespace std;

template <class T>
class MinHeap {
private:
    T* heap;
    int capacity;
    int heapSize;

    void ChangeSize1D(int newSize) {
        T* temp = new T[newSize];
        for (int i = 1; i <= heapSize; i++) {
            temp[i] = heap[i];
        }
        delete[] heap;
        heap = temp;
        capacity = newSize - 1;
    }

public:
    MinHeap(int cap = 10) : capacity(cap), heapSize(0) {
        heap = new T[capacity + 1];
    }

    ~MinHeap() { 
        delete[] heap; 
    }

    bool IsEmpty() const { 
        return heapSize == 0; 
    }

    const T& Top() const {
        if (IsEmpty()) throw "Heap is empty.";
        return heap[1];
    }

    void Push(const T& x) {
        if (heapSize == capacity) {
            ChangeSize1D(2 * capacity + 1);
        }
        
        int currentNode = ++heapSize;
        while (currentNode != 1 && heap[currentNode / 2] > x) {
            heap[currentNode] = heap[currentNode / 2];
            currentNode /= 2;
        }
        heap[currentNode] = x;
    }

    void Pop() {
        if (IsEmpty()) throw "Heap is empty.";
        
        T lastElt = heap[heapSize--];
        int currentNode = 1;
        int child = 2; 

        while (child <= heapSize) {
            if (child < heapSize && heap[child] > heap[child + 1]) child++;
            if (lastElt <= heap[child]) break;

            heap[currentNode] = heap[child];
            currentNode = child;
            child *= 2;
        }
        heap[currentNode] = lastElt;
    }
};
```

## 效能分析
Push():  
時間複雜度：程式的時間複雜度為 O(log n)  
空間複雜度：空間複雜度為 O(n)  
Pop():  
時間複雜度：程式的時間複雜度為 O(log n)  
空間複雜度：空間複雜度為 O(1)  
Top():  
時間複雜度：程式的時間複雜度為 O(1)  
空間複雜度：空間複雜度為 O(1)  

## 測試與驗證

### 測試案例
  

### 編譯與執行指令
g++ main.cpp -o MinHeapApp
./MinHeapApp

## 申論及開發報告
本次實作專注於「單一類別設計」，簡化了物件導向的層級，讓邏輯更加直接。 

### Problem 2
## 解題說明
本題要求實作二元搜尋樹 (BST) 並進行實驗分析與刪除功能實作   

## 解題策略  
**樹高測量**：使用遞迴方式定義樹高：$H(node) = 1 + \max(H(left), H(right))$。  
**隨機插入**：使用 `rand()` 或 `<random>` 生成均勻分佈的隨機數，建立樣本數 $n$ 從 100 到 10,000 的樹。  
**刪除邏輯**：當刪除節點有兩個子節點時，本實作選擇找「右子樹的最小值 (Successor)」來替代該節點，隨後再刪除該最小值節點。  

## 程式實作

```cpp
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>

using namespace std;

template <class K, class E>
struct Node {
    K key;
    E element;
    Node<K, E> *left, *right;
    Node(K k, E e) : key(k), element(e), left(nullptr), right(nullptr) {}
};

template <class K, class E>
class BST {
private:
    Node<K, E>* root;

    int getHeight(Node<K, E>* node) {
        if (!node) return 0;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }

public:
    BST() : root(nullptr) {}

    void Insert(const K& k, const E& e) {
        Node<K, E>** curr = &root;
        while (*curr) {
            if (k < (*curr)->key) curr = &((*curr)->left);
            else if (k > (*curr)->key) curr = &((*curr)->right);
            else return;
        }
        *curr = new Node<K, E>(k, e);
    }

    void Delete(const K& k) {
        Node<K, E> **curr = &root;
        while (*curr && (*curr)->key != k) {
            if (k < (*curr)->key) curr = &((*curr)->left);
            else curr = &((*curr)->right);
        }
        if (!*curr) return;

        Node<K, E>* old = *curr;
        if (!(*curr)->left) *curr = (*curr)->right;
        else if (!(*curr)->right) *curr = (*curr)->left;
        else {
            Node<K, E>** succ = &((*curr)->right);
            while ((*succ)->left) succ = &((*succ)->left);
            old->key = (*succ)->key;
            old->element = (*succ)->element;
            old = *succ;
            *succ = (*succ)->right;
        }
        delete old;
    }

    int Height() { return getHeight(root); }
};
```

## 效能分析
時間複雜度：$O(h)$。在隨機平衡的情況下為 $O(\log n)$，在最壞情況（斜傾樹）下為 $O(n)$。  
空間複雜度：$O(1)$。若使用迭代尋找位置，僅需常數級別的輔助指標。  

## 測試與驗證
| n (節點數) | Height (實測樹高) | log2 n (理論對數) | Ratio (h / log2 n) |
| :--------- | :---------------- | :---------------- | :------------------ |
| 100        | 12                | 6.64              | 1.81                |
| 500        | 18                | 8.97              | 2.01                |
| 1000       | 21                | 9.97              | 2.11                |
| 2000       | 24                | 10.97             | 2.19                |
| 3000       | 26                | 11.55             | 2.25                |
| 5000       | 28                | 12.29             | 2.28                |
| 10000      | 31                | 13.29             | 2.33                |

### 測試案例
  

### 編譯與執行指令
g++ main.cpp -o BSTApp 
./BSTApp

## 申論及開發報告
動態樹高測量：透過遞迴精確計算每棵生成的隨機樹。  
高效刪除：實作了完整的三種刪除情境，確保樹在刪除節點後依然符合 BST 的搜尋規則。  
