/*
GREED - Greedy island
#max-flow #minimum-cost-flow

Gon is on Greedy island. He wants to go home. 
But to get the ticket to leave the game, he has 
to get N cards labeled in a sequence from 1 to N 
(the order of the cards in his hand is irrelevant). 
He already has N cards, but not forming a sequence from 1 to N
 So he wants you to help him. For some cards, he can change one 
 card for another for one piece of gold. Help him to get the ticket
  at the minimum cost (using the minimum number of exchanges).

Input
The first line contains t, the number of tests (1<=t<=10). For each test case:

the number of cards N is given is given in the first line (2<=N<=500).
the next N lines contain the N cards owned by Gon.
the following line contains e, the number of different allowed types of exchanges.
the next e lines contain two integers xi,yi each which mean that we can 
exchange and replace the card marked x by the card marked y and vice versa.
There is a blank line after each test case.

Output
For each test case, output a line denoting the minimum required cost.

Example
Input:
1
4
1
2
2
2
2
2 3
3 4

Output:
3

*/

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

const long long INF = 1e18;

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

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--)
    {
        int N;
        cin >> N;

        vector<int> have(N + 1, 0);
        for (int i = 0; i < N; i++)
        {
            int x;
            cin >> x;
            have[x]++;
        }

        int e;
        cin >> e;

        int S = 0;
        int Tt = N + 1;
        int V = N + 2;

        vector<vector<long long>> cap(V, vector<long long>(V, 0));
        vector<vector<long long>> cost(V, vector<long long>(V, 0));
        vector<vector<long long>> flow(V, vector<long long>(V, 0));

        // Source / Sink edges
        for (int i = 1; i <= N; i++)
        {
            if (have[i] > 1)
            {
                cap[S][i] = have[i] - 1;
            }
            else if (have[i] == 0)
            {
                cap[i][Tt] = 1;
            }
        }

        // Exchange edges
        for (int i = 0; i < e; i++)
        {
            int x, y;
            cin >> x >> y;
            cap[x][y] = cap[y][x] = INF;
            cost[x][y] = cost[y][x] = 1;
    
        }

        long long minCost = 0;
        vector<int> parent(V);
        vector<long long> dist(V);

        while (spfa(S, Tt, cap, cost, flow, parent, dist))
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
                minCost += f * cost[p][cur];
                cur = p;
            }
        }

        cout << minCost << "\n";
    }
    return 0;
}
