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
