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

int gcd(int a, int b) {
    return b != 0 ? gcd(b, a % b) : a;
}

typedef int Vertex;
typedef int Weight;

struct Edge {
    Vertex dest;
    Weight weight;
    Edge() {}
    Edge(const Vertex& d, Weight w) : dest(d), weight(w) {}
};

typedef vector<Edge> Edges;
typedef map<Vertex, Edges> Graph;

// for MST / Dijkstra
bool operator>(const Edge& lhs, const Edge& rhs) {
    if (lhs.weight != rhs.weight) { return lhs.weight > rhs.weight; }
    return lhs.dest > rhs.dest;
}

typedef map<Vertex, Weight> Potential;

bool step(Graph& g, int u, vector<int>& matchTo, vector<bool>& visited) {
    if (u < 0) { return true; }
    
    for (Edges::iterator e = g[u].begin(); e != g[u].end(); ++e) {
        if (visited[e->dest]) { continue; }
        visited[e->dest] = true;
        if (step(g, matchTo[e->dest], matchTo, visited)) {
            matchTo[u] = e->dest;
            matchTo[e->dest] = u;
            return true;
        }
    }
    return false;
}

int bipartiteMatching(Graph& g, int L, int n, vector<pair<int, int> >& matching) {
    vector<int> matchTo(n, -1);
    int match = 0;

    for (int u = 0; u < L; ++u) {
        vector<bool> visited(n);
        if (step(g, u, matchTo, visited)) { ++match; }
    }

    for (int u = 0; u < L; ++u) {
        if (matchTo[u] >= 0) {
            matching.push_back(make_pair(u, matchTo[u]));
        }
    }
    
    return match;
}

int main(void)
{
    for (int m, n; cin >> m >> n, (m || n); ) {
        vi lefts(m), rights(n);
        for (int i = 0; i < m; ++i) { cin >> lefts[i]; }
        for (int i = 0; i < n; ++i) { cin >> rights[i]; }

        Graph g;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (gcd(lefts[i], rights[j]) > 1) {
                    g[i].push_back(Edge(j + m, 1));
                    g[j + m].push_back(Edge(i, 1));
                }
            }
        }

        vector<pair<int, int> > ans;
        cout << bipartiteMatching(g, m, m + n, ans) << endl;
    }
    return 0;
}

