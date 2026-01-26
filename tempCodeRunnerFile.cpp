#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

bool bfs(int s, int t, vector<vector<long long>> &capacity, vector<vector<long long>> &flow, vector<int> &parent)
{
    if (s == t)
        return true;

    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<int> q;
    q.push(s);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v = 0; v < capacity.size(); v++)
        {
            if (parent[v] == -1 && capacity[u][v] - flow[u][v] > 0)
            {
                parent[v] = u;
                q.push(v);
                if (v == t)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

int main()
{
    int n, m, s, t;
    long long total_flow = 0;
    cin >> n >> m;
    vector<vector<long long>> capacity(n, vector<long long>(n, 0));
    vector<vector<long long>> flow(n, vector<long long>(n, 0));
    vector<int> parent(n, -1);

    for (int i = 0; i < m; i++)
    {
        int a, b;
        long long c;
        cin >> a >> b >> c;
        capacity[a][b] += c;
    
    }

    cin >> s >> t;

    while (bfs(s, t, capacity, flow, parent))
    {
        int curr = t;
        long long f = LLONG_MAX;
        while (curr != s)
        {
            int prev = parent[curr];
            f = min(f, capacity[prev][curr] - flow[prev][curr]);
            curr = prev;
        }
        curr = t;
        while (curr != s)
        {
            int prev = parent[curr];
            flow[prev][curr] += f;
            flow[curr][prev] -= f;
            curr = prev;
        }
        total_flow += f;
    }
    cout << total_flow << endl;

    for (int u = 0; u < n; u++)
    {
        for (int v = 0; v < n; v++)
        {
            if (capacity[u][v] > 0)
            {
                cout << u << " " << v << " " << flow[u][v] << "/" << capacity[u][v] << endl;
            }
        }
    }
}