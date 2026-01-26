/*
Xuexue is a pretty monkey living in the Emei mountain. She is extremely thirsty during time 2 and
time 9 everyday, so she must drink 2 units water during this period. She may drink water more than
once, as long as the total amount of water she drinks is exactly 2 - she never drinks more than she
needs. Xuexue drinks 1 unit water in one time unit, so she may drinks from time 2 to time 4, or from
3 to 5, . . ., or from 7 to 9, or even drinks twice: first from 2 to 3, then from 8 to 9. But she can’t drink
from 1 to 3 since she’s not thirsty at time 1, and she can’t drink from 8 to 10, since she must finish at
time 9.
There are many monkeys like Xuexue: we use a triple (v, a, b) to describe a monkey who is thirsty
during time a and b, and must drink exactly v units of water during that period. Every monkey drinks
at the same speed (i.e. one unit water per unit time).
Unfortunately, people keep on doing something bad on the environment in Emei Mountain. Eventually, there are only one
unpolluted places for monkeys to drink. Further more, the place is so small
that at most m monkeys can drink water together. Monkeys like to help each other, so they want to
find a way to satisfy all the monkeys’ need. Could you help them?
Input
The input consists of several test cases. Each case contains two integers n and m (1 ≤ n ≤ 100, 1 ≤
m ≤ 5), followed by n lines of three integer numbers (v, a, b), where 0 ≤ v, a, b ≤ 50, 000, a < b,
0 < v ≤ b − a. The last test case is followed by a single zero, which should not be processed. There are
at most 50 test cases.
Output
For each test case, print the case number and whether there is a solution. If there is a solution, the
following n lines describe the solution: one monkey on a separate line. The first number k means the
monkey drinks water for k times. The following k pairs (ai
, bi) means the monkey drinks from ai to
bi (ai < bi). The pairs should be sorted in ascending order, and ai should not be equal to ai+1 for
1 ≤ i ≤ k − 1 (otherwise these two drinking periods could be combined). If more than one solution
exists, any one is acceptable. Note that there should be exactly one space between k and pairs (ai
, bi),
but no space within each pair.
Sample Input
3 1
2 2 9
2 3 5
3 5 8
2 1
4 5 9
4 8 12
5 2
2 1 3
2 3 5
2 5 7
2 1 7
4 2 6
0
Sample Output
Case 1: Yes
2 (2,3) (8,9)
1 (3,5)
1 (5,8)
Case 2: No
Case 3: Yes
1 (1,3)
1 (3,5)
1 (5,7)
2 (1,2) (6,7)
1 (2,6)
*/


#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
#include <numeric>
using namespace std;

#define INF 1e9

// BFS for finding augmenting path
bool bfs(int s, int t, vector<vector<int>> &cap, vector<vector<int>> &flow, vector<int> &parent) {
    int n = cap.size();
    fill(parent.begin(), parent.end(), -1);
    queue<int> q;
    q.push(s);
    parent[s] = -2;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v = 0; v < n; v++) {
            if (parent[v] == -1 && cap[u][v] - flow[u][v] > 0) {
                parent[v] = u;
                if (v == t) return true;
                q.push(v);
            }
        }
    }
    return false;
}

// Edmonds-Karp max flow
int maxFlow(int s, int t, vector<vector<int>> &cap, vector<vector<int>> &flow) {
    int total = 0;
    int n = cap.size();
    vector<int> parent(n);

    while (bfs(s, t, cap, flow, parent)) {
        int f = INF;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            f = min(f, cap[prev][cur] - flow[prev][cur]);
            cur = prev;
        }
        cur = t;
        while (cur != s) {
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
    ios::sync_with_stdio(false);
    cin.tie(0);

    int T, caseNo = 1;
    cin >> T;

    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<int> v(n), a(n), b(n);
        int maxTime = 0;
        for (int i = 0; i < n; i++) {
            cin >> v[i] >> a[i] >> b[i];
            maxTime = max(maxTime, b[i]);
        }

        int totalNodes = n + maxTime + 2;
        int s = 0, t = totalNodes - 1;
        vector<vector<int>> cap(totalNodes, vector<int>(totalNodes, 0));
        vector<vector<int>> flow(totalNodes, vector<int>(totalNodes, 0));

        // Source -> monkey nodes
        for (int i = 0; i < n; i++) {
            cap[s][1 + i] = v[i];
        }

        // Monkey nodes -> time nodes
        for (int i = 0; i < n; i++) {
            for (int time = a[i]; time < b[i]; time++) {
                cap[1 + i][1 + n + (time - 1)] = 1; // 1 unit per time
            }
        }

        // Time nodes -> Sink
        for (int time = 0; time < maxTime; time++) {
            cap[1 + n + time][t] = m; // max m monkeys per time
        }

        // Compute max flow
        int totalV = accumulate(v.begin(), v.end(), 0);
        int f = maxFlow(s, t, cap, flow);

        cout << "Case " << caseNo++ << ": ";
        if (f < totalV) {
            cout << "No\n";
            continue;
        }
        cout << "Yes\n";

        // Extract drinking schedule
        for (int i = 0; i < n; i++) {
            vector<int> times;
            for (int time = 0; time < maxTime; time++) {
                if (flow[1 + i][1 + n + time] > 0) {
                    times.push_back(time + 1);
                }
            }
            // Merge consecutive times
            vector<pair<int,int>> intervals;
            for (int j = 0; j < times.size(); j++) {
                int start = times[j];
                int end = start;
                while (j+1 < times.size() && times[j+1] == times[j]+1) {
                    end = times[j+1];
                    j++;
                }
                intervals.push_back({start, end});
            }

            cout << intervals.size();
            for (auto &p : intervals) {
                cout << " (" << p.first << "," << p.second << ")";
            }
            cout << "\n";
        }
    }
}
