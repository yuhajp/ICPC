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

int dx[] = { 0,  1, 1, 1, 0, -1, -1, -1};
int dy[] = {-1, -1, 0, 1, 1,  1,  0, -1};

bool go(int h, int w, int H, int W, const vvi& f, vvi& visited)
{
    visited[h][w] = 1;
    for (int d = 0; d < 8; ++d) {
        int ww = w + dx[d];
        int hh = h + dy[d];
        if (ww < 0 || W <= ww || hh < 0 || H <= hh) { continue; }
        if (visited[hh][ww]) { continue; }
        if (!f[hh][ww]) { continue; }
        visited[hh][ww] = 1;
        go(hh, ww, H, W, f, visited);
    }
}

int solve(int H, int W, vvi f)
{
    int result = 0;
    vvi visited(H, vi(W)); //
    for (int h = 0; h < H; ++h) {
        for (int w = 0; w < W; ++w) {
            if (visited[h][w]) { continue; }
            if (!f[h][w]) { continue; }
            go(h, w, H, W, f, visited);
            ++result;
        }
    }

    return result;
}

int main(void)
{
    for (int w, h; cin >> w >> h, (w || h); ) {
        vvi f(h, vi(w));
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                cin >> f[i][j];
            }
        }
        cout << solve(h, w, f) << endl;
    }
    return 0;
}

