/*

PROBLEM: SCHOOL DANCE

There are n boys and m girls in a school. Next week a school dance 
will be organized. A dance pair consists of a boy and a girl, and 
there are k potential pairs.

Your task is to find out the maximum number of dance pairs and show 
how this number can be achieved.

INPUT FORMAT:
- The first input line has three integers n, m, and k: the number of 
  boys, girls, and potential pairs.
- The boys are numbered 1, 2, ..., n.
- The girls are numbered 1, 2, ..., m.
- After this, there are k lines describing the potential pairs. 
  Each line has two integers a and b: boy a and girl b are willing 
  to dance together.

OUTPUT FORMAT:
- First, print one integer r: the maximum number of dance pairs.
- After this, print r lines describing the pairs.
- You can print any valid solution.

CONSTRAINTS:
- 1 <= n, m <= 500
- 1 <= k <= 1000
- 1 <= a <= n
- 1 <= b <= m

EXAMPLE:

Input:
3 2 4
1 1
1 2
2 1
3 1

Output:
2
1 2
3 1

EXPLANATION:
The maximum number of unique pairs that can be formed is 2. 
In this solution, Boy 1 dances with Girl 2, and Boy 3 dances with Girl 1. 
Every boy and girl can be part of at most one pair.

*/

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

int n, m, k;
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
        int f = INT_MAX;
        int curr = t;

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
    

    cin >> n >> m >> k;

    int S = 0;
    int T = n + m + 1;
    int N = T + 1;

    capacity.assign(N, vector<int>(N, 0));
    adj.assign(N, vector<int>());
    parent.assign(N, -1);

    // Source -> boys
    for (int i = 1; i <= n; i++)
    {
        capacity[S][i] = 1;
        adj[S].push_back(i);
        adj[i].push_back(S);
    }

    // Girls -> sink
    for (int i = 1; i <= m; i++)
    {
        int girl = n + i;
        capacity[girl][T] = 1;
        adj[girl].push_back(T);
        adj[T].push_back(girl);
    }

    for (int i = 0; i < k; i++)
    {
        int b, g;
        cin >> b >> g;
        int girl = n + g;

        capacity[b][girl] = 1;
        adj[b].push_back(girl);
        adj[girl].push_back(b);
    }

    int max_matching = edmonds_karp(S, T);
    cout << max_matching << "\n";

    for (int b = 1; b <= n; b++)
    {
        for (int g = 1; g <= m; g++)
        {
            int girl = n + g;
            if (capacity[girl][b] == 1)//backward edge in residual graph (this edge is saturated and used)
            {
                cout << b << " " << g << "\n";
                break; // one girl per boy
            }
        }
    }

    return 0;
}
