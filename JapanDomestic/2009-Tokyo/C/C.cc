#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <algorithm>
#include <numeric>
#include <utility>
#include <complex>
#include <cmath>
#include <cassert>

using namespace std;
typedef complex<double> P;
typedef vector<int> vi;
typedef vector<vector<int> > vvi;

void iter(int& result, int pos, 
          const vi& m, const vector<string>& ss, 
          vi& assoc, vi& visited)
{
    if (pos == assoc.size()) {
#if 0
        for (int i = 0; i < assoc.size(); ++i) {
            cout << assoc[i] << ' ';
        }
        cout << endl;
#endif
        vi vs(ss.size());
        for (int i = 0; i < ss.size(); ++i) {
            for (int j = 0; j < ss[i].length(); ++j) {
                vs[i] = vs[i] * 10 + assoc[m[ss[i][j]]];
            }
        }
        if (vs.back() == accumulate(vs.begin(), vs.end() - 1, 0)) {
            ++result;
        }
        return;
    } else {
        for (int k = 0; k <= 9; ++k) {
            if (visited[k]) { continue; }
            if (k == 0) {
                for (int i = 0; i < ss.size(); ++i) {
                    if (ss[i].length() >= 2 && m[ss[i][0]] == pos) {
                        goto NEXT_LOOP;
                    }
                }
            }
            visited[k] = true;
            assoc[pos] = k;
            iter(result, pos + 1, m, ss, assoc, visited);
            assoc[pos] = -1;
            visited[k] = false;
        NEXT_LOOP:
            ;
        }
    }
}

int solve(const vector<string>& ss)
{
    vi m(256, -1);
    int p = 0;
    for (int i = 0; i < ss.size(); ++i) {
        for (int j = 0; j < ss[i].length(); ++j) {
            if (m[ss[i][j]] < 0) { 
                m[ss[i][j]] = p++;
            }
        }
    }

    vi assoc(p, -1);
    vi visited(10, 0);
    int result = 0;
    iter(result, 0, m, ss, assoc, visited);
    return result;
}

int main(void)
{
    for (int n; cin >> n, n; ) {
        vector<string> ss(n);
        for (int i = 0; i < n; ++i) { cin >> ss[i]; }
        cout << solve(ss) << endl;
    }
    return 0;
}
