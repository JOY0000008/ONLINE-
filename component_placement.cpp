//problem ---> https://lightoj.com/problem/component-placement

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

const long long INF = 1e15;

bool bfs(int s, int t, vector<vector<long long>> &cap, vector<vector<long long>> &flow, vector<int> &parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v = 0; v < cap.size(); v++) {
            if (parent[v] == -1 && cap[u][v] - flow[u][v] > 0) {
                parent[v] = u;
                q.push(v);
                if (v == t) return true;
            }
        }
    }
    return false;
}

long long maxFlow(int s, int t, vector<vector<long long>> &cap) {
    int n = cap.size();
    vector<vector<long long>> flow(n, vector<long long>(n, 0));
    vector<int> parent(n);
    long long totalFlow = 0;

    while (bfs(s, t, cap, flow, parent)) {
        long long f = INF;
        int curr = t;
        while (curr != s) {
            int prev = parent[curr];
            f = min(f, cap[prev][curr] - flow[prev][curr]);
            curr = prev;
        }
        curr = t;
        while (curr != s) {
            int prev = parent[curr];
            flow[prev][curr] += f;
            flow[curr][prev] -= f;
            curr = prev;
        }
        totalFlow += f;
    }
    return totalFlow;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    for (int caseNum = 1; caseNum <= T; caseNum++) {
        int N, M;
        cin >> N >> M;

        vector<long long> topCost(N), bottomCost(N);
        for (int i = 0; i < N; i++) cin >> topCost[i];
        for (int i = 0; i < N; i++) cin >> bottomCost[i];

        vector<int> forced(N);
        for (int i = 0; i < N; i++) cin >> forced[i];

        int totalNodes = N + 2;
        int S = 0, Tt = N + 1;
        vector<vector<long long>> cap(totalNodes, vector<long long>(totalNodes, 0));

        for (int i = 0; i < N; i++) {
            if (forced[i] == 1) { // force top
                cap[S][i + 1] = topCost[i];
                cap[i + 1][Tt] = INF;
            } else if (forced[i] == -1) { // force bottom
                cap[S][i + 1] = INF;
                cap[i + 1][Tt] = bottomCost[i];
            } else { // normal
                cap[S][i + 1] = topCost[i];
                cap[i + 1][Tt] = bottomCost[i];
            }
        }

        for (int i = 0; i < M; i++) {
            int u, v;
            long long r;
            cin >> u >> v >> r;
            u--; v--; // 0-based
            cap[u + 1][v + 1] += r;
            cap[v + 1][u + 1] += r;
        }

        long long ans = maxFlow(S, Tt, cap);
        cout << "Case " << caseNum << ": " << ans << "\n";
    }
}
