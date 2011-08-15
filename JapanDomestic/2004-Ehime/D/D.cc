#include <iostream>
#include <vector>
#include <cmath>
#include <complex>

using namespace std;

typedef complex<double> P;

int inclusion(const vector<P>& ps, P center)
{
    int ret = 0;
    for (int i = 0; i < ps.size(); ++i) {
        if (abs(center - ps[i]) < 1.00001) { ++ret; }
    }
    return ret;
}

int maxInclusion(const vector<P>& ps, P x1, P x2)
{
    // the distance is more than 2.0
    if (abs(x2 - x1) > 2.0) { return 1; }

    P m = (x1 + x2) / 2.0;
    P n = P(-imag(x1 - x2), real(x1 - x2));
    n /= abs(n);

    double theta = asin(abs(x1 - m));
    double costheta = cos(theta);
    P center1 = m + costheta * n;
    P center2 = m - costheta * n;

    return max(inclusion(ps, center1), inclusion(ps, center2));
}

int solve(const vector<P>& ps)
{
    if (ps.size() == 1) { return 1; }
  
    int m = 0;
    for (int i = 0; i < ps.size(); ++i) {
        for (int j = i + 1; j < ps.size(); ++j) {
            int n = maxInclusion(ps, ps[i], ps[j]);
            if (n > m) { m = n; }
        }
    }

    return m;
}

int main(void)
{
    for (int N; cin >> N, N; ) {
        vector<P> ps(N);
        for (int i = 0; i < N; ++i) {
            double x, y; cin >> x >> y;
            ps[i] = P(x, y);
        }
        cout << solve(ps) << endl;
    }

    return 0;
}
