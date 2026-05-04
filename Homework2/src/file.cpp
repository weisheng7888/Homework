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
