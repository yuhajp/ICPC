#include <iostream>
#include <vector>
#include <complex>

using namespace std;
typedef complex<int> P;

const P d[] = { P(0, 1), P(0, -1), P(1, 0), P(-1, 0)};

inline bool inRect(const P& p, const vector<vector<bool> >& t, int H, int W)
{
    return (0 <= p.real() && p.real() < W && 0 <= p.imag() && p.imag() < H);
}

inline bool valid(const P& p, const vector<vector<bool> >& t, int H, int W)
{
    return (inRect(p, t, H, W) && !t[p.imag()][p.real()]);
}

int solve(int ith, int H, int W, const vector<vector<bool> >& tt, const P& s, const P& g)
{
    if (ith > 10) { return -1; }

    int minIth = -1;
    for (int i = 0; i < 4; ++i) {
        vector<vector<bool> > t(tt);
        P p = s;
        p += d[i];
        if (!valid(p, t, H, W)) { continue; }

        while(valid(p, t, H, W) && p != g) {
            p += d[i];
        } 

        if (p == g) {
            if (minIth == -1) { minIth = ith; }
            else if (ith < minIth) { minIth = ith; }
        } else if (inRect(p, t, H, W)) {
            t[p.imag()][p.real()] = false;
            int x = solve(ith + 1, H, W, t, p - d[i], g);
            if (minIth == -1) { minIth = x; }
            else if (x != -1 && x < minIth) { minIth = x; }
        }
    }

    return minIth;
}

int main(void)
{
    for (int W, H; cin >> W >> H, (W || H); ) {
        vector<vector<bool> > t(H, vector<bool>(W));
        P s, g;
        for (int h = 0; h < H; ++h) {
            for (int w = 0; w < W; ++w) {
                int x; cin >> x;
                switch (x) {
                case 0:
                    break;
                case 1:
                    t[h][w] = true;
                    break;
                case 2:
                    s = P(w, h);
                    break;
                case 3:
                    g = P(w, h);
                    break;
                default:
                    abort();
                    // shouldn't happen
                }
            }
        }

        cout << solve(1, H, W, t, s, g) << endl;
    }
}
