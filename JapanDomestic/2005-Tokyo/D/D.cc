// dijkstra で書き直し。
#include <iostream>
#include <vector>
#include <map>
#include <queue>

using namespace std;
const double INFTY = 1E10;

// ----------------------------------------------------------------------
// Common Definition for Graph Library

typedef pair<int, int> Vertex;
typedef double Weight;
struct Edge {
  Vertex dest;
  Weight weight;
  Edge() {} 
  Edge(const Vertex& d, Weight w) : dest(d) , weight(w) {} 
};

typedef vector<Edge> Edges; 
typedef map<Vertex, Edges> Graph; 
typedef map<Vertex, Weight> Potential;
 
// for MST / Dijkstra 
bool operator>(const Edge& lhs, const Edge& rhs) {
  if (lhs.weight != rhs.weight) { return lhs.weight > rhs.weight; }
  return lhs.dest > rhs.dest;
} 

/**
 * ダイクストラのアルゴリズム。
 * Graph g を startV から出発したとき、Vertex から最短距離への map を返す。
 * 到達不能な Vertex は返り値の map には入っていない。
 *
 * Complexity: --
 * Verified with: --
 *
 * @param g グラフ
 * @param startV スタート地点
 * @return 最短距離
 */
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


double solve(int start, int goal, int n, int m, const vector<vector<double> >& roads, const vector<double>& tickets)
{
    // graph の頂点は、<駅番号, 残ったチケット> で表す。
    // 「残ったチケット」は、bit で表す。
    Graph g;

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            // 道がない場合は進めない。
            if (roads[i][j] < 0) { continue; }
            // i -> j に、ticket t で進む。
            for (int t = 0; t < n; ++t) {
                // tks を今持っている。
                for (int tks = 0; tks < (1 << n); ++tks) {
                    // tks に t がない場合は進めない。
                    if (!(tks & (1 << t))) { continue; }
                    Vertex vs = make_pair(i, tks);
                    Vertex ve = make_pair(j, tks & ~(1 << t));
                    g[vs].push_back(Edge(ve, roads[i][j] / tickets[t]));
                    // cout << i << " -> " << j << " : " << t << " : " << tks << " : " << (roads[i][j] / tickets[t]) << endl;
                }
            }
        }
    }

    Vertex startV = make_pair(start, (1 << n) - 1);
    Potential pot = dijkstra(g, startV);

    double minLen = INFTY;
    for (int tks = 0; tks < (1 << n); ++tks) {
        Vertex v(goal, tks);
        if (pot.count(v)) { minLen = min(minLen, pot[v]); }
    }
    return minLen;
}

int main(void)
{
    int n, m, p, a, b;
    while (cin >> n >> m >> p >> a >> b, n || m || p || a || b) {
        vector<double> tickets(n);
        for (int i = 0; i < n; ++i) { cin >> tickets[i]; }

        vector<vector<double> > roads(m, vector<double>(m, -1));
        for (int i = 0; i < p; ++i) {
            int x, y, z;
            cin >> x >> y >> z;
            --x, --y;
            roads[x][y] = roads[y][x] = z;
        }

        double answer = solve(a - 1, b - 1, n, m, roads, tickets);
        if (answer >= INFTY) {
            cout << "Impossible" << endl;
        } else {
            cout << answer << endl;
        }
    }

    return 0;
}
