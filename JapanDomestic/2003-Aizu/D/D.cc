#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <cmath>

using namespace std;

// ----------------------------------------------------------------------
// Common Definition for Graph Library

typedef int Vertex;
typedef double Weight;
struct Edge {
    Vertex src;
    Vertex dest;
    Weight weight;
    Edge() {} 
    Edge(const Vertex& s, const Vertex& d, Weight w) : 
        src(s), dest(d) , weight(w)
    {
    } 
};

typedef vector<Edge> Edges; 
typedef map<Vertex, Edges> Graph; 
typedef map<Vertex, Weight> Potential;
 
// for MST / Dijkstra 
bool operator>(const Edge& lhs, const Edge& rhs) {
  if (lhs.weight != rhs.weight) { return lhs.weight > rhs.weight; }
  if (lhs.dest != rhs.dest) { return lhs.dest > rhs.dest; }
  return lhs.src > rhs.src;
} 

//----------------------------------------------------------------------

Weight prim(Graph &g)
{ 
    priority_queue<Edge, Edges, greater<Edge> > Q;
    set<Vertex> visited;
    Weight mstLength = 0; 
    
    Vertex first = g.begin()->first;
    Q.push(Edge(first, first, 0));
 
    while (!Q.empty()) {
        Edge top = Q.top();
        Q.pop();
        Vertex& newV = top.dest;
        if (visited.count(newV)) { continue; }

        mstLength += top.weight;
        visited.insert(newV);
        
        Edges& es = g[newV];
        for (int i = 0; i < es.size(); ++i){ 
            Edge& e = es[i]; 
            if (!visited.count(e.dest)) {
                Q.push(e);
            }
        } 
    } 

    return mstLength; 
} 

struct V {
    double x, y, z, r;
};

int main(void)
{
    for (int N; cin >> N, N;) {
        vector<V> vs(N);
        for (int i = 0; i < N; ++i) {
            cin >> vs[i].x >> vs[i].y >> vs[i].z >> vs[i].r;
        }

        Graph g;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < i; ++j) {
                double x = vs[i].x - vs[j].x, y = vs[i].y - vs[j].y, z = vs[i].z - vs[j].z;
                double d = sqrt(x * x + y * y + z * z) - vs[i].r - vs[j].r;
                if (d < 0) { d = 0; }
                g[i].push_back(Edge(i, j, d));
                g[j].push_back(Edge(j, i, d));
            }
        }

        double rslt = prim(g);
        char buf[256];
        sprintf(buf, "%.3f", rslt);
        cout << buf << endl;
    }

    return 0;
}
