
/*
n programming contests, a common problem for the contestants is to get a suited T-shirt. Sometimes people gets too long or too short T-shirts. So, this time I have planned to ask the authority to manage the T-shirts such that everyone gets a suitable one. From my past experience, it's known that there are 6 available sizes of T-shirts and they are XXL, XL, L, M, S, and XS. And exactly two sizes of the T-shirts suit a person.

Now, for a contest there are T-shirts of N colors and M contestants. And for each color, all the 6 sizes are available. So, there are 6 * N T-shirts. And you are given the suitable sizes for each contestant. You have to distribute the T-shirts to the contestants such that everyone gets a suitable size. Only size matters, color is not an issue. Now you have to decide whether it's possible or not.

Input
Input starts with an integer T (≤ 50), denoting the number of test cases.

The first line of each test case contains two integers N and M, separated by spaces, with 1 ≤ N, M ≤ 50. Each of the next M lines will contain two sizes as described earlier.

Output
For each case, print the case number and "YES" or "NO" depending on whether it's possible to distribute the T-shirts or not.

Sample
Input	Output
3
3 6
L XL
XL L
XXL XL
S XS
M S
M L
1 4
S XL
L S
L XL
L XL
1 1
L M

Case 1: YES
Case 2: NO
Case 3: YES


*/

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
#include <map>
using namespace std;

bool bfs(int s, int t, vector<vector<int>> &cap, vector<vector<int>> &flow, vector<int> &parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<int> q;
    q.push(s);
    while(!q.empty()) {
        int u = q.front(); q.pop();
        for(int v=0; v<cap.size(); v++) {
            if(parent[v]==-1 && cap[u][v] - flow[u][v] > 0) {
                parent[v] = u;
                if(v == t) return true;
                q.push(v);
            }
        }
    }
    return false;
}

int main() {
    int T; cin >> T;
    map<string,int> sizeMap = {{"XXL",0},{"XL",1},{"L",2},{"M",3},{"S",4},{"XS",5}};

    for(int test=1; test<=T; test++) {
        int N, M;
        cin >> N >> M;

        int numNodes = 2 + M + 6; // Source + Sink + M contestants + 6 sizes
        int S = 0, Tnode = numNodes-1;
        vector<vector<int>> cap(numNodes, vector<int>(numNodes,0));
        vector<vector<int>> flow(numNodes, vector<int>(numNodes,0));

        // Source → Contestants
        for(int i=0; i<M; i++) {
            cap[S][1+i] = 1;
        }

        // Contestants → their 2 suitable sizes
        for(int i=0; i<M; i++) {
            string s1, s2; cin >> s1 >> s2;
            int size1 = sizeMap[s1];
            int size2 = sizeMap[s2];
            cap[1+i][1+M+size1] = 1;
            cap[1+i][1+M+size2] = 1;
        }

        // Sizes → Sink
        for(int i=0;i<6;i++) {
            cap[1+M+i][Tnode] = N;
        }

        // Edmonds-Karp BFS
        vector<int> parent(numNodes);
        int total_flow = 0;
        while(bfs(S,Tnode,cap,flow,parent)) {
            int f = INT_MAX;
            int cur = Tnode;
            while(cur != S) {
                int prev = parent[cur];
                f = min(f, cap[prev][cur] - flow[prev][cur]);
                cur = prev;
            }
            cur = Tnode;
            while(cur != S) {
                int prev = parent[cur];
                flow[prev][cur] += f;
                flow[cur][prev] -= f;
                cur = prev;
            }
            total_flow += f;
        }

        cout << "Case " << test << ": ";
        if(total_flow == M) cout << "YES\n";
        else cout << "NO\n";
    }
}
