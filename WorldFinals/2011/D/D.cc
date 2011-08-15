#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <climits>
#include <cstdlib>

using namespace std;
typedef vector<int> vi;
typedef vector<vi> vvi;

#define IN(x)  (2*(x))
#define OUT(x) (2*(x)+1)
const int LARGE_COST = 10000;

ostream& operator<<(ostream& out, const vi& v)
{
    for (int i = 0; i < v.size(); ++i) {
        if (i) { out << ','; }
        out << v[i];
    }
    return out;
}

// 閉路を１つ返す。
// 閉路がない場合は false を返す。
bool findLoopFrom(int N, const vvi& capacity, const vvi& costs, const vvi& flow, int s, vi& path)
{
    vi dists(2*N, INT_MAX / 1000);
    dists[s] = 0;
    
    bool changed = true;
    for (int k = 0; changed && k < 2*N; ++k) {
        changed = false;
        
        for (int i = 0; i < 2*N; ++i) {
            for (int j = 0; j < 2*N; ++j) {
                // i->j へ行ける？
                if (capacity[i][j] - flow[i][j] > 0) {
                    if (dists[i] + costs[i][j] < dists[j]) {
                        dists[j] = dists[i] + costs[i][j];
                        path[j] = i;
                        changed = true;
                    }
                }
                // i->j へ、j->i をもとに戻りながらいける
                if (flow[j][i] > 0) {
                    if (dists[i] - costs[j][i] < dists[j]) {
                        dists[j] = dists[i] - costs[j][i];
                        path[j] = -i - 1;
                        changed = true;
                    }
                }

                if (j == s && dists[s] < 0) {
                    return true;
                }
            }
        }
    }
    
    // 負の閉路がある場合は実行可能
    return dists[s] < 0;
}

// TODO: この関数は美しくない
bool findLoop(int N, const vvi& capacity, const vvi& costs, const vvi& flow,
              int& s, vi& path)
{
    for (s = 0; s < 2*N; ++s) {
        bool foundLoop = true;
        while (foundLoop && findLoopFrom(N, capacity, costs, flow, s, path)) {
            foundLoop = false;

            // path をたどって s から始まっているか確認......
            // TODO: する必要はないんだけど...
            vi visited(2*N, 0);
            int prev, curr = s;
            visited[curr] = true;
            while (true) {
                prev = curr;
                curr = path[prev];
                if (curr >= 0) {
                } else {
                    curr = -curr - 1;
                }
                if (visited[curr]) {
                    if (curr == s)
                        return true;
                    break;
                } else {
                    visited[curr] = true;
                }
            }
 
            if (foundLoop)
                return true;
        }
    }

    return false;
}

void maxFlow(int N, const vvi& capacity, const vvi& costs, vvi& flow)
{
    int s;
    vi path(2*N);
    while (findLoop(N, capacity, costs, flow, s, path)) {
        int curr = s;
        do {
            int prev = curr;
            curr = path[prev];
            if (curr >= 0) {
                // prev -> curr へ、flow を１つ増やす。
                flow[curr][prev] += 1;
            } else {
                curr = -curr - 1;
                // curr -> prev へ、flow を１つ減らす。
                flow[prev][curr] -= 1;
            }
        } while (curr != s);
    }
}

int solve(int N, int A, int B, int componentCount, vvi caps, const vvi& costs)
{
    int ans = 0;

    vvi flow(2*N, vi(2*N));
    for (int m = 0; m <= N; ++m) {
//        cout << "m = " << m << endl;

        // capacity を更新
        for (int i = 0; i < N; ++i) {
            caps[IN(i)][OUT(i)] = m;
        }

        maxFlow(N, caps, costs, flow);
        
        // ans の数を数える
        int totalAns = 0;
        int totalCost = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (flow[OUT(i)][IN(j)]) {
                    ++totalAns;
//                    cout << "i -> j : " << i << " -> " << j << endl;
                    totalCost += costs[OUT(i)][IN(j)];
//                    cout << 1 << ' ';
                } else {
//                    cout << 0 << ' ';
                }
            }
//            cout << endl;
        }
#if 0
        cout << "component = " << componentCount << endl;
        cout << "totalAns = " << totalAns << endl;
        cout << "totalCost = " << totalCost << endl;
        cout << "required = " << (-componentCount * LARGE_COST) << endl;
        cout << "A, B, m = " << A << "," << B << "," << m << endl;
        cout << m * B << endl;
        cout << A * totalAns << endl;
#endif
        // m <= A/B * totalAns
        if (m * B <= A * totalAns && ans < totalAns && totalCost <= -componentCount * LARGE_COST)
            ans = totalAns;
    }
    
    return ans;
}

int main(void)
{
    int caseNo = 0;
    for (int N, A, B; cin >> N >> A >> B, (N || A || B); ) {
        vvi capacities(2*N, vi(2*N));
        vvi costs(2*N, vi(2*N));

        int componentCount = 0;
        for (int i = 0; i < N; ++i) {
            string s; cin >> s;
            for (int j = 0; j < N; ++j) {
                switch (s[j]) {
                case '/':
                    break;
                case 'C':
                    costs[OUT(i)][IN(j)] = -LARGE_COST;
                    capacities[OUT(i)][IN(j)] = 1;
                    ++componentCount;
                    break;
                case '.':
                    costs[OUT(i)][IN(j)] = -1;
                    capacities[OUT(i)][IN(j)] = 1;
                    break;
                default:
                    cout << "!" << s[j] << endl;
                    abort();
                    break;
                }
            }
        }

        int ans = solve(N, A, B, componentCount, capacities, costs) - componentCount;
        cout << "Case " << ++caseNo << ": ";
        if (ans < 0)
            cout << "impossible" << endl;
        else
            cout << ans << endl;
    }
    return 0;
}
