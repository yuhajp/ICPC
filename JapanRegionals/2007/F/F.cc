#include <iostream>
#include <vector>

using namespace std;

struct Edge {
    int a, b;
    int w;

    Edge(int a = 0, int b = 0, int w = 0) : a(a), b(b), w(w) {}

    friend bool operator<(const Edge& lhs, const Edge& rhs) {
        if (lhs.w != rhs.w) { return lhs.w < rhs.w; }
        if (lhs.a != rhs.a) { return lhs.a < rhs.a; }
        return lhs.b < rhs.b;
    }
};

struct UnionFind {
    vector<int> v;
    int cardinal;
    
    UnionFind(int n) : v(n), cardinal(n) {
        for (int i = 0; i < n; ++i) { v[i] = i; }
    }
    
    int find(int x) { 
        if (v[x] == x) { return x; }
        else { return v[x] = find(v[x]); }
    }

    int unify(int x, int y) {
        int xx = find(x);
        int yy = find(y);
        if (xx < yy) { --cardinal; v[yy] = xx; }
        else if (yy < xx) { --cardinal; v[xx] = yy; }
    }
};

int solve(int n, const vector<Edge>& edges)
{
    vector<int> vs(n);
    int minSlimness = 100000;

    int mini = 0;
    
    while (true) {
        UnionFind uf(n);
        int minw = edges[mini].w;
        int i = mini;

        while (uf.cardinal > 1 && i < edges.size()) {
            uf.unify(edges[i].a, edges[i].b);
            ++i;
        }
        
        if (uf.cardinal > 1) { return minSlimness == 100000 ? -1 : minSlimness; }
        int slimness = edges[i - 1].w - minw;
        minSlimness = std::min(minSlimness, slimness);

        while (edges[mini].w == minw) { ++mini; }
    }
}

int main(void)
{
    for (int n, m; cin >> n >> m, (n || m); ) {
        vector<Edge> edges(m);
        for (int i = 0; i < m; ++i) {
            cin >> edges[i].a >> edges[i].b >> edges[i].w;
            --edges[i].a;
            --edges[i].b;
        }
        sort(edges.begin(), edges.end());

        cout << solve(n, edges) << endl;
    }
}
