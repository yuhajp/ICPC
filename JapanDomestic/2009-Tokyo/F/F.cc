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

const double EPS = 1E-9;

// ----------------------------------------------------------------------

inline
double dot(const P& p, const P& q) {
    return std::real(std::conj(p) * q);
}

inline
double cross(const P& p, const P& q) {
    return std::imag(std::conj(p) * q);
}

// true if p is in the triangle (p1, p2, p3).
bool isIn(const P& p1, const P& p2, const P& p3, const P& p)
{
    double t1 = cross(p - p1, p2 - p1);
    double t2 = cross(p - p2, p3 - p2);
    double t3 = cross(p - p3, p1 - p3);

    if (t1 >= 0 && t2 >= 0 && t3 >= 0) { return true; }
    if (t1 <= 0 && t2 <= 0 && t3 <= 0) { return true; }
    
    return false;
}

// ----------------------------------------------------------------------

void findBetterRoute(P& p1, const P& p2, const P& p3, 
                     const vector<P>& pins, vector<P>& ps)
{
    // p1, p2, p3 からなる三角形のルートで、p1 -> p3 でもっとよいルートがあれば追加する。
    // ただし、p3 への最後のルートは追加せずに p1 が更新されるのみ。

    bool found = false;

    while (true) {
        int idx = -1;
        double minTheta = M_PI;

        for (int i = 0; i < pins.size(); ++i) {
            if (!isIn(p1, p2, p3, pins[i])) { continue; }
            double theta = abs(arg((pins[i] - p1) / (p2 - p1)));
            if (theta < minTheta) {
                idx = i; minTheta = theta;
            }
        }

        if (idx < 0) {
            break;
        } else {
            // pin のぐるぐる対策で、ちょっとだけ離れた点までの segment を入れておく。
            P p2x = pins[idx] + (p2 - pins[idx]) / abs(p2 - pins[idx]) * EPS;
            ps.push_back(p2x);
            p1 = p2x;
        }
    }
}

double length(const vector<P>& ps)
{
    double answer = 0;
    for (int i = 1; i < ps.size(); ++i) {
        answer += abs(ps[i] - ps[i-1]);
    }
    return answer;
}

double solve(vector<P> ps, const vector<P>& pins)
{
#if 0
    cout << "***" << endl;
    for (int i = 0; i < pins.size(); ++i) {
        cout << pins[i].real() << ' ' << pins[i].imag() << ' ' << 1 << endl;
    }
    cout << "--------------------" << endl;
#endif

    double currentLength = length(ps);
    while (true) {
#if 0
        cout << "%%% " << endl;
        // 現在の列を表示
        for (int i = 1; i < ps.size(); ++i) {
            cout << ps[i-1].real() << ' ' << ps[i-1].imag() << ' '
                 << ps[i].real() << ' ' << ps[i].imag() << endl;
        }
        cout << "------------------------------" << endl;
#endif

        vector<P> newPs;

        P cur = ps.front();
        newPs.push_back(ps.front());

        for (int i = 1; i + 1 < ps.size(); ++i) {
            // cur, p[i], p[i+1] へのより良い行き方を考えて与える。
            // p[i+1] への直接いく point は add されない。
           findBetterRoute(cur, ps[i], ps[i+1], pins, newPs);
        }
        
        // we have to consider the last point.
        newPs.push_back(ps.back());

        double newLength = length(newPs);

        // 長さが更新されたらもう一回やる。
        if (abs(newLength - currentLength) < EPS) {
            break;
        } else {
            swap(ps, newPs);
            currentLength = newLength;
        }
    }

    return currentLength;
}

int main(void)
{
    for (int m, n; cin >> m >> n, (m || n); ) {
        vector<P> ps(m);
        vector<P> pins(n);
        for (int i = 0; i < m; ++i) {
            double x, y; cin >> x >> y;
            ps[i] = P(x, y);
        }
        for (int i = 0; i < n; ++i) {
            double x, y; cin >> x >> y;
            pins[i] = P(x, y);
        }

        double answer = solve(ps, pins);
        char buf[80];
        sprintf(buf, "%.4f", answer);
        cout << buf << endl;
    }
    return 0;
}

