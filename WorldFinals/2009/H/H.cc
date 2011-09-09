#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <utility>
#include <map>
#include <set>
#include <queue>
#include <complex>
#include <cmath>
#include <cassert>

using namespace std;
typedef complex<double> P;

void reachability(vector<vector<int> >& r)
{
    int N = r.size();

    for (int i = 0; i < N; ++i) {
        vector<bool> visited(N);
        queue<int> Q;

        Q.push(i); visited[i] = true;

        while (!Q.empty()) {
            int x = Q.front(); Q.pop();
            for (int j = 0; j < N; ++j) {
                if (r[x][j] && !visited[j]) { 
                    Q.push(j); visited[j] = true;
                    r[i][j] = true;
                }
            }
        }
    }
}

int main(void)
{
    int caseNo = 0;
    
    for (int B, M; cin >> B >> M, (B || M); ) {
        
        vector<vector<int> > r(2 * B, vector<int>(2 * B)); // r[2]: positive, r[2i + 1]: negative

        for (int i = 0; i < M; ++i) {
            int k; cin >> k;
            vector<pair<int, char> > vs(k);
            for (int j = 0; j < k; ++j) {
                cin >> vs[j].first >> vs[j].second;
            }

            if (k == 1) {
                r[2 * (vs[0].first - 1) + (vs[0].second == 'y')][2 * (vs[0].first - 1) + (vs[0].second != 'y')] = true;
            } else if (k == 2) {
                r[2 * (vs[0].first - 1) + (vs[0].second == 'y')][2 * (vs[0].first - 1) + (vs[0].second != 'y')] = true;
                r[2 * (vs[1].first - 1) + (vs[1].second == 'y')][2 * (vs[1].first - 1) + (vs[1].second != 'y')] = true;
            } else if (k == 3) {
                r[2 * (vs[0].first - 1) + (vs[0].second == 'y')][2 * (vs[1].first - 1) + (vs[1].second != 'y')] = true;
                r[2 * (vs[0].first - 1) + (vs[0].second == 'y')][2 * (vs[2].first - 1) + (vs[2].second != 'y')] = true;
                r[2 * (vs[1].first - 1) + (vs[1].second == 'y')][2 * (vs[2].first - 1) + (vs[2].second != 'y')] = true;

                r[2 * (vs[1].first - 1) + (vs[1].second == 'y')][2 * (vs[0].first - 1) + (vs[0].second != 'y')] = true;
                r[2 * (vs[2].first - 1) + (vs[2].second == 'y')][2 * (vs[0].first - 1) + (vs[0].second != 'y')] = true;
                r[2 * (vs[2].first - 1) + (vs[2].second == 'y')][2 * (vs[1].first - 1) + (vs[1].second != 'y')] = true;
            } else if (k == 4) {
                r[2 * (vs[0].first - 1) + (vs[0].second == 'y')][2 * (vs[1].first - 1) + (vs[1].second != 'y')] = true;
                r[2 * (vs[0].first - 1) + (vs[0].second == 'y')][2 * (vs[2].first - 1) + (vs[2].second != 'y')] = true;
                r[2 * (vs[0].first - 1) + (vs[0].second == 'y')][2 * (vs[3].first - 1) + (vs[3].second != 'y')] = true;
                r[2 * (vs[1].first - 1) + (vs[1].second == 'y')][2 * (vs[2].first - 1) + (vs[2].second != 'y')] = true;
                r[2 * (vs[1].first - 1) + (vs[1].second == 'y')][2 * (vs[3].first - 1) + (vs[3].second != 'y')] = true;
                r[2 * (vs[2].first - 1) + (vs[2].second == 'y')][2 * (vs[3].first - 1) + (vs[3].second != 'y')] = true;

                r[2 * (vs[1].first - 1) + (vs[1].second == 'y')][2 * (vs[0].first - 1) + (vs[0].second != 'y')] = true;
                r[2 * (vs[2].first - 1) + (vs[2].second == 'y')][2 * (vs[0].first - 1) + (vs[0].second != 'y')] = true;
                r[2 * (vs[3].first - 1) + (vs[3].second == 'y')][2 * (vs[0].first - 1) + (vs[0].second != 'y')] = true;
                r[2 * (vs[2].first - 1) + (vs[2].second == 'y')][2 * (vs[1].first - 1) + (vs[1].second != 'y')] = true;
                r[2 * (vs[3].first - 1) + (vs[3].second == 'y')][2 * (vs[1].first - 1) + (vs[1].second != 'y')] = true;
                r[2 * (vs[3].first - 1) + (vs[3].second == 'y')][2 * (vs[2].first - 1) + (vs[2].second != 'y')] = true;
            }
        }

        cout << "Case " << ++caseNo << ": ";
        
        reachability(r);

        bool impossible = false;
        for (int i = 0; i < B; ++i) {
            if (r[2*i][2*i+1] && r[2*i+1][2*i]) { impossible = true; break; }
        }

        if (impossible) {
            cout << "impossible" << endl;
        } else {
            for (int i = 0; i < B; ++i) {
                if (r[2*i][2*i+1]) { cout << 'n'; }
                else if (r[2*i+1][2*i]) { cout << 'y'; }
                else { cout << '?'; }
            }
            cout << endl;
        }
    }


    return 0;
}

