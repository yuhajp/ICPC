#include <iostream>
#include <vector>
#include <stack>

using namespace std;
typedef vector<int> vi;
typedef vector<vi> vvi;

int paint(int h, int w, int H, int W, int o, int c, vvi& v, vvi& painted)
{
    if (v[h][w] != o)
        return 0;
    if (painted[h][w])
        return 0;

    int cnt = 1;
    v[h][w] = c;
    painted[h][w] = 1;

    if (h-1 >= 0)
        cnt += paint(h-1, w, H, W, o, c, v, painted);
    if (h+1 < H)
        cnt += paint(h+1, w, H, W, o, c, v, painted);
    if (w-1 >= 0)
        cnt += paint(h, w-1, H, W, o, c, v, painted);
    if (w+1 < W)
        cnt += paint(h, w+1, H, W, o, c, v, painted);

    return cnt;
}

int maxArea(int H, int W, const vvi& v, int c)
{
#if 0
    for (int h = 0; h < H; ++h) {
        for (int w = 0; w < W; ++w) 
            cout << v[h][w] << ' ';
        cout << endl;
    }
    cout << "--------------------" << endl;
#endif    
    vvi painted(H, vi(W));
    vvi vs(v);

    int mx = 0;
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            int m = paint(i, j, H, W, c, c, vs, painted);
            if (m > mx)
                mx = m;
        }
    }
    
    return mx;
}

int iter(int nth, int H, int W, int c, const vvi& v)
{
    if (nth == 5)
        return maxArea(H, W, v, c);

    int mx = 0;
    for (int i = 1; i <= 6; ++i) {
        if (nth == 4 && i != c) continue;
        if (i == v[0][0]) continue;
        vvi painted(H, vi(W));
        vvi vs(v);
        paint(0, 0, H, W, v[0][0], i, vs, painted);

        int m = iter(nth + 1, H, W, c, vs);
        if (m > mx)
            mx = m;
    }

    return mx;
}

int solve(int H, int W, int c, const vvi& v)
{
    return iter(0, H, W, c, v);
}

int main(void)
{
    for (int h, w, c; cin >> h >> w >> c, (h || w || c); ) {
        vvi v(h, vi(w));
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                cin >> v[i][j];
            }
        }

        cout << solve(h, w, c, v) << endl;
    }


    return 0;
}
