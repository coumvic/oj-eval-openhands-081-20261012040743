#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 3005;

// Global arrays
vector<int> graph[MAXN];
bool visited[MAXN];

// DFS to find a path from s to t in the residual graph
bool dfs(int u, int t, vector<vector<int>>& residual, vector<int>& path) {
    if (u == t) return true;
    
    visited[u] = true;
    
    for (int v : graph[u]) {
        if (!visited[v] && residual[u][v] > 0) {
            path[v] = u;
            if (dfs(v, t, residual, path)) return true;
        }
    }
    
    return false;
}

// Compute max flow from s to t using Ford-Fulkerson
int max_flow(int s, int t, int n) {
    // Create residual graph
    vector<vector<int>> residual(n+1, vector<int>(n+1, 0));
    for (int u = 1; u <= n; u++) {
        for (int v : graph[u]) {
            residual[u][v] = 1;  // All edges have capacity 1
        }
    }
    
    vector<int> path(n+1, -1);
    int flow = 0;
    
    // Find augmenting paths
    while (true) {
        memset(visited, 0, sizeof(visited));
        path.assign(n+1, -1);
        
        if (!dfs(s, t, residual, path)) break;
        
        // Found a path, update residual capacities
        int v = t;
        while (v != s) {
            int u = path[v];
            residual[u][v] -= 1;
            residual[v][u] += 1;
            v = u;
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
            total_flow += max_flow(a, b, n);
        }
    }
    
    cout << total_flow << endl;
    
    return 0;
}
