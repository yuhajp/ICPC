#include <vector>
#include <utility>
#include <iostream>

using namespace std;

const int LOSE = 1;
const int BACK = 2;

double solve(int N, int T, const vector<int>& f)
{
    // pair is (not turn-losing, turn-losing)
    vector<vector<pair<double, double> > > possibility(T + 1, vector< pair<double, double> >(N + 1));
    possibility[0][0] = make_pair(1.0, 0.0);
                                              
    for (int t = 1; t <= T; ++t) {
        for (int pos = 0; pos < N; ++pos) {
            possibility[t][pos].first += possibility[t - 1][pos].second;
            for (int dice = 1; dice <= 6; ++dice) {
                int p;
                if (pos + dice <= N) { p = pos + dice; }
                else { p = N - (pos + dice - N); }
                if (f[p] == BACK) { p = 0; }
                
                if (f[p] == LOSE) {
                    possibility[t][p].second += possibility[t - 1][pos].first / 6;
                } else {
                    possibility[t][p].first += possibility[t - 1][pos].first / 6;
                }
            }
        }
    }

    double sum = 0;
    for (int t = 0; t <= T; ++t) {
        sum += possibility[t][N].first;
    }
    return sum;
}

int main(void)
{
    int N, T, L, B;
    while ((cin >> N >> T >> L >> B), (N || T || L || B)) {
        vector<int> field(N + 1);
        for (int i = 0; i < L; ++i) { 
            int l; cin >> l; field[l] = LOSE;
        }
        for (int i = 0; i < B; ++i) {
            int b; cin >> b; field[b] = BACK;
        }
        
        double answer = solve(N, T, field);
        char buf[256];
        sprintf(buf, "%.6f", answer);
        cout << buf << endl;
    }

    return 0;
}
