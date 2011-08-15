#include <iostream>
#include <vector>

using namespace std;

int solve(const vector<vector<int> >& t, int W, int H, int S, int T)
{
    int m = 0;
    for (int left = 0; left + S <= W; ++left) {
        for (int top = 0; top + T <= H; ++top) {
            int v = 0;
            for (int w = left; w < left + S; ++w) {
                for (int h = top; h < top + T; ++h) {
                    v += t[w][h];
                }
            }
            if (v > m) { m = v; }
        }
    }
    
    return m;
}

int main(void)
{
    for (int N; cin >> N, N; ) {
        int W, H; cin >> W >> H;
        vector<vector<int> > t(W, vector<int>(H));
        for (int i = 0; i < N; ++i) {
            int x, y; cin >> x >> y;
            t[x - 1][y - 1] = 1;
        }
        int S, T; cin >> S >> T;

        cout << solve(t, W, H, S, T) << endl;
    }
}
