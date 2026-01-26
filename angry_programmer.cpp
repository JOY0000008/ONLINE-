//problem link --> https://lightoj.com/problem/angry-programmer

/*

You, a programmer of an important software house, have been fired
 because you didn't solve an important problem that was assigned to you.
  You are very furious and want to take revenge on your boss, breaking the
   communication between his computer and the central server.

The computer of your boss and the central server are in the same network,
 which is composed of many machines (computers) and wires linking pairs of
  those machines. There is at most one wire between any pair of machines and 
  there can be pairs of machines without a wire between them.

To accomplish your objective, you can destroy machines and wires,
 but you can't destroy neither the computer of your boss, nor the
  central server, because those machines are monitored by security cameras. 
  You have estimated the cost of blowing up each machine and
   the cost of cutting each wire in the network.

You want to determine the minimum cost of interrupting the communication
 between your boss' computer and the central server. Two computers A and B
  can communicate if there is a sequence of undestroyed machines x1, x2, ..., xn
   such that x1 = A, xn = B and xi is linked with xi+1 with an uncut wire 
   (for each 1 ≤ i < n).

Input
Input starts with an integer T (≤ 50), denoting the number of test cases.

Each case starts with two integers M (2 ≤ M ≤ 50) and W (0 ≤ W ≤ 1200) 
where M denotes the number of machines and W denotes the number of wires.
 The ids of the boss' machine and the server are 1 and M respectively. 
 The next line contains M - 2 integers denoting the cost for destroying 
 the machines from 2 to M - 1 respectively. Each of the next W lines 
 contains three integers i j c, meaning that the wire between machine 
 i and j can be destroyed with the cost of c. All the destroying costs
  are between 0 and 105.

Output
For each case, print the case number and the minimum cost of interrupting
 the communication between the computer of your boss and the central server.

Sample
Input	Output
2
4 4
2 5
1 2 3
1 3 3
2 4 1
3 4 3
4 4
2 2
1 2 3
1 3 3
2 4 1
3 4 3

Case 1: 4
Case 2: 3



*/

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

const long long INF = 1e18;

bool bfs(int s, int t, vector<vector<long long>> &cap,
         vector<vector<long long>> &flow, vector<int> &parent)
{
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<int> q;
    q.push(s);

    while (!q.empty())
    {
        int u = q.front(); q.pop();
        for (int v = 0; v < cap.size(); v++)
        {
            if (parent[v] == -1 && cap[u][v] - flow[u][v] > 0)
            {
                parent[v] = u;
                if (v == t) return true;
                q.push(v);
            }
        }
    }
    return false;
}

int main()
{
    int T; cin >> T;
    for (int tc = 1; tc <= T; tc++)
    {
        int M, W;
        cin >> M >> W;

        vector<long long> destroy(M + 1, 0);
        for (int i = 2; i <= M - 1; i++)
            cin >> destroy[i];

        int N = 2 * (M + 1);
        vector<vector<long long>> cap(N, vector<long long>(N, 0));
        vector<vector<long long>> flow(N, vector<long long>(N, 0));

        auto in = [&](int x) { return 2 * x; };
        auto out = [&](int x) { return 2 * x + 1; };

        // Node destruction edges
        for (int i = 1; i <= M; i++)
        {
            if (i == 1 || i == M)
                cap[in(i)][out(i)] = INF;
            else
                cap[in(i)][out(i)] = destroy[i];
        }

        // Wire edges
        for (int i = 0; i < W; i++)
        {
            int u, v;
            long long c;
            cin >> u >> v >> c;
            cap[out(u)][in(v)] += c;
            cap[out(v)][in(u)] += c;
        }

        int S = in(1);
        int Tt = out(M);

        vector<int> parent(N);
        long long maxflow = 0;

        while (bfs(S, Tt, cap, flow, parent))
        {
            long long f = INF;
            int cur = Tt;
            while (cur != S)
            {
                int p = parent[cur];
                f = min(f, cap[p][cur] - flow[p][cur]);
                cur = p;
            }
            cur = Tt;
            while (cur != S)
            {
                int p = parent[cur];
                flow[p][cur] += f;
                flow[cur][p] -= f;
                cur = p;
            }
            maxflow += f;
        }

        cout << "Case " << tc << ": " << maxflow << "\n";
    }
}
