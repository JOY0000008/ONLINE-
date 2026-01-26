/*
The world is in great danger!! Mental’s forces have returned to
Earth to eradicate humankind. Our last hope to stop this great
evil is Sam “Serious” Stone. Equipped with various powerful
weapons, Serious Sam starts his mission to destroy the forces of
evil.
After fighting two days and three nights, Sam is now in front of
the temple KOPTOS where Mental’s general Ugh Zan III is waiting
for him. But this time, he has a serious problem. He is in shortage
of ammo and a lot of enemies crawling inside the temple waiting
for him. After rounding the temple Sam finds that the temple is
in rectangle shape and he has the locations of all enemies in the
temple.
All of a sudden he realizes that he can kill the enemies without entering the temple using the great
cannon ball which spits out a gigantic ball bigger than him killing anything it runs into and keeps on
rolling until it finally explodes. But the cannonball can only shoot horizontally or vertically and all the
enemies along the path of that cannon ball will be killed.
Now he wants to save as many cannon balls as possible for fighting with Mental. So, he wants to
know the minimum number of cannon balls and the positions from which he can shoot the cannonballs
to eliminate all enemies from outside that temple.
Input
The input file contains several test cases.
Here, the temple is defined as a R × C grid. The first line of each test case contains 3 integers:
R (0 < R < 1001), C (0 < C < 1001) representing the grid of temple (R means number of row and
C means number of column of the grid) and the number of enemies N (0 < N < 1000001) inside the
temple. After that there are N lines each of which contains 2 integers representing the position of
the enemies in that temple. Each test case is followed by a new line (except the last one). Input is
terminated when R = C = N = 0.
Output
For each test case there will be one line output. First print the minimum number (m) of cannonballs
needed to wipe out the enemies followed by a single space and then m positions from which he can
shoot those cannonballs. For shooting horizontally print ‘r’ followed by the row number and for vertical
shooting print ‘c’ followed by the column number. If there is more than one solution any one will do.
Sample Input
4 4 3
1 1
1 4
3 2
4 4 2
1 1
2 2
0 0 0
Sample Output
2 r1 r3
2 r1 r2
*/

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

static const long long INF = 1e18;

bool bfs(int s, int t,
         vector<vector<long long>> &cap,
         vector<vector<long long>> &flow,
         vector<int> &par)
{
    fill(par.begin(), par.end(), -1);
    queue<int> q;
    q.push(s);
    par[s] = -2;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v = 0; v < cap.size(); v++) {
            if (par[v] == -1 && cap[u][v] - flow[u][v] > 0) {
                par[v] = u;
                if (v == t) return true;
                q.push(v);
            }
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, C, N;
    while (cin >> R >> C >> N) {
        if (R == 0 && C == 0 && N == 0) break;

        vector<pair<int,int>> enemies(N);
        for (int i = 0; i < N; i++) {
            cin >> enemies[i].first >> enemies[i].second;
            enemies[i].first--;
            enemies[i].second--;
        }

        int S = 0;
        int rowStart = 1;
        int colStart = rowStart + R;
        int T = colStart + C;
        int V = T + 1;

        vector<vector<long long>> cap(V, vector<long long>(V, 0));
        vector<vector<long long>> flow(V, vector<long long>(V, 0));
        vector<int> par(V);

        // S -> rows
        for (int i = 0; i < R; i++) {
            cap[S][rowStart + i] = 1;
        }

        // columns -> T
        for (int j = 0; j < C; j++) {
            cap[colStart + j][T] = 1;
        }

        // row -> column (enemy)
        for (auto &e : enemies) {
            int r = e.first;
            int c = e.second;
            cap[rowStart + r][colStart + c] = INF;
        }

        // Edmonds–Karp
        long long maxflow = 0;
        while (bfs(S, T, cap, flow, par)) {
            long long f = INF;
            for (int v = T; v != S; v = par[v]) {
                f = min(f, cap[par[v]][v] - flow[par[v]][v]);
            }
            for (int v = T; v != S; v = par[v]) {
                flow[par[v]][v] += f;
                flow[v][par[v]] -= f;
            }
            maxflow += f;
        }

        // Find min cut
        vector<bool> vis(V, false);
        queue<int> q;
        q.push(S);
        vis[S] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v = 0; v < V; v++) {
                if (!vis[v] && cap[u][v] - flow[u][v] > 0) {
                    vis[v] = true;
                    q.push(v);
                }
            }
        }

        vector<string> ans;
        for (int i = 0; i < R; i++) {
            if (!vis[rowStart + i])
                ans.push_back("r" + to_string(i + 1));
        }
        for (int j = 0; j < C; j++) {
            if (vis[colStart + j])
                ans.push_back("c" + to_string(j + 1));
        }

        cout << ans.size();
        for (auto &s : ans) cout << " " << s;
        cout << "\n";
    }
}
