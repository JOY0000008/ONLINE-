/*
PROBLEM: DOWNLOAD SPEED

Consider a network consisting of n computers and m connections. Each 
connection specifies how fast a computer can send data to another computer.

Kotivalo wants to download some data from a server. What is the maximum 
speed he can do this, using the connections in the network?

INPUT FORMAT:
- The first input line has two integers n and m: the number of computers 
  and connections.
- The computers are numbered 1, 2, ..., n. 
- Computer 1 is the server and computer n is Kotivalo's computer.
- After this, there are m lines describing the connections. Each line 
  has three integers a, b, and c: 
    a: source computer
    b: destination computer
    c: transmission speed

OUTPUT FORMAT:
- Print one integer: the maximum speed Kotivalo can download data.

CONSTRAINTS:
- 1 <= n <= 500
- 1 <= m <= 1000
- 1 <= a, b <= n
- 1 <= c <= 10^9

EXAMPLE:

Input:
4 5
1 2 3
2 4 2
1 3 4
3 4 5
4 1 3

Output:
6

EXPLANATION:
The maximum flow from computer 1 to computer 4 is 6. 
You can send 2 units through 1 -> 2 -> 4 and 4 units through 1 -> 3 -> 4.

*/


#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

bool bfs(int s, int t, vector<vector<long long>> &capacity,
         vector<bool> &visited, vector<int> &parent)
{
    queue<int> q;
    q.push(s);
    visited[s] = true;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v = 1; v < capacity.size(); v++)
        {
            if (!visited[v] && capacity[u][v] > 0)
            {
                parent[v] = u;
                visited[v] = true;
                q.push(v);

                if (v == t)
                    return true;
            }
        }
    }
    return false;
}

long long ford_fulkerson(int s, int d, vector<vector<long long>> &capacity)
{
    long long max_flow = 0;
    int n = capacity.size();

    vector<int> parent(n, -1);
    vector<bool> visited(n, false);

    while (bfs(s, d, capacity, visited, parent))
    {
        long long flow = LLONG_MAX;

        int curr = d;
        while (curr != s)
        {
            int prev = parent[curr];
            flow = min(flow, capacity[prev][curr]);
            curr = prev;
        }

        max_flow += flow;

        curr = d;
        while (curr != s)
        {
            int prev = parent[curr];
            capacity[prev][curr] -= flow;
            capacity[curr][prev] += flow;
            curr = prev;
        }

        fill(visited.begin(), visited.end(), false);
        fill(parent.begin(), parent.end(), -1);
    }

    return max_flow;
}

int main()
{
    int n, e;
    cin >> n >> e;

    vector<vector<long long>> capacity(n + 1, vector<long long>(n + 1, 0));

    for (int i = 0; i < e; i++)
    {
        int u, v, c;
        cin >> u >> v >> c;
        capacity[u][v] += c; // FIX: handle multiple edges
    }

    int s = 1;
    int d = n;

    cout << ford_fulkerson(s, d, capacity) << endl;

    return 0;
}
