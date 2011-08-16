#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>

using namespace std;
typedef complex<double> P;
const double EPS = 1E-12;

namespace std {
    bool operator<(const P& lhs, const P& rhs) {
        if (lhs.imag() != rhs.imag()) { return lhs.imag() < rhs.imag(); }
        return lhs.real() < rhs.imag();
    }
}

struct Line {
    P s, e;

    Line(const P& s, const P& e) : s(s), e(e) {}
};


vector<Line> commonTangents(const P& o1, double r1, const P& o2, double r2)
{
    double a = abs(o2 - o1);
    if (a < EPS) { return vector<Line>(); }

    vector<Line> rslt;
    P rot = (o2 - o1) / a;
    
    double xs[] = {r1 * (r1 + r2) / a, r1 * (r1 - r2) / a};
    for (int i = 0; i < 2; ++i) {
        double x1 = xs[i];
        if (x1 < -r1 || r1 < x1) { continue; }

        double ys[] = {sqrt(r1 * r1 - x1 * x1), -sqrt(r1 * r1 - x1 * x1)};
        for (int j = 0; j < 2; ++j) {
            double y1 = ys[j];
            // これは接点が１つしかない場合。y1 が 0 になる。
            if (abs(y1) < EPS) {
                rslt.push_back(Line(P(x1, 0), P(x1, 1)));
                break;
            }
            
            double x2 = (a * y1 * y1 + x1 * r1 * r1) / (r1 * r1);
            if (x2 < a - r2 || a + r2 < x2) { continue; }
            double y2 = (r1 * r1 - x1 * x2) / y1;
            P p1 = P(x1, y1) * rot + o1;
            P p2 = P(x2, y2) * rot + o1;
            rslt.push_back(Line(p1, p2));
        }
    }

    return rslt;
}


vector<double> makeEdges(const vector<P>& cs)
{
    int N = cs.size();
    vector<double> ts;

    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            vector<Line> lns = commonTangents(cs[i], 1, cs[j], 1);
            for (int k = 0; k < lns.size(); ++k) {
                double t = -arg(lns[k].e - lns[k].s);
                // 運が悪いと t の負号だけが残る！　のでこの if 文を
                // 入れておく。M_PI 近いときも 0 にする。
                if (abs(t) < EPS) { t = 0; }
                while (t < 0) { t += M_PI; }
                ts.push_back(t);
                // cout << "theta: " << t << " : " << tan(t) << endl;
            }
        }
    }

    return ts;
}

void update(double v, double t, 
            double& minV, double& minT, double& maxV, double& maxT)
{
    if (v > maxV) { maxV = v; maxT = t; }
    if (v < minV) { minV = v; minT = t; }
    
    // この２行は問題文により不要。ただ見た目が悪いので追加。
    if (abs(minT - M_PI) < EPS) { minT = 0; }
    if (abs(maxT - M_PI) < EPS) { maxT = 0; }
}

pair<double, double> solve(const vector<P>& cs)
{
    int N = cs.size();
    vector<double> thetas = makeEdges(cs);
    thetas.push_back(0);
    thetas.push_back(M_PI);
    sort(thetas.begin(), thetas.end());

#if 0
    for (int i = 0; i < thetas.size(); ++i) {
        cout << thetas[i] << endl;
    }
#endif
    double minT, minV = 1e+10;
    double maxT, maxV = 0;

    // thetas[i] と thetas[i + 1] の間
    for (int i = 0; i + 1 < thetas.size(); ++i) {
        if (thetas[i + 1] - thetas[i] < EPS) { continue; }
        //cout << "-----" << thetas[i] << "-----" << thetas[i + 1] << "-----" << endl;
        vector<P> ts(N);
        // 中間の theta で回転。その場合の重なり具合を調べる。
        double mtheta = (thetas[i] + thetas[i + 1]) / 2;
        for (int j = 0; j < N; ++j) {
            ts[j] = cs[j] * polar(1.0, mtheta);
        }
        sort(ts.begin(), ts.end());
        // 重なっている円に対して、f を y 軸への正射影で得られる範囲とすると
        // z1 を重なっている円の最初 z2 を最後として、
        // θは回転角、δは原点とz1を結ぶ直線と、z1 と z2 を結ぶ直線のなす角
        // f(θ) は f(θ) = |z2 - z1| * sin(θ + δ) + 2
        // |z2 - z1| * (sinθcosδ + cosθsinδ) + 2
        int idx = 0;
        double a = 0, b = 0, c = 0;
        while (idx < N) {
            int lidx = idx;
            while (idx + 1 < N && imag(ts[idx]) + 1 > imag(ts[idx + 1]) - 1) {
                ++idx;
            }
            int ridx = idx;
            double delta = arg(ts[ridx] - ts[lidx]) - mtheta;
#if 0
            cout << "lidx, ridx : " << lidx << ", " << ridx << endl
                 << "mtheta     : " << mtheta << endl
                 << "delta      : " << delta << endl
                 << "cos, sin   : " << cos(delta) << " -- " << sin(delta)
                 << endl;
#endif
            double r = abs(ts[ridx] - ts[lidx]);
            a += r * cos(delta); b += r * sin(delta); c += 2;
            ++idx;
        }
#if 0
        cout << "a = " << a << ", b = " << b << ", c = " << c << endl;
#endif
        // f(θ) = a sinθ + b cosθ + c で今表されている。
        // df/dθ = 0 となるθに対して、f(θ) を求め、最大最小を求める。
        // df/dt = a cos t - b sin t = 0
        // a cos t = b sin t
        // a/b = tan t
        // t = atan (a/b)
        double t = atan(a / b);
        if (t < 0) { t += M_PI; }

        // check ts[idx], ts[idx + 1], and t
        {
            double d = a * sin(thetas[i]) + b * cos(thetas[i]) + c;
//            cout << "d1 = " << d << endl;
            update(d, thetas[i], minV, minT, maxV, maxT);
            d = a * sin(thetas[i + 1]) + b * cos(thetas[i + 1]) + c;
//            cout << "d2 = " << d << endl;
            update(d, thetas[i + 1], minV, minT, maxV, maxT);
        }
        if (thetas[i] < t && t < thetas[i + 1]) {
            double d = a * sin(t) + b * cos(t) + c;
//            cout << "d3 = " << d << endl;
            update(d, t, minV, minT, maxV, maxT);
        }
    }

    return make_pair(minT, maxT);
}

int main(void)
{
    for (int N; cin >> N, N; ) {
        vector<P> cs(N);
        for (int i = 0; i < N; ++i) {
            int x, y; cin >> x >> y;
            cs[i] = P(x, y);
        }

        pair<double, double> rslt = solve(cs);
        char buf[256];
        sprintf(buf, "%.15f", rslt.first);
        cout << buf << endl;
        sprintf(buf, "%.15f", rslt.second);
        cout << buf << endl;
    }
}
