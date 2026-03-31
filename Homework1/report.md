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

