# 41343125 41343116
作業二

## 解題說明
本題實作了一個全方位的 圖形 (Graph) 類別，支援無向圖與有向圖，並整合了資料結構課程中核心的圖論演算法，包含基礎遍歷、最小生成樹、最短路徑及活動網路分析。

## 解題策略  
遍歷演算法：實作 DFS (遞迴) 與 BFS (佇列)，並藉此延伸出連通元件 (Connected Components) 與生成樹的提取。  
最小生成樹 (MST)：Kruskal：結合 std::sort 與 互斥集 (Disjoint Set Union) 進行邊的選取。  
Prim：使用貪心策略，逐步將鄰近最小權重頂點納入 MST。路徑演算法：  
Dijkstra：利用 priority_queue 優化，處理單源最短路徑。  
Floyd-Warshall：使用動態規劃實作全點對最短路徑。
活動網路：Topological Sort (AOV)：利用入度 (Indegree) 陣列與佇列進行排序。  
Critical Path (AOE)：計算最早發生時間 (ve) 與最晚發生時間 (vl)，找出鬆弛時間為零的關鍵路徑。   

## 程式實作

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <algorithm>
#include <iomanip>
#include <limits>

using namespace std;

struct Edge {
    int u, v, w;
    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

class Graph {
private:
    int n;
    vector<list<pair<int, int>>> adj;
    int num;
    int* dfn;
    int* low;
    stack<pair<int, int>> edgeStack;

public:
    Graph(int vertices = 0) : n(vertices) {
        adj.resize(n);
        dfn = nullptr;
        low = nullptr;
    }

    void InsertEdge(int u, int v, int w = 1, bool undirected = true) {
        if (u == v) return;
        if (max(u, v) >= n) {
            adj.resize(max(u, v) + 1);
            n = adj.size();
        }
        adj[u].push_back({ v,w });
        if (undirected) {
            adj[v].push_back({ u,w });
        }
    }

    void showMatrix() {
        for (int i = 0; i < n; i++) cout << setw(3) << i;
        cout << "\n";
        for (int i = 0; i < n; i++) {
            cout << setw(2) << i;
            for (int j = 0; j < n; j++) {
                bool found = false;
                for (auto& p : adj[i]) if (p.first == j) found = true;
                cout << setw(3) << (found ? 1 : 0);
            }
            cout << endl;
        }
    }

    void showArrayList() {
        cout << "\n[ Adjacency List ]\n";
        for (int i = 0; i < n; i++) {
            cout << i << " : ";
            for (auto& p : adj[i]) {
                cout << "(" << p.first << "," << p.second << ") ";
            }
            cout << endl;
        }
    }

    void DeleteEdge(int u, int v) {
        if (u >= n || v >= n) return;
        adj[u].remove_if([v](pair<int, int> p) { return p.first == v; });
        adj[v].remove_if([u](pair<int, int> p) { return p.first == u; });
    }

    void DeleteVertex(int v) {
        if (v < 0 || v >= n) return;
        for (int i = 0; i < n; i++) {
            adj[i].remove_if([v](pair<int, int> p) { return p.first == v; });
        }
        adj.erase(adj.begin() + v);
        n--;
        for (int i = 0; i < n; i++) {
            for (auto& p : adj[i]) {
                if (p.first > v) p.first--;
            }
        }
    }

    void DFS(int start = 0) {
        vector<bool> vis(n, false);
        DFSUtil(start, vis);
        cout << endl;
    }

    void DFSUtil(int v, vector<bool>& vis) {
        vis[v] = true;
        cout << v << " ";
        for (auto& p : adj[v]) if (!vis[p.first]) DFSUtil(p.first, vis);
    }

    void BFS(int start = 0) {
        vector<bool> vis(n, false);
        queue<int> q;
        q.push(start); vis[start] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            cout << u << " ";
            for (auto& p : adj[u]) {
                if (!vis[p.first]) {
                    vis[p.first] = true;
                    q.push(p.first);
                }
            }
        }
        cout << endl;
    }

    void Components() {
        vector<bool> vis(n, false);
        for (int i = 0; i < n; i++) {
            if (!vis[i]) {
                DFSUtil(i, vis);
                cout << endl;
            }
        }
    }

    void SpanningTreeDFS(int start = 0) {
        vector<bool> vis(n, false);
        STDFS(start, vis);
        cout << endl;
    }

    void STDFS(int v, vector<bool>& vis) {
        vis[v] = true;
        for (auto& p : adj[v]) {
            if (!vis[p.first]) {
                cout << "(" << v << "," << p.first << ") ";
                STDFS(p.first, vis);
            }
        }
    }

    void SpanningTreeBFS(int start = 0) {
        vector<bool> vis(n, false);
        queue<int> q;
        vis[start] = true; q.push(start);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto& p : adj[u]) {
                if (!vis[p.first]) {
                    vis[p.first] = true;
                    cout << "(" << u << "," << p.first << ") ";
                    q.push(p.first);
                }
            }
        }
        cout << endl;
    }

    void Biconnected() {
        num = 1;
        dfn = new int[n]();
        low = new int[n]();
        while (!edgeStack.empty()) edgeStack.pop();
        for (int i = 0; i < n; i++) {
            if (dfn[i] == 0) BiconnectedHelper(i, -1);
        }
        delete[] dfn;
        delete[] low;
    }

    void BiconnectedHelper(int u, int parent) {
        dfn[u] = low[u] = num++;
        for (auto& p : adj[u]) {
            int w = p.first;
            if (w == parent) continue;
            if (dfn[w] == 0) {
                edgeStack.push({ u, w });
                BiconnectedHelper(w, u);
                low[u] = min(low[u], low[w]);
                if (low[w] >= dfn[u]) {
                    cout << "BCC: ";
                    while (true) {
                        auto e = edgeStack.top();
                        edgeStack.pop();
                        cout << "(" << e.first << "," << e.second << ") ";
                        if (e.first == u && e.second == w) break;
                    }
                    cout << endl;
                }
            }
            else if (dfn[w] < dfn[u]) {
                edgeStack.push({ u, w });
                low[u] = min(low[u], dfn[w]);
            }
        }
    }

    int Find(vector<int>& parent, int x) {
        if (parent[x] == x) return x;
        return parent[x] = Find(parent, parent[x]);
    }

    void Union(vector<int>& parent, int a, int b) {
        a = Find(parent, a);
        b = Find(parent, b);
        if (a != b) parent[a] = b;
    }

    void Kruskal() {
        vector<Edge> edges;
        for (int i = 0; i < n; i++) {
            for (auto& p : adj[i]) {
                if (i < p.first) edges.push_back({ i,p.first,p.second });
            }
        }
        sort(edges.begin(), edges.end());
        vector<int> parent(n);
        for (int i = 0; i < n; i++) parent[i] = i;
        for (auto& e : edges) {
            if (Find(parent, e.u) != Find(parent, e.v)) {
                Union(parent, e.u, e.v);
                cout << "(" << e.u << "," << e.v << "," << e.w << ") ";
            }
        }
        cout << endl;
    }

    void Prim() {
        vector<int> key(n, INT_MAX), parent(n, -1);
        vector<bool> inMST(n, false);
        key[0] = 0;
        for (int i = 0; i < n - 1; i++) {
            int u = -1, minv = INT_MAX;
            for (int j = 0; j < n; j++) {
                if (!inMST[j] && key[j] < minv) {
                    minv = key[j]; u = j;
                }
            }
            if (u == -1) break;
            inMST[u] = true;
            for (auto& p : adj[u]) {
                int v = p.first, w = p.second;
                if (!inMST[v] && w < key[v]) {
                    key[v] = w;
                    parent[v] = u;
                }
            }
        }
        for (int i = 1; i < n; i++) {
            if (parent[i] != -1)
                cout << "(" << parent[i] << "," << i << "," << key[i] << ") ";
        }
        cout << endl;
    }

    void Dijkstra(int src = 0) {
        vector<int> dist(n, INT_MAX);
        vector<bool> visited(n, false);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        dist[src] = 0;
        pq.push({ 0, src });
        while (!pq.empty()) {
            pair<int, int> top = pq.top();
            int u = top.second;
            pq.pop();
            if (visited[u]) continue;
            visited[u] = true;
            for (auto& p : adj[u]) {
                int v = p.first;
                int w = p.second;
                if (!visited[v] && dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    pq.push({ dist[v], v });
                }
            }
        }
        for (int i = 0; i < n; i++) {
            if (dist[i] == INT_MAX) cout << "INF ";
            else cout << dist[i] << " ";
        }
        cout << endl;
    }

    void Floyd() {
        vector<vector<int>> dist(n, vector<int>(n, 1e9));
        for (int i = 0; i < n; i++) dist[i][i] = 0;
        for (int i = 0; i < n; i++) {
            for (auto& p : adj[i]) {
                dist[i][p.first] = p.second;
            }
        }
        for (int k = 0; k < n; k++)
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][j] == 1e9) cout << "INF ";
                else cout << dist[i][j] << " ";
            }
            cout << endl;
        }
    }

    void TopologicalSort() {
        vector<int> indeg(n, 0);
        for (int i = 0; i < n; i++) {
            for (auto& p : adj[i]) indeg[p.first]++;
        }
        queue<int> q;
        for (int i = 0; i < n; i++) if (indeg[i] == 0) q.push(i);
        vector<int> result;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            result.push_back(u);
            for (auto& p : adj[u]) {
                if (--indeg[p.first] == 0) q.push(p.first);
            }
        }
        if (result.size() != n) {
            cout << "Cycle detected or incomplete graph." << endl;
        }
        else {
            for (int node : result) cout << node << " ";
            cout << endl;
        }
    }

    void CriticalPath() {
        vector<int> indeg(n, 0);
        for (int i = 0; i < n; i++) {
            for (auto& p : adj[i]) indeg[p.first]++;
        }
        queue<int> q;
        vector<int> ve(n, 0);
        vector<int> topo;
        for (int i = 0; i < n; i++) if (indeg[i] == 0) q.push(i);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            topo.push_back(u);
            for (auto& p : adj[u]) {
                if (ve[p.first] < ve[u] + p.second) ve[p.first] = ve[u] + p.second;
                if (--indeg[p.first] == 0) q.push(p.first);
            }
        }
        if (topo.size() != n) return;
        vector<int> vl(n, ve[topo.back()]);
        for (int i = n - 1; i >= 0; i--) {
            int u = topo[i];
            for (auto& p : adj[u]) {
                if (vl[u] > vl[p.first] - p.second) vl[u] = vl[p.first] - p.second;
            }
        }
        for (int u = 0; u < n; u++) {
            for (auto& p : adj[u]) {
                int ee = ve[u];
                int el = vl[p.first] - p.second;
                if (ee == el) cout << u << "->" << p.first << " [" << p.second << "] ";
            }
        }
        cout << endl;
    }
};

int main() {
    Graph g(7);
    int edges[][2] = { {0,1}, {1,2}, {2,0}, {2,3}, {3,4}, {3,5}, {4,5} };
    for (auto& e : edges) g.InsertEdge(e[0], e[1], 1);
    cout << "========================================\n";
    cout << " INITIAL GRAPH\n";
    cout << "========================================\n";
    cout << "\n=== Adjacency Matrix ===\n";
    g.showMatrix();
    cout << "\n=== Adjacency List ===\n";
    g.showArrayList();
    cout << "\n=== DFS ===\n";
    g.DFS(0);
    cout << "\n=== BFS ===\n";
    g.BFS(0);
    cout << "\n=== Connected Components ===\n";
    g.Components();
    cout << "\n=== Spanning Tree (DFS) ===\n";
    g.SpanningTreeDFS(0);
    cout << "\n=== Spanning Tree (BFS) ===\n";
    g.SpanningTreeBFS(0);
    cout << "\n=== Biconnected Components ===\n";
    g.Biconnected();
    cout << "\n========================================\n";
    cout << " AFTER DELETE EDGE(0,1) & VERTEX(3)\n";
    cout << "========================================\n";
    g.DeleteEdge(0, 1);
    g.DeleteVertex(3);
    cout << "\n=== Adjacency Matrix ===\n";
    g.showMatrix();
    cout << "\n=== Connected Components ===\n";
    g.Components();
    cout << "\n========================================\n";
    cout << " WEIGHTED GRAPH ALGORITHMS\n";
    cout << "========================================\n";
    Graph g2(6);
    g2.InsertEdge(0, 1, 4);
    g2.InsertEdge(0, 2, 3);
    g2.InsertEdge(1, 2, 1);
    g2.InsertEdge(1, 3, 2);
    g2.InsertEdge(2, 3, 4);
    g2.InsertEdge(3, 4, 2);
    g2.InsertEdge(4, 5, 6);
    cout << "\n=== Adjacency Matrix ===\n";
    g2.showMatrix();
    cout << "\n=== Kruskal MST ===\n";
    g2.Kruskal();
    cout << "\n=== Prim MST ===\n";
    g2.Prim();
    cout << "\n=== Dijkstra (from 0) ===\n";
    g2.Dijkstra(0);
    cout << "\n=== Floyd ===\n";
    g2.Floyd();
    cout << "\n========================================\n";
    cout << " ACTIVITY NETWORKS (AOV & AOE)\n";
    cout << "========================================\n";
    Graph g3(6);
    g3.InsertEdge(0, 1, 3, false);
    g3.InsertEdge(0, 2, 2, false);
    g3.InsertEdge(1, 3, 2, false);
    g3.InsertEdge(1, 4, 3, false);
    g3.InsertEdge(2, 3, 4, false);
    g3.InsertEdge(3, 5, 2, false);
    g3.InsertEdge(4, 5, 4, false);
    cout << "\n=== Topological Sort (AOV) ===\n";
    g3.TopologicalSort();
    cout << "\n=== Critical Path (AOE) ===\n";
    g3.CriticalPath();
    return 0;
}
```

## 效能分析
演算法     |   時間複雜度   |   空間複雜度   |   說明   
DFS / BFS  |   O(V+E)      |   O(V)        |   頂點數 V，邊數 E。   
Kruskal    |   O(ElogE)    |   O(V+E)      |   主要耗時在邊的排序。   
Prim       |   O(V2)       |   O(V)        |   目前實作未使用優先權佇列優化，適合稠密圖。   
Dijkstra   |   O(ElogV)    |   O(V)        |   使用 priority_queue 實作。   
Floyd      |   O(V3)       |   O(V2)       |   三層迴圈遍歷所有頂點組合。   
TopologicalSort | O(V+E)   |   O(V)        |   線性時間掃描入度。   

## 測試與驗證
### 測試案例
<img width="540" height="969" alt="image" src="https://github.com/user-attachments/assets/15ea35d4-19fa-4930-a303-f8c593638e7c" />
<img width="547" height="1005" alt="image" src="https://github.com/user-attachments/assets/17d34012-9b8d-4c06-bc59-af6e6f683e41" />
<img width="469" height="204" alt="image" src="https://github.com/user-attachments/assets/ef8de474-8c6c-4f34-9a4a-7e7c0fea08ab" />

  

### 編譯與執行指令
g++ main.cpp -o GraphApp  
./GraphApp

## 申論及開發報告
記憶體管理：在 Biconnected 元件實作中，練習了動態配置 dfn 與 low 陣列，並在結束後正確釋放，避免 Memory Leak。  
AOE 網絡理解：透過實作 CriticalPath，深入理解了關鍵事件必須滿足 ee[i] == el[i] 的特性，這對於專案管理與時程安排有實務上的理解。  
刪除機制：在 DeleteVertex 中，除了移除邊，還需處理頂點索引的偏移調整，這考驗了對鄰接串列指標操作的細緻度。  

