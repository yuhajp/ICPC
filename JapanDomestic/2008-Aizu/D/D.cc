#include <iostream>
#include <vector>
#include <map>
#include <queue>

using namespace std;
typedef vector<int> vectori;
typedef vector<vectori> vectorvi;
typedef int point;
typedef int weight;
typedef map<point, int> potential;

struct edge {
  point dest;
  weight w;
  edge() {}
  edge(const point& dest, weight w) : dest(dest), w(w) {}

  friend bool operator>(const edge& lhs, const edge& rhs) {
    return lhs.w > rhs.w;
  }
};
typedef vector<edge> edges;
typedef map<point, edges> graph;

int dw[] = { 0,  1, 0, -1 };
int dh[] = { -1, 0, 1,  0 };

int make_point(int H, int W, int h, int w, int d)
{
  if (h < 0 || H <= h) { return -1; }
  if (w < 0 || W <= w) { return -1; }
  if (d < 0 || 4 <= d) { return -1; }
  return ((h * W + w) << 2) + d;
}

graph create_graph(int W, int H, const vectorvi& f, int c0, int c1, int c2, int c3)
{
  graph g;
  for (int h = 0; h < H; ++h) {
    for (int w = 0; w < W; ++w) {
      for (int d = 0; d < 4; ++d) {
	point p = make_point(H, W, h, w, d);
	// straight
	g[p].push_back(edge(make_point(H, W, h + dh[d], w + dw[d], d), 
			    f[h][w] == 0 ? 0 : c0));
	// turn right
	int rd = (d + 1) % 4;
	g[p].push_back(edge(make_point(H, W, h + dh[rd], w + dw[rd], rd),
			    f[h][w] == 1 ? 0 : c1));
	// turn back
	int bd = (d + 2) % 4;
	g[p].push_back(edge(make_point(H, W, h + dh[bd], w + dw[bd], bd),
			    f[h][w] == 2 ? 0 : c2));
	// turn left
	int ld = (d + 3) % 4;
	g[p].push_back(edge(make_point(H, W, h + dh[ld], w + dw[ld], ld),
			    f[h][w] == 3 ? 0 : c3));
	// stop
      }
    }
  }

  return g;
}

int solve(int W, int H, const vectorvi& f, int c0, int c1, int c2, int c3)
{
  graph g = create_graph(W, H, f, c0, c1, c2, c3);
  point st = make_point(H, W, 0, 0, 1);
  point goals[] = {
    make_point(H, W, H - 1, W - 1, 0),
    make_point(H, W, H - 1, W - 1, 1),
    make_point(H, W, H - 1, W - 1, 2),
    make_point(H, W, H - 1, W - 1, 3),
  };

  potential pot;
  priority_queue<edge, edges, greater<edge> > Q;
  Q.push(edge(st, 0));

  while (!Q.empty()) {
    edge top = Q.top(); Q.pop();
    point p = top.dest;
    int curr = top.w;

    for (int j = 0; j < 4; ++j) {
      if (p == goals[j]) { return curr; }
    }
    
    if (pot.count(p) != 0) { continue; }
    pot[p] = curr;
    edges& es = g[p];
    for (int i = 0; i < es.size(); ++i) {
      edge& e = es[i];
      if (pot.count(e.dest) != 0) { continue; }
      Q.push(edge(e.dest, curr + e.w));
    }
  }

  return -1;
}

int main(void)
{
  for (int W, H; cin >> W >> H, (W || H); ) {
    vectorvi f(H, vectori(W));
    for (int h = 0; h < H; ++h) {
      for (int w = 0; w < W; ++w) {
	cin >> f[h][w];
      }
    }
    int c0, c1, c2, c3;
    cin >> c0 >> c1 >> c2 >> c3;

    cout << solve(W, H, f, c0, c1, c2, c3) << endl;
  }
  return 0;
}
