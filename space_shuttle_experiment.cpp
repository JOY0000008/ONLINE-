/*
Input starts with an integer T (≤ 100), denoting the number of test cases.

Each case starts with a line containing two integers m (1 ≤ m ≤ 100) and n (1 ≤ n ≤ 100),
 *where *m denotes the number of experiments and n denotes the number of instruments. 
 The next line contains m space separated integers, where the jth integer denotes the
  commercial sponsor of Ej paying NASA pj (1 ≤ pj ≤ 10000) dollars for the result of the experiment.
   The next line contains n space separated integers, where the kth integer denotes the cost of carrying
    the kth instrument, ck (1 ≤ ck ≤ 10000). Each of the next m lines contains an integer qi (1 ≤ qi ≤ n)
     followed by qi distinct integers each between 1 and n, separated by spaces. These qi integers denote
      the required instruments for the ith experiment
*/

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

const long long INF = 1e18;


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
    cin >> m >> n;
    s=0;
    t=m+n+1;
    int N=m+n+2;
    vector<int>experiment(m);
    vector<int>instrument(n);
    vector<vector<long long>> capacity(N, vector<long long>(N, 0));
    vector<vector<long long>> flow(N, vector<long long>(N, 0));
    vector<int> parent(N, -1);
    long long total_profit=0;
    for(int i=0;i<m;i++)
    {
        cin>>experiment[i];
        capacity[s][1+i]=experiment[i];
        total_profit+=experiment[i];

    }
    for(int i=0;i<n;i++)
    {
        cin>>instrument[i];
        capacity[1+m+i][t]=instrument[i];

    }


     for (int i = 0; i < m; i++) {
            int q;
            cin >> q;
            while (q--) {
                int k;
                cin >> k;
                --k;
               capacity[i+1][m+1+k]=INF;
            }
        }

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
    cout << total_profit-total_flow << endl;


}
