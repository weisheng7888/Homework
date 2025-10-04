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
