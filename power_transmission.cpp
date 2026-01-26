// problem link---> https://lightoj.com/problem/power-transmission

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;
const long long INF = 1e9;

bool bfs(int S, int T, vector<vector<long long>> &cap, vector<vector<long long>> &flow, vector<int> &parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[S] = -2;
    queue<int> q;
    q.push(S);
    while(!q.empty()) {
        int u = q.front(); q.pop();
        for(int v = 0; v < cap.size(); v++) {
            if(parent[v] == -1 && cap[u][v] - flow[u][v] > 0) {
                parent[v] = u;
                if(v == T) return true;
                q.push(v);
            }
        }
    }
    return false;
}

long long maxFlow(int S, int T, vector<vector<long long>> &cap) {
    int n = cap.size();
    vector<vector<long long>> flow(n, vector<long long>(n, 0));
    vector<int> parent(n);
    long long total = 0;

    while(bfs(S, T, cap, flow, parent)) {
        long long f = INF;
        int cur = T;
        while(cur != S) {
            int prev = parent[cur];
            f = min(f, cap[prev][cur] - flow[prev][cur]);
            cur = prev;
        }
        cur = T;
        while(cur != S) {
            int prev = parent[cur];
            flow[prev][cur] += f;
            flow[cur][prev] -= f;
            cur = prev;
        }
        total += f;
    }
    return total;
}

int main() {
    int T;
    cin >> T;
    for(int test=1; test<=T; test++) {
        int N;
        cin >> N;
        vector<int> nodeCap(N);
        for(int i=0;i<N;i++) cin >> nodeCap[i];

        int M;
        cin >> M;
        vector<tuple<int,int,int>> edges;
        for(int i=0;i<M;i++) {
            int u,v,c;
            cin >> u >> v >> c;
            u--; v--;
            edges.push_back({u,v,c});
        }

        int B,D;
        cin >> B >> D;
        vector<int> entry(B), exit(D);
        for(int i=0;i<B;i++) { cin >> entry[i]; entry[i]--; }
        for(int i=0;i<D;i++) { cin >> exit[i]; exit[i]--; }

        int nodes = 2*N + 2;
        int S = 2*N, T = 2*N+1;
        vector<vector<long long>> cap(nodes, vector<long long>(nodes, 0));

        // Node splitting
        for(int i=0;i<N;i++) {
            cap[i][i+N] = nodeCap[i];
        }

        // Edges between regulators
        for(auto &[u,v,c]: edges) {
            cap[u+N][v] = c; // from out[u] -> in[v]
        }

        // Super-source -> entry nodes
        for(int i=0;i<B;i++) cap[S][entry[i]] = INF;

        // Exit nodes -> super-sink
        for(int i=0;i<D;i++) cap[exit[i]+N][T] = INF;

        long long f = maxFlow(S,T,cap);
        cout << "Case " << test << ": " << f << "\n";
    }
}
