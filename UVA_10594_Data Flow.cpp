/*
UVA 10594 – Data Flow

Problem:
You are given a network of N nodes (2 ≤ N ≤ 100) and M edges (1 ≤ M ≤ 5000). Each edge
is undirected, has a capacity (maximum number of units that can flow through it at a time),
and a propagation time per unit.

You are required to send D units of data from node 1 (source) to node N (sink). Each edge
can carry at most K units at a time. At any instant, only one unit can travel on a link
(i.e., discrete flow). Each node can buffer unlimited data. Find the minimum total time 
to send all D units of data. If it is impossible, output "Impossible".

Input:
- N M
- Next M lines: u v t  (undirected edge from u to v with propagation time t)
- Next line: D K

Output:
- Minimum total time to send D units, or "Impossible" if not possible.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

const long long INF = 1e18;

// SPFA to find shortest path with residual capacity
bool spfa(int s, int t,
          vector<vector<long long>> &cap,
          vector<vector<long long>> &cost,
          vector<vector<long long>> &flow,
          vector<int> &parent,
          vector<long long> &dist)
{
    int n = cap.size();
    fill(dist.begin(), dist.end(), INF);
    fill(parent.begin(), parent.end(), -1);
    vector<bool> inq(n, false);
    queue<int> q;

    dist[s] = 0;
    parent[s] = -2;
    q.push(s);
    inq[s] = true;

    while (!q.empty())
    {
        int u = q.front(); q.pop();
        inq[u] = false;

        for (int v = 0; v < n; v++)
        {
            if (cap[u][v] - flow[u][v] > 0 &&
                dist[v] > dist[u] + cost[u][v])
            {
                dist[v] = dist[u] + cost[u][v];
                parent[v] = u;
                if (!inq[v])
                {
                    q.push(v);
                    inq[v] = true;
                }
            }
        }
    }
    return dist[t] != INF;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    while(cin >> N >> M) { // Handle EOF automatically

        vector<vector<long long>> cap(N+1, vector<long long>(N+1,0));
        vector<vector<long long>> cost(N+1, vector<long long>(N+1,0));
        vector<vector<long long>> flow(N+1, vector<long long>(N+1,0));

        // Read edges
        for(int i=0;i<M;i++){
            int u,v,t;
            cin >> u >> v >> t;
            cap[u][v] = cap[v][u] = 1e9; // Infinite capacity before scaling
            cost[u][v] = cost[v][u] = t;
        }

        long long D, K;
        cin >> D >> K;

        // Scale capacities by K (each edge can carry at most K units)
        for(int u=1;u<=N;u++)
            for(int v=1;v<=N;v++)
                if(cap[u][v]>0) cap[u][v] = K;

        int S = 1, Tt = N;
        long long totalCost = 0;
        long long totalFlow = 0;

        vector<int> parent(N+1);
        vector<long long> dist(N+1);

        while(totalFlow < D && spfa(S,Tt,cap,cost,flow,parent,dist)) {
            long long f = D - totalFlow;
            int cur = Tt;

            // Find bottleneck along the path
            while(cur != S) {
                int p = parent[cur];
                f = min(f, cap[p][cur] - flow[p][cur]);
                cur = p;
            }

            // Augment flow along the path
            cur = Tt;
            while(cur != S) {
                int p = parent[cur];
                flow[p][cur] += f;
                flow[cur][p] -= f;
                totalCost += f * cost[p][cur];
                cur = p;
            }

            totalFlow += f;
        }

        if(totalFlow < D) cout << "Impossible.\n";
        else cout << totalCost << "\n";
    }

    return 0;
}
