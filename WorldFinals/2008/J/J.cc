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

using namespace std;
typedef complex<double> P;
const double EPS = 1E-10;

// 内積
inline double dot(const P& p, const P& q) {
    return std::real(std::conj(p) * q);
}

// 外積
inline double cross(const P& p, const P& q) {
    return std::imag(std::conj(p) * q);
}

// 線分 As-Ae と線分 Bs-Be が交わっているかどうかチェック。交わっていれば true を返し、crossed にその座標を入れる。
bool lineIntersection(P As, P Ae, P Bs, P Be, P& crossed) { 
    P A = Ae - As, B = Be-Bs, S = Bs-As; 
    double f = cross(A, B);
    if (abs(f) < EPS) { return false; } // parallel
    double p = cross(S, B) / f;
    double q = cross(S, A) / f; 
    if (p < 0 || 1 < p) { return false; } // A が線分のとき、B と交わってない
    if (q < 0 || 1 < q) { return false; } // B が線分のとき、A と交わってない
    
    crossed = Bs + q * B; // or, As + p*A 
    return true;
}

double solve(const vector<pair<P, P> >& lines)
{
    // 線分を区間に分ける。eventPoints は、線分の開始、終了、交差の点。
    vector<double> eventPoints;
    
    for (int i = 0; i < lines.size(); ++i) {
        eventPoints.push_back(real(lines[i].first));
        eventPoints.push_back(real(lines[i].second));
        
        for (int j = i + 1; j < lines.size(); ++j) {
            P crossed;
            if (lineIntersection(lines[i].first, lines[i].second, lines[j].first, lines[j].second, crossed)) {
                eventPoints.push_back(real(crossed));
            }
        }
    }
    sort(eventPoints.begin(), eventPoints.end());

#if 0
    cout << "----- begin of e.p. -----" << endl;
    for (int i = 0; i < eventPoints.size(); ++i) {
        cout << eventPoints[i] << endl;
    }
    cout << "----- end of e.p. -----" << endl;
#endif
    
    // ここまでで eventpoint によって区間を作ることが出来た。
    // ただし、同じ x 座標のものがいくつもある場合があるので、区間が非常に短い場合の区間は無視している。

    double result = 0.0;
    
    double lastEp = eventPoints.front() - 1;
    // 各区間についてループ
    for (int i = 0; i < eventPoints.size(); ++i) {
        double ep = eventPoints[i];
        if (abs(lastEp - ep) < EPS) { continue; }                
        // cout << "*** EVENT : " << lastEp << " - " << ep << endl;
        
        double midV = -1.0; // 

        // 区間中にある線分のうち、一番上にあるものを得る。
        // その時の長さは len に入れておく。
        double len = 0.0;
        for (int j = 0; j < lines.size(); ++j) {
            const P& left = lines[j].first;
            const P& right = lines[j].second;
            // cout << "CHECKING : " << left << ' ' << right << endl;
            if (real(left) - EPS <= lastEp && lastEp <= real(right) + EPS &&
                real(left) - EPS <= ep && ep <= real(right) + EPS)
            {
                double mid = (lastEp + ep) / 2.0;
                double k = imag(right - left) / real(right - left);
                double y = k * (mid - real(left)) + imag(left);
                // cout << "FOUND V : " << y << endl;
                if (y > midV) {
                    double leftY = k * (lastEp - real(left)) + imag(left);
                    double rightY = k * (ep - real(left)) + imag(left);
                    double diffY = rightY - leftY;
                    double diffX = ep - lastEp;
                    len = sqrt(diffY * diffY + diffX * diffX);
                    // cout << P(lastEp, leftY) << "-" << P(ep, rightY) << " LEN " << len << endl;
                    midV = y;
                }
            }
        }

        // event point を更新。すなわち次の区間へ。また長さも加える。
        lastEp = ep;
        result += len;
//        cout << "CURRENT RESULT : " << result << endl;
    }
    
    
    return result;
}

int main(void)
{
    int num = 0;
    for (int N; cin >> N, N; ) {
        // 山を読み込んで線分に変換する。
        vector<pair<P, P> > lines(2 * N);
        for (int i = 0; i < N; ++i) {
            double X, H, B; cin >> X >> H >> B;
            lines[2 * i] = make_pair(P(X - B / 2, 0), P(X, H));
            lines[2 * i + 1] = make_pair(P(X, H), P(X + B / 2, 0));
        }

        double len = solve(lines);
        
        cout << "Case " << (++num) << ": " << int(len + 0.5) << endl;
        cout << endl;
    }
    return 0;
}

