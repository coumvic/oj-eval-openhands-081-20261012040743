#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <queue>

using namespace std;

const int MAXN = 3005;

// Global arrays to avoid stack overflow
vector<int> graph[MAXN];
int capacity[MAXN][MAXN];
bool visited_global[MAXN];
int parent_global[MAXN];

// BFS to find augmenting path (Edmonds-Karp algorithm)
bool bfs(int s, int t, int n) {
    // Initialize visited and parent arrays
    for (int i = 1; i <= n; i++) {
        visited_global[i] = false;
        parent_global[i] = -1;
    }
    
    queue<int> q;
    q.push(s);
    visited_global[s] = true;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v = 1; v <= n; v++) {
            if (!visited_global[v] && capacity[u][v] > 0) {
                q.push(v);
                parent_global[v] = u;
                visited_global[v] = true;
            }
        }
    }
    
    return visited_global[t];
}

// Compute max flow from s to t using Edmonds-Karp algorithm
int max_flow(int s, int t, int n) {
    // Reset residual capacities
    int original_capacity[MAXN][MAXN];
    
    // Store original capacities
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            original_capacity[i][j] = capacity[i][j];
        }
    }
    
    int flow = 0;
    
    // Find augmenting paths until no more exist
    while (bfs(s, t, n)) {
        // Find minimum capacity on the path
        int path_flow = 1;  // Since all edges have capacity 1
        
        // Update residual capacities
        for (int v = t; v != s; v = parent_global[v]) {
            int u = parent_global[v];
            capacity[u][v] -= path_flow;
            capacity[v][u] += path_flow;
        }
        
        flow += path_flow;
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
