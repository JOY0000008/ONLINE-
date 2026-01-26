/*
PROBLEM: PARCEL DELIVERY

There are n cities and m routes through which parcels can be carried. 
For each route, you know the maximum number of parcels and the cost of 
a single parcel.

You want to send k parcels from Syrjala (City 1) to Lehmala (City n). 
What is the cheapest way to do that?

INPUT FORMAT:
- The first input line has three integers n, m and k: the number of cities, 
  routes and parcels.
- After this, there are m lines describing the routes. Each line has four 
  integers a, b, r and c:
    a: starting city
    b: destination city
    r: maximum parcels (capacity)
    c: cost per parcel

OUTPUT FORMAT:
- Print one integer: the minimum total cost.
- If there are no solutions (cannot send k parcels), print -1.

CONSTRAINTS:
- 2 <= n <= 500
- 1 <= m <= 1000
- 1 <= k <= 100
- 1 <= a, b <= n
- 1 <= r, c <= 1000

EXAMPLE:

Input:
4 5 3
1 2 5 100
1 3 10 50
1 4 7 500
2 4 8 350
3 4 2 100

Output:
750

EXPLANATION:
- One parcel is delivered through route 1 -> 2 -> 4 (cost: 1 * 450 = 450)
- Two parcels are delivered through route 1 -> 3 -> 4 (cost: 2 * 150 = 300)
- Total minimum cost = 750

*/
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

const int N = 505;
const int INF = 1e9;

int n, m, k;
int capacity[N][N];
int flow_used[N][N];

int min_cost_max_flow(int s, int t, int need, int cost[N][N]) {
    int flow = 0, total_cost = 0;

    while (flow < need) {
        vector<int> dist(n + 1, INF), parent(n + 1, -1);
        vector<bool> inq(n + 1, false);
        queue<int> q;

        dist[s] = 0;
        q.push(s);
        inq[s] = true;

        // SPFA for min-cost path
        while (!q.empty()) {
            int u = q.front(); q.pop();
            inq[u] = false;
            for (int v = 1; v <= n; v++) {
                if (capacity[u][v] - flow_used[u][v] > 0 && dist[v] > dist[u] + cost[u][v]) {
                    dist[v] = dist[u] + cost[u][v];
                    parent[v] = u;
                    if (!inq[v]) {
                        inq[v] = true;
                        q.push(v);
                    }
                }
            }
        }

        if (dist[t] == INF) break;

        int add = need - flow;
        int v = t;
        while (v != s) {
            int u = parent[v];
            add = min(add, capacity[u][v] - flow_used[u][v]);
            v = u;
        }

        v = t;
        while (v != s) {
            int u = parent[v];
            flow_used[u][v] += add;
            flow_used[v][u] -= add; // residual reverse
            v = u;
        }

        flow += add;
        total_cost += add * dist[t];
    }

    return flow; // total flow achieved
}

// extract one path using DFS on used flow
bool extract_path(int u, int t, vector<int> &path, bool visited[N]) {
    visited[u] = true;
    path.push_back(u);
    if (u == t) return true;

    for (int v = 1; v <= n; v++) {
        if (!visited[v] && flow_used[u][v] > 0) {
            flow_used[u][v]--;
            if (extract_path(v, t, path, visited))
                return true;
            flow_used[u][v]++; // undo
        }
    }

    path.pop_back();
    visited[u] = false;
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> k;

    int cost[N][N];
    memset(capacity, 0, sizeof(capacity));
    memset(flow_used, 0, sizeof(flow_used));
    memset(cost, 0, sizeof(cost));

    for (int i = 0; i < m; i++) {
        int a, b, r, c;
        cin >> a >> b >> r >> c;
        capacity[a][b] = r;
        cost[a][b] = c;
    }

    int flow = min_cost_max_flow(1, n, k, cost);
    cout << flow << "\n";

    for (int i = 0; i < flow; i++) {
        vector<int> path;
        bool visited[N] = {false};
        extract_path(1, n, path, visited);
        cout << path.size() << "\n";
        for (int v : path) cout << v << " ";
        cout << "\n";
    }

    return 0;
}
