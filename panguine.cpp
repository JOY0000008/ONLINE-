/*

Somewhere near the South Pole, a number of penguins are standing on a number of ice floes. Being social animals, the penguins would like to get together, all on the same floe. The penguins do not want to get wet, so they have use their limited jumping capabilities to get together by jumping from piece to piece.

However, temperatures have risen lately, and the floes are showing cracks, and they get damaged further by the force needed to jump to another floe. Fortunately the penguins are real experts on cracking ice floes, and know exactly how many times a penguin can jump off each floe before it disintegrates and disappears. Landing on an ice floe does not damage it. You have to help the penguins find all floes where they can meet.

A sample layout of ice floes with 3 penguins on them is shown in the picture.

Input
Input starts with an integer T (≤ 25), denoting the number of test cases.

Each case starts with the integer N (1 ≤ N ≤ 100) and a floating-point number D (0 ≤ D ≤ 105), denoting the number of ice pieces and the maximum distance a penguin can jump.

After that there will be N lines, each line containing xi, yi, ni and mi, denoting for each ice piece its X and Y coordinate, the number of penguins on it and the maximum number of times a penguin can jump off this piece before it disappears (-10000 ≤ xi, yi ≤ 10000, 0 ≤ ni ≤ 10, 1 ≤ mi ≤ 200).

Output
For each case of input, print the case number and a space-separated list of 0-based indices of the pieces on which all penguins can meet. If no such piece exists, output -1.

Sample
Input	Output
2
5 3.5
1 1 1 1
2 3 0 1
3 5 1 1
5 1 1 1
5 4 0 1
3 1.1
-1 0 5 10
0 0 3 9
2 0 1 1

Case 1: 1 2 4
Case 2: -1



*/


#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
using namespace std;

struct Floe {
    double x, y;
    int ni, mi;
};

const long long INF = 1e9;

bool bfs(int S, int T, vector<vector<long long>> &cap, vector<vector<long long>> &flow, vector<int> &parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[S] = -2;
    queue<int> q;
    q.push(S);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v = 0; v < cap.size(); v++) {
            if (parent[v] == -1 && cap[u][v] - flow[u][v] > 0) {
                parent[v] = u;
                if (v == T) return true;
                q.push(v);
            }
        }
    }
    return false;
}

long long maxFlow(int S, int T, vector<vector<long long>> &cap) {
    int n = cap.size();
    vector<vector<long long>> flow(n, vector<long long>(n, 0));
    vector<int> parent(n);
    long long total_flow = 0;

    while (bfs(S, T, cap, flow, parent)) {
        long long f = INF;
        int cur = T;
        while (cur != S) {
            int prev = parent[cur];
            f = min(f, cap[prev][cur] - flow[prev][cur]);
            cur = prev;
        }
        cur = T;
        while (cur != S) {
            int prev = parent[cur];
            flow[prev][cur] += f;
            flow[cur][prev] -= f;
            cur = prev;
        }
        total_flow += f;
    }
    return total_flow;
}

int main() {
    int T;
    cin >> T;
    for (int test = 1; test <= T; test++) {
        int N;
        double D;
        cin >> N >> D;
        vector<Floe> floes(N);
        int totalPenguins = 0;
        for (int i = 0; i < N; i++) {
            cin >> floes[i].x >> floes[i].y >> floes[i].ni >> floes[i].mi;
            totalPenguins += floes[i].ni;
        }

        vector<int> answer;

        for (int target = 0; target < N; target++) {
            int nodes = 2 * N + 2; // in-node and out-node for each floe + source + sink
            int S = 2 * N;
            int T = 2 * N + 1;

            vector<vector<long long>> cap(nodes, vector<long long>(nodes, 0));

            // Source -> floe_in[i]
            for (int i = 0; i < N; i++) {
                if (floes[i].ni > 0)
                    cap[S][i] = floes[i].ni;
            }

            // floe_in[i] -> floe_out[i] (capacity = mi)
            for (int i = 0; i < N; i++) {
                cap[i][i + N] = floes[i].mi;
            }

            // floe_out[i] -> floe_in[j] if distance <= D
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    if (i == j) continue;
                    double dx = floes[i].x - floes[j].x;
                    double dy = floes[i].y - floes[j].y;
                    double dist = sqrt(dx*dx + dy*dy);
                    if (dist <= D) {
                        cap[i + N][j] = INF; // can jump
                    }
                }
            }

            // target floe_in -> sink
            cap[target][T] = INF;

            long long f = maxFlow(S, T, cap);
            if (f == totalPenguins) answer.push_back(target);
        }

        cout << "Case " << test << ": ";
        if (answer.empty()) cout << -1;
        else {
            for (int idx : answer) cout << idx << " ";
        }
        cout << "\n";
    }
}
