#include <iostream>
#include <cstdio>
#include <vector>
#include <complex>
#include <algorithm>
#include <functional>
#include <list>

using namespace std;

typedef complex<double> P;
const double PI = 4 * atan(1.0); // or use M_PI.
const double EPS = 1E-8;

struct comparator : std::binary_function<P, P, bool>
{
    P p;
    P dir;

    comparator(const P& p, const P& dir) : p(p), dir(dir) {}

    bool operator()(const P& p1, const P& p2)
    {
        double d1 = this->arg(p1);
        double d2 = this->arg(p2);
        if (abs(d1 - d2) < EPS) {
            return abs(p1 - p) < abs(p2 - p);
        } else {
            return d1 < d2;
        }
    }

    double arg(const P& p1) {
        P q = p1 - p;
        double d = std::arg(dir / q);
        while (d < 0) { d += 2 * PI; }
        return d;
    }
};


double solve(const vector<P>& points)
{
    list<P> ps(points.begin(), points.end());
    P p = P(0, 0);
    P dir = P(0, 1);
    double sum = 0.0;
    
    while (!ps.empty()) {
        list<P>::iterator it = min_element(ps.begin(), ps.end(), comparator(p, dir));
        if (it == ps.end()) { abort(); } // shouldn't happen
        sum += abs(*it - p);
        dir = (*it - p) / abs(*it - p); // normalize for safety (maybe meaningless)
        p = *it;
        ps.erase(it);
    }

    return sum;
}


int main(void)
{
    for (int N; cin >> N, N; ) {
        vector<P> ps;
        ps.reserve(N);
        for (int i = 0; i < N; ++i) {
            int x, y; cin >> x >> y;
            ps.push_back(P(x, y));
        }

        double d = solve(ps);
        char buf[256];
        sprintf(buf, "%.1f", d);
        cout << buf << endl;
    }

    return 0;
}
