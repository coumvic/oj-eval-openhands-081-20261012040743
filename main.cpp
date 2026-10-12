#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <stack>

using namespace std;

const int MAXN = 3005;

// Global arrays to avoid stack overflow
vector<int> graph[MAXN];
int capacity[MAXN][MAXN];

// Iterative DFS using stack to find augmenting path
bool dfs_iterative(int s, int t, int n) {
    vector<int> parent(n+1, -1);
    vector<bool> visited(n+1, false);
    stack<int> st;
    
    st.push(s);
    visited[s] = true;
    
    while (!st.empty()) {
        int u = st.top();
        st.pop();
        
        if (u == t) break;
        
        for (int v : graph[u]) {
            if (!visited[v] && capacity[u][v] > 0) {
                visited[v] = true;
                parent[v] = u;
                st.push(v);
            }
        }
    }
    
    // If we didn't reach the target, no augmenting path exists
    if (!visited[t]) return false;
    
    // Reconstruct path and find minimum capacity
    int min_capacity = 1;  // Since all edges have capacity 1
    int u = t;
    while (parent[u] != -1) {
        min_capacity = min(min_capacity, capacity[parent[u]][u]);
        u = parent[u];
    }
    
    // Update residual capacities along the path
    u = t;
    while (parent[u] != -1) {
        capacity[parent[u]][u] -= min_capacity;
        capacity[u][parent[u]] += min_capacity;
        u = parent[u];
    }
    
    return true;
}

// Compute max flow from s to t using Ford-Fulkerson with iterative DFS
int max_flow(int s, int t, int n) {
    // Reset residual capacities
    int original_capacity[MAXN][MAXN];
    
    // Store original capacities
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            original_capacity[i][j] = capacity[i][j];
        }
    }
    
    // Find augmenting paths until no more exist
    int flow = 0;
    while (dfs_iterative(s, t, n)) {
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
