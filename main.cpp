#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAXN = 3005;

// Graph representation
vector<int> graph[MAXN];
bool visited[MAXN];
int capacity[MAXN][MAXN];

// DFS for finding augmenting path in residual graph
bool dfs(int u, int t, int min_capacity) {
    if (u == t) return true;
    if (visited[u]) return false;
    
    visited[u] = true;
    
    for (int v : graph[u]) {
        if (!visited[v] && capacity[u][v] > 0) {
            if (dfs(v, t, min(min_capacity, capacity[u][v]))) {
                // Update residual capacities
                capacity[u][v] -= min_capacity;
                capacity[v][u] += min_capacity;
                return true;
            }
        }
    }
    
    return false;
}

// Compute max flow from s to t using Ford-Fulkerson
int max_flow(int s, int t, int n) {
    // Reset residual capacities
    int flow = 0;
    int original_capacity[MAXN][MAXN];
    
    // Store original capacities
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            original_capacity[i][j] = capacity[i][j];
        }
    }
    
    // Find augmenting paths until no more exist
    while (true) {
        memset(visited, 0, sizeof(visited));
        if (!dfs(s, t, 1e9)) break;
        flow++;
    }
    
    // Restore original capacities
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            capacity[i][j] = original_capacity[i][j];
        }
    }
    
    return flow;
}

int main() {
    int n, m;
    cin >> n >> m;
    
    // Initialize capacity matrix
    memset(capacity, 0, sizeof(capacity));
    
    // Read edges and build graph
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
        capacity[a][b] = 1;
        capacity[b][a] = 1;
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
