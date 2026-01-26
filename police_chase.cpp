/*
PROBLEM: POLICE CHASE

Kaaleppi has just robbed a bank and is now heading to the harbor.
However, the police want to stop him by closing some streets of the city.

What is the minimum number of streets that should be closed so 
that there is no route between the bank and the harbor?

INPUT FORMAT:
- The first input line has two integers n and m: the number of crossings 
  and streets.
- The crossings are numbered 1, 2, ..., n.
- The bank is at crossing 1, and the harbor is at crossing n.
- After this, there are m lines describing the streets. Each line has 
  two integers a and b: there is a street between crossings a and b.
- All streets are two-way (bidirectional).
- There is at most one street between any two crossings.

OUTPUT FORMAT:
- First, print an integer k: the minimum number of streets to be closed.
- After this, print k lines describing the streets.
- You can print any valid solution.

CONSTRAINTS:
- 2 <= n <= 500
- 1 <= m <= 1000
- 1 <= a, b <= n

EXAMPLE:

Input:
4 5
1 2
1 3
2 3
3 4
1 4

Output:
2
3 4
1 4

EXPLANATION:
By closing the streets between (3,4) and (1,4), there is no longer 
any path from crossing 1 to crossing 4. The number of streets 
closed (2) is the minimum possible (this is a Min-Cut problem).

*/
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;

int n, m;
vector<vector<int>> capacity;
vector<vector<int>> adj;
vector<int> parent;

bool bfs(int s, int t)
{
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;

    queue<int> q;
    q.push(s);

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v : adj[u])
        {
            if (parent[v] == -1 && capacity[u][v] > 0)
            {
                parent[v] = u;
                if (v == t)
                    return true;
                q.push(v);
            }
        }
    }
    return false;
}

int edmonds_karp(int s, int t)
{
    int flow = 0;

    while (bfs(s, t))
    {
        int curr = t;
        int f = INT_MAX;

        while (curr != s)
        {
            int prev = parent[curr];
            f = min(f, capacity[prev][curr]);
            curr = prev;
        }

        curr = t;
        while (curr != s)
        {
            int prev = parent[curr];
            capacity[prev][curr] -= f;
            capacity[curr][prev] += f;
            curr = prev;
        }

        flow += f;
    }
    return flow;
}

int main()
{
  

    cin >> n >> m;

    capacity.assign(n + 1, vector<int>(n + 1, 0));
    adj.assign(n + 1, vector<int>());
    parent.assign(n + 1, -1);

    vector<pair<int, int>> edges;

    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;

        // undirected → two directed edges
        capacity[u][v] = 1;
        capacity[v][u] = 1;

        adj[u].push_back(v);
        adj[v].push_back(u);

        edges.push_back({u, v});
    }

    // Max flow = min cut
    edmonds_karp(1, n);

    // Find reachable nodes from source in residual graph
    vector<bool> visited(n + 1, false);
    queue<int> q;
    q.push(1);
    visited[1] = true;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v : adj[u])
        {
            if (!visited[v] && capacity[u][v] > 0)
            {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    // Output min-cut edges
    vector<pair<int, int>> cut;
    for (auto [u, v] : edges)
    {
        if (visited[u] && !visited[v])
            cut.push_back({u, v});
        else if (visited[v] && !visited[u])
            cut.push_back({v, u});
    }

    cout << cut.size() << "\n";
    for (auto &e : cut)
        cout << e.first << " " << e.second << "\n";

    return 0;
}
