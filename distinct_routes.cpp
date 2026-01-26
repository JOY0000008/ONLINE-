/*
PROBLEM: TELEPORTERS

A game consists of n rooms and m teleporters. At the beginning of each 
day, you start in room 1 and you have to reach room n.

You can use each teleporter at most once during the game. How many 
days can you play if you choose your routes optimally?

INPUT FORMAT:
- The first input line has two integers n and m: the number of rooms 
  and teleporters.
- The rooms are numbered 1, 2, ..., n.
- After this, there are m lines describing the teleporters. Each line 
  has two integers a and b: there is a teleporter from room a to room b.
- Note: No two teleporters have the same starting and ending room.

OUTPUT FORMAT:
- First, print an integer k: the maximum number of days you can play.
- Then, print k route descriptions. 
- Each route description starts with the number of rooms in the route, 
  followed by the rooms in order.
- You can print any valid solution.

CONSTRAINTS:
- 2 <= n <= 500
- 1 <= m <= 1000
- 1 <= a, b <= n

EXAMPLE:

Input:
6 7
1 2
1 3
2 6
3 4
3 5
4 6
5 6

Output:
2
3
1 2 6
4
1 3 4 6

EXPLANATION:
You can play for 2 days. 
Day 1: Route 1 -> 2 -> 6 (uses 2 teleporters).
Day 2: Route 1 -> 3 -> 4 -> 6 (uses 3 teleporters).
Total teleporters used: 5. No teleporter is reused.

*/
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;


int n, m;
vector<vector<int>> adj;
vector<vector<int>> capacity;
vector<vector<int>> flow_used;

/* Edmonds–Karp BFS */
bool bfs(int s, int t, vector<int> &parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            // Check residual capacity instead of just capacity
            if (parent[v] == -1 && capacity[u][v] - flow_used[u][v] > 0) {
                parent[v] = u;
                if (v == t) return true;
                q.push(v);
            }
        }
    }
    return false;
}

/* Edmonds–Karp max flow */
int maxflow(int s, int t) {
    int flow = 0;
    vector<int> parent(n + 1);
    while (bfs(s, t, parent)) {
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            flow_used[prev][cur]++;//remaining capacity will be capacity[u][v]-flow[u][v] like 3/5 means flows 3 and capacity 5 remaining 5-3=2
            //we did it this way to simplify back edge. if we didnt then we had to check 0 capacity edge and back edge =1 for path extraction but now
            //we can just see if flow==1 or not if the path is used in the valid solution
            flow_used[cur][prev]--;  // Update reverse flow
            cur = prev;
        }
        flow++;
    }
    return flow;
}

/* Extract ONE simple path using used flow */
bool extract_path(int u, int t, vector<int> &path, vector<bool> &visited) {
    visited[u] = true;
    path.push_back(u);
    if (u == t) return true;
    for (int v : adj[u]) {
        if (!visited[v] && flow_used[u][v] > 0) {
            flow_used[u][v]--;
            if (extract_path(v, t, path, visited))
                return true;
            flow_used[u][v]++;//if it return false means no path possible then undo the flow 
        }
    }
    path.pop_back();
    visited[u] = false;
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    adj.assign(n + 1, {});
    capacity.assign(n + 1, vector<int>(n + 1, 0));
    flow_used.assign(n + 1, vector<int>(n + 1, 0));
    
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);//the graph actually one directional but we 
        //took bidirectional cause we considered back edge with 0 capacity
        // which actually means forward edges. taking birectional adj graph
        // help us to redue choices we make along a path
        adj[b].push_back(a);
        capacity[a][b] = 1;
    }
    
    int s = 1, t = n;
    int k = maxflow(s, t);
    cout << k << "\n";
    
    for (int i = 0; i < k; i++) {
        vector<int> path;
        vector<bool> visited(n + 1, false);
        extract_path(s, t, path, visited);
        cout << path.size() << "\n";
        for (int v : path) cout << v << " ";
        cout << "\n";
    }
    
    return 0;
}