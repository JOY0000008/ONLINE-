//problem link ---> https://lightoj.com/problem/aladdin-and-the-grand-feast


#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

static const long long INF = 1e18;

bool bfs(int s, int t, vector<vector<long long>>& cap,
         vector<vector<long long>>& flow, vector<int>& parent)
{
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<int> q;
    q.push(s);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v = 0; v < cap.size(); v++) {
            if (parent[v] == -1 && cap[u][v] - flow[u][v] > 0) {
                parent[v] = u;
                if (v == t) return true;
                q.push(v);
            }
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin >> T;
    for (int tc = 1; tc <= T; tc++) {
        int n, tables, chairs, e;
        cin >> n >> tables >> chairs >> e;

        vector<int> a(n), d(n), f(n);
        for (int i = 0; i < n; i++)
            cin >> a[i] >> d[i] >> f[i];

        int timeSlots = e - 1;
        int S = 0;
        int personStart = 1;
        int timeStart = personStart + n;
        int D = timeStart + timeSlots;
        int N = D + 1;

        vector<vector<long long>> cap(N, vector<long long>(N, 0));
        vector<vector<long long>> flow(N, vector<long long>(N, 0));
        vector<int> parent(N);

        long long need = 0;

        // S -> Person
        for (int i = 0; i < n; i++) {
            cap[S][personStart + i] = f[i];
            need += f[i];
        }

        // Person -> Time
        for (int i = 0; i < n; i++) {
            for (int t = a[i]; t < d[i]; t++) {
                cap[personStart + i][timeStart + (t - 1)] = 1;
            }
        }

        // Time -> Sink
        for (int t = 0; t < timeSlots; t++) {
            cap[timeStart + t][D] = tables * chairs;
        }

        long long maxflow = 0;
        while (bfs(S, D, cap, flow, parent)) {
            long long push = INF;
            for (int v = D; v != S; v = parent[v]) {
                int u = parent[v];
                push = min(push, cap[u][v] - flow[u][v]);
            }
            for (int v = D; v != S; v = parent[v]) {
                int u = parent[v];
                flow[u][v] += push;
                flow[v][u] -= push;
            }
            maxflow += push;
        }

        cout << "Case " << tc << ": ";
        if (maxflow == need) cout << "Yes\n";
        else cout << "No\n";
    }
}
