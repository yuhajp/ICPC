#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <complex>

using namespace std;

const int INFTY = 1E8;

typedef complex<int> P;
typedef pair<P, bool> Vertex;  // trueIfLeft
typedef int Weight;
struct Edge {
    Vertex dest;
    Weight weight;
    Edge() {} 
    Edge(const Vertex& dest, Weight weight) : dest(dest), weight(weight) {} 
};

typedef vector<Edge> Edges; 
typedef map<Vertex, Edges> Graph;
typedef map<Vertex, Weight> Potential;

namespace std {
    bool operator<(const P& lhs, const P& rhs) {
        if (real(lhs) != real(rhs)) { return real(lhs) < real(rhs); }
        return imag(lhs) < imag(rhs);
    }
    
    bool operator>(const P& lsh, const P& rhs) {
        return rhs < lsh;
    }
}

// for MST / Dijkstra 
bool operator>(const Edge& lhs, const Edge& rhs) {
    if (lhs.weight != rhs.weight) { return lhs.weight > rhs.weight; }
    return lhs.dest > rhs.dest;
}


inline bool valid(const P& p, const vector<vector<int> >& f, int H, int W)
{
    int w = real(p), h = imag(p);
    //cout << "valid? : " << H << ' ' << W << " : " << h << ' ' << w << endl;
    return 0 <= h && h < H && 0 <= w && w < W && f[h][w] < INFTY;
}


int solve(int H, int W, const vector<vector<int> >& f,
          const vector<P>& ss, const set<P>& gs)
{
    Potential pot;
    priority_queue<Edge, Edges, greater<Edge> > Q;
    for (int i = 0; i < ss.size(); ++i) {
        Q.push(Edge(make_pair(ss[i], true), 0));
        Q.push(Edge(make_pair(ss[i], false), 0));
    }
    
    while (!Q.empty()) {
        Edge edge = Q.top(); Q.pop();
        Vertex v = edge.dest;
        Weight curr = edge.weight; 
        
        if (pot.count(v)) { continue; } // already visited.
        if (gs.count(v.first)) { return curr; } // found goal
        pot[v] = curr;
#if 0
        cout << "v(h, w) = " << v.first.imag() << ' ' << v.first.real() << ' ' << v.second
             << " -- " << curr << endl;
#endif
        for (int dw = 1; dw <= 3; ++dw) {
            for (int dh = -2; dh <= 2; ++dh) {
                if (abs(dw) + abs(dh) > 3) { continue; }
                P np = v.first + P(v.second ? dw : -dw, dh);
                if (!valid(np, f, H, W)) { continue; }
                Vertex v2(np, !v.second);
                if (pot.count(v2)) { continue; }
                Q.push(Edge(v2, curr + f[imag(np)][real(np)]));
            }
        }
    }

    return -1;
}

int main(void)
{
    for (int W, H; cin >> W >> H, (W || H); ) {
        vector<vector<int> > f(H, vector<int>(W, -1));
        vector<P> ss;
        set<P> gs;
        for (int h = 0; h < H; ++h) {
            for (int w = 0; w < W; ++w) {
                char c; cin >> c;
                if (c == 'T') {
                    f[h][w] = 0;
                    gs.insert(P(w, h));
                } else if (c == 'S') {
                    f[h][w] = 0;
                    ss.push_back(P(w, h));
                } else if (c == 'X') {
                    f[h][w] = INFTY;
                } else {
                    f[h][w] = c - '0';
                }
            }
        }

        cout << solve(H, W, f, ss, gs) << endl;
    }

    return 0;
}
