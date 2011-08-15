#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <queue>
#include <complex>

using namespace std;
typedef complex<double> P;
const double EPS = 1E-10;

struct Robot {
    string nickname;
    vector<P> vs; // 時刻 t ~ t + 1 の間の速度

    P curP; // current position
    bool info;
    set<int> inRange;

    Robot() : info(false) {}

    void addVelocity(int t, const P& v) {
        while (vs.size() < t) {
            vs.push_back(v);
        }
    }

    friend bool operator<(const Robot& lhs, const Robot& rhs) {
        return lhs.nickname < rhs.nickname;
    }
};

struct Event {
    double t;
    int r1, r2;
    bool isEnter; 

    Event(double t, int r1, int r2, bool isEnter) :
        t(t), r1(r1), r2(r2), isEnter(isEnter)
    {
    }

    // t が同じになることはないと書かれてあるので t で sort すれば OK
    friend bool operator<(const Event& lhs, const Event& rhs) {
        return lhs.t < rhs.t;
    }
};

// rs[r] が通信可能な robot の index を全て返す
set<int> closures(vector<Robot>& rs, int r)
{
    set<int> rslt;
    queue<int> Q;
    Q.push(r);

    while (!Q.empty()) {
        int i = Q.front(); Q.pop();
        if (rslt.count(i)) { continue; }

        rslt.insert(i);
        const set<int>& rb = rs[i].inRange;
        for (set<int>::const_iterator it = rb.begin(); it != rb.end(); ++it) {
            Q.push(*it);
        }
    }

    return rslt;
}

inline
double dot(const P& p, const P& q)
{
    return real(conj(p) * q);
}

vector<Event> makeEvents(int N, int T, int R, vector<Robot>& rs)
{
    vector<Event> es;

    // t = 0 のときに交わっているかどうか
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            double r = abs(rs[i].curP - rs[j].curP);
            if (r < R) { 
                //cout << "in @ " << 0 << " : " << i << " - " << j << endl;
                es.push_back(Event(0, i, j, true));
            }
        }
    }

    for (int t0 = 0; t0 < T; ++t0) {
        for (int i = 0; i < N; ++i) {
            for (int j = i + 1; j < N; ++j) {
                // P(t) = rs[i].curP + (t - t0) * rs[i].vs[t0];
                // Q(t) = rs[j].curP + (t - t0) * rs[j].vs[t0];
                // P(t) - Q(t) = 
                //  rs[i].curP + (t - t0) * rs[i].vs[t0]
                // -rs[j].curP - (t - t0) * rs[j].vs[t0]
                // = (rs[i].vs[t0] - rs[j].vs[t0]) * t
                //    + (rs[i].curP - rs[j].curP) - (rs[i].vs[t0] - rs[j].vs[t0]) * t0
                P alpha = rs[i].vs[t0] - rs[j].vs[t0];
                P beta = rs[i].curP - rs[j].curP - double(t0) * alpha;
                // P(t) - Q(t) = αt + β
                // |P(t) - Q(t)| = R となる t を求める。
                // R^2 = α^2t^2 + 2(α, β)t + β^2
                // α^2 t^2 + 2αβt + β^2 - R^2 = 0
                // α^2 >= 0 なのでグラフは下に凸。
                double a2 = norm(alpha);
                double b2 = norm(beta);
                double ab = dot(alpha, beta);
                double D = ab * ab - a2 * (b2 - R * R);
                if (D < EPS) { continue; } // 交わることはない
                // t = (-αβ +- sqrt(D)) / α^2
                double t1 = (-ab + sqrt(D)) / a2;
                double t2 = (-ab - sqrt(D)) / a2;
                if (t0 <= t1 && t1 < t0 + 1) {
                    //cout << "out @ " << t1 << " : " << i << " - " << j << endl;
                    es.push_back(Event(t1, i, j, false));
                }
                if (t0 <= t2 && t2 < t0 + 1) {
                    //cout << "in @ " << t2 << " : " << i << " - " << j << endl;
                    es.push_back(Event(t2, i, j, true));
                }
            }
        }

        for (int i = 0; i < N; ++i) {
            rs[i].curP += rs[i].vs[t0];
        }
    }
    
    return es;
}

int main(void)
{
    for (int N, T, R; cin >> N >> T >> R, (N || T || R); ) {
        vector<Robot> rs(N);
        for (int i = 0; i < N; ++i) {
            cin >> rs[i].nickname;
            int t, vx, vy; cin >> t >> vx >> vy;
            rs[i].curP = P(vx, vy);
            while (true) {
                cin >> t >> vx >> vy;
                rs[i].addVelocity(t, P(vx, vy));
                if (t == T) { break; }
            }
        }

        rs[0].info = true;

        // nickname で sort
        sort(rs.begin(), rs.end());

        // event を全部列挙して event の発生時刻で sort
        vector<Event> es = makeEvents(N, T, R, rs);
        sort(es.begin(), es.end());

        // 情報伝播
        for (int i = 0; i < es.size(); ++i) {
            if (es[i].isEnter) {
                rs[es[i].r1].inRange.insert(es[i].r2);
                rs[es[i].r2].inRange.insert(es[i].r1);
                // 情報伝播: 
                set<int> c = closures(rs, es[i].r1);
                // 誰かが知っているか？
                bool info = false;
                for (set<int>::iterator it = c.begin(); it != c.end(); ++it) {
                    info |= rs[*it].info;
                }
                // 全員に伝える。
                for (set<int>::iterator it = c.begin(); it != c.end(); ++it) {
                    rs[*it].info |= info;
                }
            } else {
                rs[es[i].r1].inRange.erase(es[i].r2);
                rs[es[i].r2].inRange.erase(es[i].r1);
            }
        }

        for (int i = 0; i < rs.size(); ++i) {
            if (rs[i].info) { cout << rs[i].nickname << endl; }
        }
    }
}
