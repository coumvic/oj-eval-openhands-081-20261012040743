#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 3005;

// Global arrays
vector<int> graph[MAXN];
bool used_edges[MAXN][MAXN];  // Track used edges for current flow computation

// DFS to find a path from s to t
bool dfs(int u, int t, int parent) {
    if (u == t) return true;
    
    for (int v : graph[u]) {
        // Skip the edge we just came from (to avoid going back)
        if (v == parent) continue;
        
        // If edge is not used, try to use it
        if (!used_edges[u][v]) {
            used_edges[u][v] = used_edges[v][u] = true;
            if (dfs(v, t, u)) return true;
            // If path not found, backtrack and mark edge as unused
            used_edges[u][v] = used_edges[v][u] = false;
        }
    }
    
    return false;
}

// Compute max flow from s to t (number of edge-disjoint paths)
int max_flow(int s, int t) {
    // Reset used edges
    memset(used_edges, 0, sizeof(used_edges));
    
    int flow = 0;
    // Find edge-disjoint paths
    while (dfs(s, t, -1)) {
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
