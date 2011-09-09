#include <iostream>
#include <vector>
#include <complex>

using namespace std;
typedef complex<double> P;
const double EPS = 1E-8;

double cross(const P& p, const P& q)
{
    return imag(conj(p) * q);
}

double dot(const P& p, const P& q)
{
    return real(conj(p) * q);
}

// B is line. A is segment.
bool intersect(P& ps, const P& As, const P& Ae, const P& Bs, const P& Be)
{
    P A = Ae - As, B = Be - Bs, S = Bs - As;
    double f = cross(A, B);
    if (-EPS <= f && f <= EPS) { return false; }
    double p = cross(S, B) / f;
    if (p < 0 - EPS || 1 + EPS < p) { return false; }
    ps = As + p * A;
#ifndef NDEBUG   
    cout << "IS " << A << ":" << B << ":" << S << endl;
    cout << f << ":" << p << ":" << ps << endl;
#endif
    return true;
}

// ps is convex.
void clip(vector<P>& ps, const P& s, const P& e)
{
    if (ps.empty()) { return; }

    const int N = ps.size();
    vector<P> ans;
    ans.reserve(ps.size());
    P p;
    
    for (int i = 0; i < N; ++i) {
        if (intersect(p, ps[i], ps[(i + 1) % N], s, e)) {
            if (cross(e - s, ps[i] - s) > 0) {
                ans.push_back(ps[i]);
                ans.push_back(p);
            } else {
                ans.push_back(p);
            }
        } else {
            if (cross(e - s, ps[i] - s) > 0) {
                ans.push_back(ps[i]);
            }
        }
    }


    swap(ans, ps);
}

double solve(const vector<P>& pts)
{
    const P img(0, 1);
    const int N = pts.size();
        
    double left = 0.0, right = 10000;
   
    while (right - left > 0.000001) {
        vector<P> ps(pts);
        double mid = (left + right) * 0.5;
        
        for (int i = 0; i < N; ++i) {
            const P& p0 = pts[i];
            const P& p1 = pts[(i + 1) % N];
            P p10 = p1 - p0;
            P n = (p10 * img) * (mid / abs(p10));
            clip(ps, p0 + n, p1 + n);

#ifndef NDEBUG
            cout << "CLIP: " << (p0 + n) << "-" << (p1 + n) << ":" << mid << endl;
            for (int i = 0; i < ps.size(); ++i) {
                cout << ps[i] << endl;
            }
#endif
            if (ps.size() < 3) { break; }
        }

        if (ps.size() < 3) {
            right = mid;
        } else {
            left = mid;
        }
    }

    return right;
}

int main(void)
{
    for (int N; cin >> N, N; ) {
        vector<P> pts(N);
        for (int i = 0; i < N; ++i) {
            int x, y; cin >> x >> y;
            pts[i] = P(x, y);
        }

        double d = solve(pts);
        char buf[256];
        sprintf(buf, "%.6f", d);
        cout << buf << endl;
    }
}
