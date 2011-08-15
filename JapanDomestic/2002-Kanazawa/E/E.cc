#include <iostream>
#include <vector>
#include <complex>
#include <cmath>

using namespace std;
typedef complex<double> P;
#ifdef NDEBUG
#define DEBUG(x)
#else
#define DEBUG(x) (x)
#endif

const double INFTY = 1E+10;
const double EPS = 1E-10;

// 接線
struct Edge {
    double sTheta; // (-π, π]
    int eIdx; // 円 index
    double eTheta;
    mutable bool used; // 同じ接線に来たら終了

    Edge(double sTheta, int eIdx, double eTheta) :
        sTheta(sTheta), eIdx(eIdx), eTheta(eTheta), used(false) {}

    friend bool operator<(const Edge& lhs, const Edge& rhs) {
        if (lhs.sTheta != rhs.sTheta) { return lhs.sTheta < rhs.sTheta; }
        if (lhs.eIdx != rhs.eIdx) { return lhs.eIdx < rhs.eIdx; }
        return lhs.eTheta < rhs.eTheta;
    }
};

struct Circle {
    P o;
    double r;
    vector<Edge> es; // theta で sort されているとする。

    Circle(const P& o, double r) : o(o), r(r), es() {}

    double right() const {
        return o.real() + r;
    }

    double up() const {
        return o.imag() + r;
    }

    // es で、theta から反時計回りに進んでいったとき、始めて出会う接線を返す。
    // そのようなものがない場合 NULL を返す。
    // left は exact theta になりうる。right を exact theta にしてしまうと
    // 使った接線を返してしまう。
    const Edge* next(double theta) const {
        //cout << "NEXT: " << theta << endl;
        for (int i = 0; i < es.size(); ++i) {
            int ii = (i + es.size() - 1) % es.size();
            double left = es[ii].sTheta;
            double right = es[i].sTheta;
            if (left <= right && left <= theta && theta < right) {
                return &es[i];
            } else if (right < left && (left <= theta || theta < right)) {
                return &es[i];
            }
        }

        // 見つからなかった
        return NULL;
    }
};

// c1 が c2 に包含されている
bool isCircleIncluded(const Circle& c1, const Circle& c2)
{
    return abs(c2.o - c1.o) < c2.r - c1.r + EPS;
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
            // この接線が欲しい場合は、(x1, 1) あたりをもう一つの
            // 端点とする必要がある。
            if (abs(y1) < EPS) { continue; }
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


void makeEdges(vector<Circle>& cs)
{
    for (int i = 0; i < cs.size(); ++i) {
        for (int j = i + 1; j < cs.size(); ++j) {
            vector<Line> ct = commonTangents(cs[i].o, cs[i].r, cs[j].o, cs[j].r);
            for (int k = 0; k < ct.size(); ++k) {
                double sTheta = arg(ct[k].s - cs[i].o);
                double eTheta = arg(ct[k].e - cs[j].o);
                // 境界は怖いので M_PI に統一しておく。
                if (abs(sTheta + M_PI) < EPS) { sTheta = M_PI; }
                if (abs(eTheta + M_PI) < EPS) { eTheta = M_PI; }
                cs[i].es.push_back(Edge(sTheta, j, eTheta));
                cs[j].es.push_back(Edge(eTheta, i, sTheta));
            }
        }
    }


    for (int i = 0; i < cs.size(); ++i) {
        sort(cs[i].es.begin(), cs[i].es.end());
        // もし同じ程度の theta がある場合、一番遠いところまでいけるようにする。
        // 距離が同じぐらいであればより大きな円に行く (inclusion の cut をしている
        // ので実際には起こらないはず？)
        // 
        vector<Edge>& es = cs[i].es;
        int j = 0;
        while (j + 1 < es.size()) {
            Edge& e1 = es[j];
            Edge& e2 = es[j + 1];
            if (abs(e1.sTheta - e2.sTheta) > EPS) { ++j; continue; }

            P p = cs[i].o + polar(cs[i].r + e1.sTheta);
            P p1 = cs[e1.eIdx].o + polar(cs[e1.eIdx].r, e1.eTheta);
            P p2 = cs[e2.eIdx].o + polar(cs[e2.eIdx].r, e2.eTheta);
            if (abs(p1 - p2) < EPS) {
                if (cs[e1.eIdx].r > cs[e2.eIdx].r) {
                    es.erase(es.begin() + j + 1);
                } else {
                    es.erase(es.begin() + j);
                }
                cout << p  << ' ' << i << ' ' << j << ' ' << es.size() << endl;
                cout << p1 << ' ' << e1.eIdx << endl;
                cout << p2 << ' ' << e2.eIdx << endl;
            } else if (abs(p1 - p) < abs(p2 - p)) {
                es.erase(es.begin() + j);
            } else {
                es.erase(es.begin() + j + 1);
            }
        }
    }

#if 0
    for (int i = 0; i < cs.size(); ++i) {
        cout << "*** " << i << endl;
        for (int j = 0; j < cs[i].es.size(); ++j) {
            Edge e = cs[i].es[j];
            cout << e.sTheta << ' ' << e.eIdx << ' ' << e.eTheta << endl;
        }
        cout << endl;
    }
#endif
}

double solve(const vector<Circle>& cs)
{
    // はじめの circle を探す。
    double maxX = -INFTY, maxY = -INFTY, stIdx = -1;
    for (int i = 0; i < cs.size(); ++i) {
        if (abs(cs[i].right() - maxX) < EPS && cs[i].up() > maxY) {
            maxX = cs[i].right();
            maxY = cs[i].up();
            stIdx = i;
        } else if (cs[i].right() > maxX) {
            maxX = cs[i].right();
            maxY = cs[i].up();
            stIdx = i;
        }
    }

    if (stIdx < 0) { abort(); } // shouldn't happen

    // theta = 0 から始めて、rope を emulate する。
    // はれるものがない場合は円周を返して終わる。
    const Edge* pEdge = cs[stIdx].next(EPS);
    if (pEdge == NULL) {
        return 2 * M_PI * cs[stIdx].r;
    }

    double len = 0;
    int idx = stIdx;
#if 0
        cout << "bT" << (cs[idx].o + polar(cs[idx].r, pEdge->sTheta)) << endl;
#endif
    
    while (!pEdge->used) {
        // 今は pEdge のはじめにいる。
        pEdge->used = true;

        P s = cs[idx].o + polar(cs[idx].r, pEdge->sTheta);
        P e = cs[pEdge->eIdx].o + polar(cs[pEdge->eIdx].r, pEdge->eTheta);
        len += abs(e - s); // 接線の分

#if 0
        cout << "aT : " << e << endl;
#endif

        // 次を探す。
        const Edge* nextEdge = cs[pEdge->eIdx].next(pEdge->eTheta);
#if 0
        cout << "aR: " << (cs[pEdge->eIdx].o + polar(cs[pEdge->eIdx].r, nextEdge->sTheta)) << endl;
#endif

        // 円の分の距離を足す。
        double th = nextEdge->sTheta - pEdge->eTheta;
        while (th < 0 - EPS) { th += 2 * M_PI; }
        len += cs[pEdge->eIdx].r * th; // 円の分

        idx = pEdge->eIdx;
        pEdge = nextEdge;
    }

    return len;
}

int main(void)
{
    for (int N; cin >> N, N; ) {
        vector<Circle> cs;
        for (int i = 0; i < N; ++i) {
            double x, y, r; cin >> x >> y >> r;
            cs.push_back(Circle(P(x, y), r));
        }

        // 完全に含まれている円があり、またそれが外円に接しているとまずい。
        // そういうのは先に消去しておく。
        int i = 0;
        while (i < cs.size()) {
            for (int j = 0; j < cs.size(); ++j) {
                if (i == j) { continue; }
                if (isCircleIncluded(cs[i], cs[j])) {
                    cs.erase(cs.begin() + i);
                    goto NEXT;
                }
            }
            ++i;
        NEXT:
            ;
        }

        makeEdges(cs);
        double d = solve(cs);
        char buf[256];
        sprintf(buf, "%.5f", d);
        cout << buf << endl;
    }
}
