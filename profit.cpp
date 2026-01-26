
/*

PROFIT - Maximum Profit
no tags 

CS&T, the well-known cellphone company, is going to set some new service stations among n possible ones, which are numbered 1, 2 ... n. The costs of setting these stations are known as P1, P2 ... Pn. Also the company has made a survey among the cellphone users, and now they know that there are m user groups numbered 1, 2 ... m, which will communicate by service station Ai and Bi, and the company can profit Ci.

Now CS&T wants to know which service stations are to be set that the company will profit most.

Input
T [The number of tests]
n m [n<=5000 m<=50000]
P1 P2 P3 ... Pn [Pi<=100]
A1 B1 C1 
A2 B2 C2
...
Am Bm Cm [1<=Ai,Bi<=n, Ci<=100]
[other tests]
At least 80% of the tests satisfy that n<=200, m<=1000.

Output
MaximumProfit
[other tests]
Example
Input:
1
5 5
1 2 3 4 5
1 2 3
2 3 4
1 3 3
1 4 2
4 5 3

Output:
4
*/

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;


bool bfs(int s, int t, vector<vector<long long>> &capacity, vector<vector<long long>> &flow, vector<int> &parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<int> q;
    q.push(s);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v = 0; v < capacity.size(); v++) {
            if (parent[v] == -1 && capacity[u][v] - flow[u][v] > 0) {
                parent[v] = u;
                q.push(v);
                if (v == t) return true;
            }
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<long long> P(n + 1); // station costs
        for (int i = 1; i <= n; i++) cin >> P[i];

        struct Group { int a, b, c; };
        vector<Group> groups(m);
        long long TOTAL_PROFIT = 0;
        for (int i = 0; i < m; i++) {
            cin >> groups[i].a >> groups[i].b >> groups[i].c;
            TOTAL_PROFIT += groups[i].c;
        }

        // Build graph
        int S = 0;
        int groupStart = 1;
        int stationStart = groupStart + m;
        int Tnode = stationStart + n;
        int NODES = Tnode + 1;

        vector<vector<long long>> capacity(NODES, vector<long long>(NODES, 0));
        vector<vector<long long>> flow(NODES, vector<long long>(NODES, 0));
        vector<int> parent(NODES);

        // S -> group profit edges
        for (int i = 0; i < m; i++) {
            int g = groupStart + i;
            capacity[S][g] = groups[i].c;        // profit
            capacity[g][stationStart + groups[i].a - 1] = LLONG_MAX; // group -> station a
            capacity[g][stationStart + groups[i].b - 1] = LLONG_MAX; // group -> station b
        }

        // Station -> T edges (station cost)
        for (int i = 1; i <= n; i++) {
            capacity[stationStart + i - 1][Tnode] = P[i];
        }

        // Edmonds-Karp (BFS)
        long long total_flow = 0;
        while (bfs(S, Tnode, capacity, flow, parent)) {
            long long f = LLONG_MAX;
            int curr = Tnode;
            while (curr != S) {
                int prev = parent[curr];
                f = min(f, capacity[prev][curr] - flow[prev][curr]);
                curr = prev;
            }
            curr = Tnode;
            while (curr != S) {
                int prev = parent[curr];
                flow[prev][curr] += f;
                flow[curr][prev] -= f;
                curr = prev;
            }
            total_flow += f;
        }

        long long maxProfit = TOTAL_PROFIT - total_flow;
        cout << maxProfit << "\n";
    }

    return 0;
}
