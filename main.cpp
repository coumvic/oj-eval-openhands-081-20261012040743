#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <queue>

using namespace std;

const int MAXN = 3005;

// Global arrays
vector<int> graph[MAXN];
bool used_edges[MAXN][MAXN];  // Track used edges for current flow computation
int parent[MAXN];  // For BFS

// BFS to find a path from s to t
bool bfs(int s, int t) {
    memset(parent, -1, sizeof(parent));
    queue<int> q;
    q.push(s);
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v : graph[u]) {
            if (parent[v] == -1 && v != s && !used_edges[u][v]) {
                parent[v] = u;
                if (v == t) return true;
                q.push(v);
            }
        }
    }
    
    return false;
}

// Compute max flow from s to t (number of edge-disjoint paths)
int max_flow(int s, int t) {
    // Reset used edges
    memset(used_edges, 0, sizeof(used_edges));
    
    int flow = 0;
    // Find edge-disjoint paths using BFS
    while (bfs(s, t)) {
        // Found a path, mark edges as used
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            used_edges[u][v] = used_edges[v][u] = true;
        }
        flow++;
    }
    
    return flow;
}

int main() {
    int n, m;
    cin >> n >> m;
    
    // Read edges and build graph
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    
    // Calculate sum of max flows for all pairs (a,b) where a < b
    long long total_flow = 0;
    for (int a = 1; a <= n; a++) {
        for (int b = a + 1; b <= n; b++) {
            total_flow += max_flow(a, b);
        }
    }
    
    cout << total_flow << endl;
    
    return 0;
}
