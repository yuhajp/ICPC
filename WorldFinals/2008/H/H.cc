// author: mayah

#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <utility>
#include <map>
#include <set>
#include <queue>
#include <complex>
#include <cmath>
#include <cassert>
#include <stdio.h>

using namespace std;
typedef complex<double> Pd;
const double EPS = 1E-10;

#define NDEBUG

namespace std {
    inline
    bool operator<(const Pd& lhs, const Pd& rhs) {
        if (real(lhs) != real(rhs)) { return real(lhs) < real(rhs); }
        return imag(lhs) < imag(rhs);
    }
}

// 三角形を表す型。もともとはいろいろ情報を持っていたが諸般の事情により
// level だけをもつようになってしまった。別に要らない。
struct Triangle {
    int level;
    
    Triangle() : level(-1) {}    
};

// 線分を表す。
struct Segment {
    Triangle* tri; // この線分はどの三角形に属している?
    Pd s, e; // 開始点と終了点
    double k;  // 線分の傾き。計算高速かのためのキャッシュ。
    bool higher; // この線分が属する三角形のなかで上にある線分であれば true
    bool higherCalced; // higher を計算済みであれば true。false のとき higher の値は未定義。

    Segment() {}
    
    Segment(Triangle* tri, const Pd& s, const Pd& e) : tri(tri), s(s < e ? s : e), e(s < e ? e : s) {
        higherCalced = false;
        Pd es = this->e - this->s;
        k = imag(es) / real(es);
    }

    // x 座標に対応する線分の y 座標。範囲外のときは Assertion Failed になるはず。
    double y(double x) const {
        if (real(e) == real(s)) { return imag(s); }
        assert(real(s) <= x && x <= real(e));
        
        return k * (x - real(s)) + imag(s);
    }

};

// 線分を並び替えるための比較関数。開始点と終了点の順にチェック。より左、より下にある方が小さいことになっている。
struct LessLeftPoint : public binary_function<Segment, Segment, bool> {
    bool operator()(const Segment& lhs, const Segment& rhs) {
        if (real(lhs.s) != real(rhs.s)) { return real(lhs.s) < real(rhs.s); }
        if (imag(lhs.s) != imag(rhs.s)) { return imag(lhs.s) < imag(rhs.s); }
        if (real(lhs.e) != real(rhs.e)) { return real(lhs.e) < real(rhs.e); }
        
        return imag(lhs.e) < imag(rhs.e);
    }
};

// 線分を set に入れておくときのソーター。途中で今考えている x 座標を使う為に double x を持つ。この
// x のあたいは随時変更される。x を non-static で保持しておくとコピーされて途中で変更できなくなるので注意。
struct SegmentSorter : public binary_function<Segment*, Segment*, bool> {
    static double x;

    SegmentSorter() {}

    // y 座標でまずソート。判断できないときは より左下に線分の開始点があるものが先になる。
    bool operator()(const Segment* pLhs, const Segment* pRhs) const {
        // if (pLhs == pRhs) { return false; }
        const Segment& lhs = *pLhs;
        const Segment& rhs = *pRhs;

        double x = SegmentSorter::x;
        double lhsY = lhs.y(x);
        double rhsY = rhs.y(x);
        if (abs(lhsY - rhsY) > EPS) { return lhsY < rhsY; }
        // if (pLhs == pRhs) { return false; }
        if (real(lhs.s) == real(rhs.s)) {
            int x = min(real(lhs.e), real(rhs.e));
            double lhsY = lhs.y(x);
            double rhsY = rhs.y(x);
            if (abs(lhsY - rhsY) > EPS) { return lhsY < rhsY; }     
        } else if (real(lhs.e) == real(rhs.e)) {
            int x = max(real(lhs.s), real(rhs.s));
            double lhsY = lhs.y(x);
            double rhsY = rhs.y(x);
            if (abs(lhsY - rhsY) > EPS) { return lhsY < rhsY; }                        
        }
        // return pLhs < pRhs;
        return LessLeftPoint()(lhs, rhs);
    }
};
double SegmentSorter::x;

inline
double cross(const Pd& p1, const Pd& p2)
{
    return imag(conj(p1) * p2);
}

bool lineIntersection(const Segment& lhs, const Segment& rhs) {
    if (lhs.tri == rhs.tri) { return false; }

    Pd A = lhs.e - lhs.s, B = rhs.e - rhs.s, S = rhs.s - lhs.s;
    double f = cross(A, B);
    if (abs(f) < EPS) { // parallel
        Pd Bs = S / A;
        if (abs(imag(Bs)) > EPS) { return false; }        
        Pd Be = (rhs.e - lhs.s) / A;
        if (real(Bs) < -EPS && real(Be) < -EPS) { return false; }
        if (1 + EPS < real(Bs) && 1 + EPS < real(Be)) { return false; }
        return true;
    } 
    double p = cross(S, B) / f;
    double q = cross(S, A) / f;
    
    if (p < 0 - EPS || 1 + EPS < p) { return false; } // A が線分のとき、B と交わってない
    if (q < 0 - EPS || 1 + EPS < q) { return false; } // B が線分のとき、A と交わってない

    return true;
}

// 線分 lhs が 線分 rhs よりも上にあるときに true。lhs が rhs よりも上にあるの定義は、
// lhs の端点を除く任意の点をとったときその y 座標が rhs の同じ x 座標の点よりも上にあること。
// もし rhs に比較できる点がなければ lhs は上にある、と定義される。lhs が rhs よりも上にあり、
// さらに rhs が lhs よりも上にあることもある。（定義はあんまりよくないね。)
// たとえば、どちらかが y 軸と平行なときは常に true になる。
bool isHigher(const Segment& lhs, const Segment& rhs)
{
    if (real(lhs.s) == real(lhs.e)) { return true; }
    if (real(rhs.s) == real(rhs.e)) { return true; }
    if (real(lhs.s) == real(rhs.e)) { return true; }
    if (real(lhs.e) == real(rhs.s)) { return true; }
    
    int left  = max(real(lhs.s), real(rhs.s));
    int right = min(real(lhs.e), real(rhs.e));
    double mid   = (left + right) / 2.0;
    assert(real(lhs.s) <= mid && mid <= real(lhs.e));
    assert(real(rhs.s) <= mid && mid <= real(rhs.e));

    // cout << "mid = " << mid << ' ' << lhs.y(mid) << ' ' << rhs.y(mid) << endl;
    if (lhs.y(mid) >= rhs.y(mid)) { return true; }
    else { return false; }
}

// vector だと速度が稼げなかったので global 配列にしている。普段は vector でとるべき。
Segment gsegs[300000];
Triangle gts[100000];
pair<pair<int, bool>, const Segment*> ges[600000];

// plain sweep のアルゴリズムを用いる。この部分の詳細は解説を読んでほしい。
bool plainsweep(int segNum, const Segment* segs)
{
    for (int i = 0; i < segNum; ++i) {
        ges[2*i] = make_pair(make_pair(real(segs[i].s), false), segs + i);
        ges[2*i+1] = make_pair(make_pair(real(segs[i].e), true), segs + i);
    }
    sort(ges, ges + segNum * 2);

//    return true;
    
    typedef set<const Segment*, SegmentSorter> SegmentSet;
    SegmentSet t;

    for (int i = 0; i < segNum * 2; ++i) {
        const Segment& seg = *ges[i].second;
        if (!ges[i].first.second) {
            // left point
            SegmentSorter::x = real(seg.s);

            pair<SegmentSet::iterator, bool> pr = t.insert(&seg);
            if (!pr.second) { return true; }
            SegmentSet::iterator it = pr.first;
            assert (it != t.end());
            
            SegmentSet::iterator upper = it;
            if (upper != t.end() && (*upper)->tri == (*it)->tri) {
                ++upper;
            }
            if (upper != t.end()) {
                if (lineIntersection(**it, **upper)) { return true; }
            }

            SegmentSet::iterator lower = it;
            if (lower != t.begin() && (*lower)->tri == (*it)->tri) {
                --lower;
            }
            if ((*lower)->tri == (*it)->tri) { lower = t.end(); }
            if (lower != t.end()) {
                if (lineIntersection(**it, **lower)) { return true; }
            }

            if ((*it)->tri->level < 0) {
                if (lower != t.end()) {
                    assert((*it)->tri != (*lower)->tri);
                    assert((*lower)->tri->level >= 0);
                    if (!(*lower)->higherCalced) {
                        int gsegsIdx = *lower - gsegs;
                        switch (gsegsIdx % 3) {
                        case 0:
                            gsegs[gsegsIdx].higher = 
                                isHigher(gsegs[gsegsIdx], gsegs[gsegsIdx+1]) && 
                                isHigher(gsegs[gsegsIdx], gsegs[gsegsIdx+2]);
                            gsegs[gsegsIdx].higherCalced = true;
                            break;
                        case 1:
                            gsegs[gsegsIdx].higher = 
                                isHigher(gsegs[gsegsIdx], gsegs[gsegsIdx+1]) && 
                                isHigher(gsegs[gsegsIdx], gsegs[gsegsIdx-1]);
                            gsegs[gsegsIdx].higherCalced = true;
                            break;
                        case 2:
                            gsegs[gsegsIdx].higher = 
                                isHigher(gsegs[gsegsIdx], gsegs[gsegsIdx-1]) && 
                                isHigher(gsegs[gsegsIdx], gsegs[gsegsIdx-2]);
                            gsegs[gsegsIdx].higherCalced = true;
                            break;
                        default:
                            assert(false);
                        }
                    }
                    if ((*lower)->higher) {
                        (*it)->tri->level = (*lower)->tri->level;
                    } else {
                        (*it)->tri->level = (*lower)->tri->level + 1;                        
                    }
                } else {
                    (*it)->tri->level = 2;
                }
            }
        } else {
            // right point
            SegmentSorter::x = real(seg.e);
            SegmentSet::iterator it = t.find(&seg);
            assert(it != t.end());
            assert((*it)->tri == seg.tri);

            SegmentSet::iterator upper = it;
            if (upper != t.end() && (*upper)->tri == (*it)->tri) {
                ++upper;
            }
            SegmentSet::iterator lower = it;
            if (lower != t.begin() && (*lower)->tri == (*it)->tri) {
                --lower;
            }
            if ((*lower)->tri == (*it)->tri) { lower = t.end(); }
            
            if (upper != t.end() && lower != t.end()) {
                if (lineIntersection(**upper, **lower)) {
                    return true;
                }
            }
            t.erase(it);
        }
    }

    return false;
}



int main(void)
{
    int caseNo = 0;
    for (int N; scanf("%d", &N), N >= 0; ) {
        for (int i = 0; i < N; ++i) {
            int x1, y1, x2, y2, x3, y3;
            scanf("%d%d%d%d%d%d", &x1, &y1, &x2, &y2, &x3, &y3);
            Pd p0(x1, y1), p1(x2, y2), p2(x3, y3);
            gts[i].level = -1;

            gsegs[i*3]   = Segment(gts + i, p0, p1);
            gsegs[i*3+1] = Segment(gts + i, p1, p2);
            gsegs[i*3+2] = Segment(gts + i, p2, p0);
        }


        int answer = 1;            
        if (plainsweep(3*N, gsegs)) {
            answer = -1;
        } else if (N > 0) {
            for (int i = 0; i < N; ++i) {
                answer = max(answer, gts[i].level);
            }
        }

        if (answer < 0) { printf("Case %d: ERROR\n", ++caseNo); }
        else { printf("Case %d: %d shades\n", ++caseNo, answer); }
    }
    return 0;
}

