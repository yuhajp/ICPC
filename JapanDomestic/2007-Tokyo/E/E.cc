#include <iostream>
#include <vector>
#include <complex>

using namespace std;
typedef complex<double> P;
const double EPS = 1E-10;

#define NDEBUG

#ifdef NDEBUG
#define DEBUG(x)
#else
#define DEBUG(x) (x)
#endif

inline double dot(const P& p, const P& q) {
    return real(conj(p) * q);
}

inline double cross(const P& p, const P& q) {
    return imag(conj(p) * q);
}

struct Stick {
    P a, b;
    P rc; // rotation center
    Stick(double L) : a(0.0, 0.0), b(0.0, L), rc(0.0, 0.0) {}

    void rotate(double theta) {
        P r = polar(1.0, -theta);
        if (abs(rc - a) > EPS) {
            a = (a - rc) * r + rc;
        }
        if (abs(rc - b) > EPS) {
            b = (b - rc) * r + rc;
        }
    }

    void print() const {
        cout << a << b << " : " << rc << endl;
    }
};

/**
 * 点 a が 線分 s-t の上に乗っているときは true
 */
bool onSegment(P a, P s, P t)
{
    // (a - s) を (t - s) でわり、ともに x 軸上に乗っていれば online
    // さらに 0 <= x <= 1 であれば線分の上。
    P p = (a - s) / (t - s);
    return (abs(imag(p)) < EPS && 0 <= real(p) && real(p) <= 1);
}

bool lineIntersection(P As, P Ae, P Bs, P Be) { 
    DEBUG(cout << "LI : " << As << Ae << Bs << Be << endl);
    P A = Ae - As, B = Be-Bs, S = Bs-As; 
    double f = cross(A, B);
    if (abs(f) < EPS) { DEBUG(cout << "PARALLEL" << endl); return false; } // parallel
    double p = cross(S, B) / f;
    double q = cross(S, A) / f; 
    DEBUG(cout << p << " - " << q << endl);
    if (p < 0 || 1 < p) { return false; } // A が線分のとき、B と交わってない
    if (q < 0 || 1 < q) { return false; } // B が線分のとき、A と交わってない
    
    return true;
}


/**
 * 円 |z-o| = |x-o| と 線分 s-e の交点を求める。このとき、向きが考慮される。
 * 角度が 0 のときは、候補に対して少しまわしてみて、線分の上にのるかどうかをチェックすること。
 */
vector<pair<P, bool> > lineCircleIntersect(P o, P x, P s, P e)
{
    vector<pair<P, bool> > rslt;
    double r = abs(x - o);

    DEBUG(cout << "inter: " << o << x << r << s << e << endl);
    double a = norm(s - e);
    double b = dot(s - e, e - o);
    double c = norm(e - o) - r * r;
    
    double D = b * b - a * c;
    if (D < EPS) { return rslt; }

    double ks[] = {0, 1, ((-b + sqrt(D)) / a), ((-b - sqrt(D)) / a)};
    for (int i = 0; i < 4; ++i) {
        double k = ks[i];
        if (k < 0 || k > 1) { continue; }
        DEBUG(cout << "k: " << k << endl);
        P p = k * s + (1 - k) * e;
        DEBUG(cout << "r: " << (abs(p - o)) << " : " << r << endl);
        if (abs(p - o) < r + EPS) {
            P v = (o - p) * P(0, 1);
            if (cross(e - s, v) < -EPS) { rslt.push_back(make_pair(p, abs(k) < EPS || abs(k - 1) < EPS)); }
        }
    }

    return rslt;
}

bool next(pair<P, double>& rslt, const Stick& stick, int N, const vector<P>& poly)
{
    vector<pair<P, double> > cands; // candicates
    P ab[] = {stick.a, stick.b};
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < 2; ++j) {
            P e = ab[j];
            if (abs(e - stick.rc) > EPS) {
                vector<pair<P, bool> > ps = lineCircleIntersect(stick.rc, e, poly[i], poly[i + 1]);
                for (int k = 0; k < ps.size(); ++k) {
                    P p = ps[k].first;
                    double t = arg(e - stick.rc) - arg(p - stick.rc);
                    while (t < 0) { t += 2 * M_PI; }
                    while (t > 2 * M_PI) { t -= 2 * M_PI; }
                    if (abs(t - 2 * M_PI) < EPS) { t = 0; }
                    DEBUG(cout << "BP : " << p << " - " << t << endl);
                    if (abs(t) < EPS && ps[k].second) {
                        // ps[k] が true であれば (i.e. 端ならば)
                        // 少しまわしてみる。まわしたときに segment が cross するようならば候補として追加する。
                        P aa = e + (e - stick.rc) * P(0, -0.01);
                        DEBUG(cout << "AA : " << aa << endl);
                        if (lineIntersection(stick.rc, aa, poly[i], poly[i + 1])) {
                            DEBUG(cout << "CAND-1-" << j << ": " << p << " -- " << t << endl);
                            cands.push_back(make_pair(p, t));
                        }
                    } else {
                        DEBUG(cout << "CAND-2-" << j << ": " << p << " -- " << t << endl);
                        cands.push_back(make_pair(p, t));
                    }
                }
            }
        }
    }

    // theta が最も小さく、回転中心距離が最も長いものを選ぶ
    double theta = 2 * M_PI;
    P p;
    for (int i = 0; i < cands.size(); ++i) {
        double t = cands[i].second;
        // t < EPS なのは、これ以上進めない証拠
        if (t < EPS) { return false; }
        // theta がおなじぐらいなら、距離が遠い方を選ぶ
        if (abs(t - theta) < EPS) {
            if (abs(cands[i].first - stick.rc) > abs(p - stick.rc)) {
                theta = t;
                p = cands[i].first;
            }
        } else if (t < theta) {
            theta = t;
            p = cands[i].first;
        }
    }

    rslt = make_pair(p, theta);
    return true;
}


P solve(double L, double R, int N, const vector<P>& poly)
{
    Stick stick(L);
    double sumTheta = 0;
    while (sumTheta + EPS < 2 * M_PI * R) {
        DEBUG(cout << "==================== " << sumTheta << " -- " << (2 * R * M_PI) << endl);
        DEBUG(stick.print());
        
        pair<P, double> nd;
        if (!next(nd, stick, N, poly)) {
            return stick.a;
        }

        double theta = nd.second;
        if (sumTheta + theta + EPS > 2 * M_PI * R) {
            DEBUG(cout << "ENDING: " << sumTheta << ' ' << theta << " " << (2 * M_PI * R - sumTheta) << endl);
            theta = 2 * M_PI * R - sumTheta;
            stick.rotate(theta);
            return stick.a;
        }

        stick.rotate(theta);
        stick.rc = nd.first;
        sumTheta += theta;
    }
}

int main(void)
{
    int N;
    double L, R;
    while (cin >> L >> R >> N, (L || R || N)) {
        vector<P> poly(N + 1);
        for (int i = 0; i < N; ++i) {
            double x, y; cin >> x >> y;
            poly[i] = P(x, y);
        }
        poly[N] = poly[0];

        P p = solve(L, R, N, poly);
        char buf[256];
        sprintf(buf, "%.6f %.6f", real(p), imag(p));
        cout << buf << endl;
    }
}
