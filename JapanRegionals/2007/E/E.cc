#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <complex>
#include <queue>

using namespace std;
typedef complex<double> P;
typedef pair<P, P> Line;
typedef vector<pair<P, P> > Lines;

namespace std {
    bool operator<(const P& lhs, const P& rhs) {
        if (lhs.real() != rhs.real()) { return lhs.real() < rhs.real(); }
        return lhs.imag() < rhs.imag();
    }
    bool operator>(const P& lhs, const P& rhs) {
        return rhs < lhs;
    }
};

struct Edge {
    P dest;
    double w;
    P src;
    Edge() {}
    Edge(const P& dest, double w, P src = P(-1, -1)) : dest(dest), w(w), src(src) {}

    friend bool operator>(const Edge& lhs, const Edge& rhs) {
        if (lhs.w != rhs.w) { return lhs.w > rhs.w; }
        if (lhs.dest != rhs.dest) { return lhs.dest > rhs.dest; }
        return lhs.src > rhs.src;
    }
};
typedef vector<Edge> Edges;
typedef map<P, Edges> Graph;

double dijkstra(Graph& g, const P& src, const P& dst, map<P, P>& prev)
{
    map<P, double> shortest;
    priority_queue<Edge, Edges, greater<Edge> > Q;
    Q.push(Edge(src, 0, src));

    while (!Q.empty()) {
        Edge c = Q.top(); Q.pop();
        //cout << "EDGE: " << c.src << " -> " << c.dest << endl;
        if (shortest.count(c.dest)) { continue; }
        shortest[c.dest] = c.w;
        prev[c.dest] = c.src;
        //cout << "DIJK" << c.src << " -> " << c.dest << endl;
        if (c.dest == dst) { return c.w; }

        Edges& ne = g[c.dest];
        for (Edges::const_iterator it = ne.begin(); it != ne.end(); ++it) {
            //cout << "* " << it->dest << endl;
            if (!shortest.count(it->dest)) {
                Q.push(Edge(it->dest, c.w + it->w, c.dest));
            }
        }
    }

    return -1;
}


double dot(const P& p, const P& q)
{
    return real(conj(p) * q);
}

bool onLine(const P& p, const P& s, const P& e)
{
    P ps = p - s, es = e - s;
    P pses = ps / es;

    return (ps.imag() * es.real() == ps.real() * es.imag()) &&
        0 <= pses.real() && pses.real() <= 1;
}

Graph createGraph(const Lines& signs, const Lines& streets)
{
    set<P> vs;
    for (int i = 0; i < streets.size(); ++i) {
        vs.insert(streets[i].first);
        vs.insert(streets[i].second);
    }

    Graph g;

    for (int i = 0; i < streets.size(); ++i) {
        const Line& st = streets[i];
        vector<pair<double, P> > ps;
        ps.push_back(make_pair(0.0, st.first));
        for (set<P>::iterator it = vs.begin(); it != vs.end(); ++it) {
            if (st.first == *it || st.second == *it) { continue; }
            if (onLine(*it, st.first, st.second)) {
                double v = ((*it - st.first) / (st.second - st.first)).real();
                ps.push_back(make_pair(v, *it));
            }
        }
        ps.push_back(make_pair(1.0, st.second));

        sort(ps.begin(), ps.end());
#if 0
        cout << "*";
        for (int j = 0; j < ps.size(); ++j) {
            cout << ps[j].first << ' ' << ps[j].second;
        }
        cout << endl;
#endif
        //cout << "CREATING: " << endl;
        for (int j = 0; j < ps.size() - 1; ++j) {
            P s = ps[j].second;
            P e = ps[j + 1].second;
            double w = abs(e - s);
            
            bool okSE = true;
            bool okES = true;

            for (int k = 0; k < signs.size(); ++k) {
                if (onLine(signs[k].first, s, e)) {
                    if (dot(e - s, signs[k].second - signs[k].first) < 1E-6) { okES = false; }
                    if (dot(s - e, signs[k].second - signs[k].first) < 1E-6) { okSE = false; }
                }
                if (onLine(signs[k].second, s, e)) {
                    if (dot(e - s, signs[k].first - signs[k].second) < 1E-6) { okES = false; }
                    if (dot(s - e, signs[k].first - signs[k].second) < 1E-6) { okSE = false; }                    
                }
            }

            if (okSE) {
                g[s].push_back(Edge(e, w, s));
                //cout << s << " -> " << e;
            }
            if (okES) {
                g[e].push_back(Edge(s, w, e));
                //cout << e << " -> " << s;
            }
        }
        //cout << endl;
    }

    return g;
}

void separate(Lines& signs, Lines& streets, const Lines& lines)
{
    for (int i = 0; i < lines.size(); ++i) {
        P s = lines[i].first;
        P e = lines[i].second;
        bool sHasShared = false;
        bool eHasShared = false;

        for (int j = 0; j < lines.size(); ++j) {
            if (i == j) { continue; }
            if (onLine(s, lines[j].first, lines[j].second)) {
                sHasShared = true;
            }
            if (onLine(e, lines[j].first, lines[j].second)) {
                eHasShared = true;
            }
        }

        if (sHasShared && eHasShared) { streets.push_back(lines[i]); }
        else { signs.push_back(lines[i]); }
    }
}

int main(void)
{
    for (int N; cin >> N, N; ) {
        int xs, ys, xg, yg;
        cin >> xs >> ys >> xg >> yg;

        Lines lines(N);
        P s(xs, ys), g(xg, yg);

        for (int i = 0; i < N; ++i) {
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            lines[i] = make_pair(P(x1, y1), P(x2, y2));
        }

        Lines signs;
        Lines streets;
        separate(signs, streets, lines);
#if 0
        cout << "STREET: " << endl;
        for (int i = 0; i < streets.size(); ++i) {
            cout << streets[i].first << "-" << streets[i].second << endl;
        }
        cout << "SIGNS: " << endl;
        for (int i = 0; i < signs.size(); ++i) {
            cout << signs[i].first << "-" << signs[i].second << endl;            
        }
#endif
        Graph graph = createGraph(signs, streets);
#if 0
        cout << "GRAPH: " << endl;
        for (Graph::iterator it = graph.begin(); it != graph.end(); ++it) {
            cout << it->first << " -> ";
            for (Edges::iterator that = it->second.begin(); that != it ->second.end(); ++that) {
                cout << that->dest;
            }
            cout << endl;
        }
#endif
        map<P, P> prevs;
        double d = dijkstra(graph, s, g, prevs);
        if (d < 0) { 
            cout << -1 << endl;
            continue;
        }

        P cur(g);
        vector<P> answer;
        while (cur != s) {
            if (cur == prevs[cur]) { break; }            
            answer.push_back(cur);
            cur = prevs[cur];
        }
        answer.push_back(s);

        if (cur == s) {
            for (int i = answer.size() - 1; i >= 0; --i) {
                cout << answer[i].real() << ' ' << answer[i].imag() << endl;
            }
            cout << 0 << endl;
        } else {
            cout << -1 << endl;
        }
    }
}
