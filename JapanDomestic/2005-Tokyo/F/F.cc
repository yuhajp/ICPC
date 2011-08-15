#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
typedef pair<int, int> P;

int distance(int H, int W, const vector<vector<char> >& table, P start, P end)
{
    vector<vector<bool> > visited(H, vector<bool>(W, false));
    queue<pair<P, int> > Q;
    Q.push(make_pair(start, 0));
    
    while (!Q.empty()) {
        pair<P, int> cur = Q.front(); Q.pop();
        P p = cur.first;
        if (p.first < 0 || H <= p.first || p.second < 0 || W <= p.second) { continue; }
        if (visited[p.first][p.second]) { continue; }
        if (table[p.first][p.second] == 'x') { continue; }

        visited[p.first][p.second] = true;
        if (p == end) { return cur.second; }
        Q.push(make_pair(P(p.first + 1, p.second), cur.second + 1));
        Q.push(make_pair(P(p.first - 1, p.second), cur.second + 1));
        Q.push(make_pair(P(p.first, p.second + 1), cur.second + 1));
        Q.push(make_pair(P(p.first, p.second - 1), cur.second + 1));
    }

    return -1;
}

int solve(int H, int W, const vector<vector<char> >& table, const vector<P>& ps) {
    int n = ps.size();
    if (n == 1) { return 0; }

    // ps[i] <-> ps[j] の表を作る
    vector<vector<int> > dist(n, vector<int>(n, -1));
    for (int i = 0; i < n; ++i) {
        dist[i][i] = 0;
        for (int j = i + 1; j < n; ++j) {
            int d = distance(H, W, table, ps[i], ps[j]);
            if (d < 0) { return -1; }
            dist[i][j] = dist[j][i] = d;
        }
    }

    // ps[0] から始めて、全通りを列挙して最短距離を求める。
    vector<int> perm(n - 1);
    for (int i = 0; i < n - 1; ++i) { perm[i] = i + 1; }
    
    int minLength = INT_MAX;
    do {
        int length = dist[0][perm[0]];
        for (int i = 0; i + 1 < perm.size(); ++i) {
            length += dist[perm[i]][perm[i + 1]];
        }
        minLength = min(minLength, length);
    } while (next_permutation(perm.begin(), perm.end()));

    return minLength;
}


int main(void)
{
    for (int W, H; cin >> W >> H, (W || H); ) {
        // ps[0] にスタート地点が、それ以降に dirty place が入っている。
        vector<P> ps;
        ps.push_back(make_pair(0, 0));
        vector<vector<char> > table(H, vector<char>(W));
        for (int i = 0; i < H; ++i) {
            for (int j = 0; j < W; ++j) {
                cin >> table[i][j];
                if (table[i][j] == '*') {
                    ps.push_back(make_pair(i, j));
                } else if (table[i][j] == 'o') {
                    ps[0] = make_pair(i, j);
                }
            }
        }

        cout << solve(H, W, table, ps) << endl;
    }
    return 0;
}

