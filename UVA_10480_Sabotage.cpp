/*
The regime of a small but wealthy dictatorship has been abruptly overthrown by an unexpected rebellion. Because of the enormous disturbances this is causing in world economy, an imperialist military
super power has decided to invade the country and reinstall the old regime.
For this operation to be successful, communication between the capital and the largest city must
be completely cut. This is a difficult task, since all cities in the country are connected by a computer
network using the Internet Protocol, which allows messages to take any path through the network.
Because of this, the network must be completely split in two parts, with the capital in one part and
the largest city in the other, and with no connections between the parts.
There are large differences in the costs of sabotaging different connections, since some are much
more easy to get to than others.
Write a program that, given a network specification and the costs of sabotaging each connection,
determines which connections to cut in order to separate the capital and the largest city to the lowest
possible cost.
Input
Input file contains several sets of input. The description of each set is given below.
The first line of each set has two integers, separated by a space: First one the number of cities, n in
the network, which is at most 50. The second one is the total number of connections, m, at most 500.
The following m lines specify the connections. Each line has three parts separated by spaces: The
first two are the cities tied together by that connection (numbers in the range 1 − n). Then follows the
cost of cutting the connection (an integer in the range 1 to 40000000). Each pair of cites can appear
at most once in this list.
Input is terminated by a case where values of n and m are zero. This case should not be processed.
For every input set the capital is city number 1, and the largest city is number 2.
Output
For each set of input you should produce several lines of output. The description of output for each set
of input is given below:
The output for each set should be the pairs of cities (i.e. numbers) between which the connection
should be cut (in any order), each pair on one line with the numbers separated by a space. If there is
more than one solution, any one of them will do.
Print a blank line after the output for each set of input.
Sample Input
5 8
1 4 30
1 3 70
5 3 20
4 3 5
4 5 15
5 2 10
3 2 25
2 4 50
5 8
1 4 30
1 3 70
5 3 20
4 3 5
4 5 15
5 2 10
3 2 25
2 4 50
0 0
Sample Output
4 1
3 4
3 5
3 2
4 1
3 4
3 5
3 2
*/


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
        a--,b--;
        capacity[a][b] += c;
        capacity[b][a] +=c;
     
    }

   s=0;
   t=1;

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

    vector<bool>reachable(n,false);
    reachable[s]=true;
    queue<int> q;
    q.push(s);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        for(int v=0;v<n;v++)
        {
            if(!reachable[v]&&capacity[u][v]-flow[u][v]>0)
            {
                reachable[v]=true;
                q.push(v);
            }
        }
    }

   for (int u = 0; u < n; u++)
        {
            for (int v = u + 1; v < n; v++)
            {
                if (capacity[u][v] > 0 &&
                    reachable[u] != reachable[v])
                {
                    cout << u + 1 << " " << v + 1 << endl;
                }
            }
        }

}


