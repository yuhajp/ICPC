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

struct Vertex {
    int city;
    int v;
    int prevCity;
    Vertex() {}
    Vertex(int city, int v, int prevCity) : city(city), v(v), prevCity(prevCity) {}
    
    friend bool operator<(const Vertex& lhs, const Vertex& rhs) {
        if (lhs.v != rhs.v) { return lhs.v < rhs.v; }
        if (lhs.city != rhs.city) { return lhs.city < rhs.city; }
        return lhs.prevCity < rhs.prevCity;
    }

    friend bool operator>(const Vertex& lhs, const Vertex& rhs) {
        return rhs < lhs;
    }
};
typedef double Weight;

struct Edge {
    Vertex dest;
    Weight weight;
    Edge() {}
    Edge(const Vertex& d, Weight w) : dest(d), weight(w) {}
};

typedef vector<Edge> Edges;
typedef map<Vertex, Edges> Graph;
typedef map<Vertex, Weight> Potential;

bool operator>(const Edge& lhs, const Edge& rhs) {
    if (lhs.weight != rhs.weight) { return lhs.weight > rhs.weight; }
    return lhs.dest > rhs.dest;
}

Potential dijkstra(Graph& g, const Vertex& startV) {
    Potential pot;
    priority_queue<Edge, Edges, greater<Edge> > Q;

    Q.push(Edge(startV, 0));

    while (!Q.empty()) {
        Edge edge = Q.top(); Q.pop();
        Vertex p = edge.dest;
        Weight curr = edge.weight;

        if (pot.count(p)) { continue; } // already visited.
        pot[p] = curr;

        Edges& es = g[p];
        for (int i = 0; i < es.size(); ++i) {
            Edge& e = es[i];
            if (pot.count(e.dest)) { continue; } // already visited.
            Q.push(Edge(e.dest, curr + e.weight));
        }
    }

    return pot;
}

int main(void)
{
    for (int n, m; cin >> n >> m, (n || m); ) {
        Graph g;
        int start, goal; cin >> start >> goal;
        for (int i = 0; i < m; ++i) {
            int x, y, d, c; cin >> x >> y >> d >> c;
            for (int v = 0; v <= c + 1; ++v) {
                for (int prevCity = 0; prevCity <= n; ++prevCity) { // 0 は start を表す架空の Node 
                    if (0 < v && v <= c) {
                        if (prevCity != y) {
                            g[Vertex(x, v, prevCity)].push_back(Edge(Vertex(y, v, x), (double)d / v));
                        }
                        if (prevCity != x) {
                            g[Vertex(y, v, prevCity)].push_back(Edge(Vertex(x, v, y), (double)d / v));
                        }
                    }
                    if (0 < v + 1 && v + 1 <= c) {
                        if (prevCity != y) {
                            g[Vertex(x, v, prevCity)].push_back(Edge(Vertex(y, v + 1, x), (double)d / (v + 1)));
                        }
                        if (prevCity != x) {
                            g[Vertex(y, v, prevCity)].push_back(Edge(Vertex(x, v + 1, y), (double)d / (v + 1)));
                        }
                    }
                    if (0 < v - 1 && v - 1 <= c) {
                        if (prevCity != y) {
                            g[Vertex(x, v, prevCity)].push_back(Edge(Vertex(y, v - 1, x), (double)d / (v - 1)));
                        }
                        if (prevCity != x) {
                            g[Vertex(y, v, prevCity)].push_back(Edge(Vertex(x, v - 1, y), (double)d / (v - 1)));
                        }
                    }
                }    
            }
        }

        double result = 1E+9;
        Potential pot = dijkstra(g, Vertex(start, 0, 0));
        for (int p = 0; p <= n; ++p) {
            if (pot.count(Vertex(goal, 1, p))) {
                result = result < pot[Vertex(goal, 1, p)] ? result : pot[Vertex(goal, 1, p)];
            }
        }

        if (result >= 1E+9) {
            cout << "unreachable" << endl;
        } else {
            char buf[256];
            sprintf(buf, "%.4f", result);
            cout << buf << endl;
        }
    }
    
    return 0;
}

