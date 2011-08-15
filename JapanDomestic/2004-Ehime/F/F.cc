#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <utility>

using namespace std;

typedef pair<string, string> StrPair;
typedef pair<int, int> IdPair;
typedef vector<vector<int> > Graph;

class Identifier {
public:
    Identifier() {}
    
    int id(const string& str) {
        if (mapping.count(str)) { return mapping[str]; }

        int x = revMap.size();
        mapping.insert(make_pair(str, x));
        revMap.push_back(str);
        return x;
    }
    
    const string& value(int i) const {
        return revMap[i];
    }
    
    bool isMember(const string& str) {
        return mapping.count(str);
    }
    
    int size() const { return revMap.size(); }
    
private:
    map<string, int>  mapping;
    vector<string>    revMap;
};

StrPair split(const string& str)
{
  string::size_type idx =  str.find('-');
  return make_pair(str.substr(0, idx), str.substr(idx + 1));
}

bool reachable(const Graph& g, int a, int b)
{
  return g[a][b] != -1;
}

bool isSameOrder(const Graph& g, int a, int b)
{
  bool b1 = false; // when A-C and B-C
  bool b2 = false; // when C-A and C-B
  bool b3 = false; // when C-A and B-C
  bool b4 = false; // when A-C and C-B

  for (int c = 0; c < g.size(); ++c) {
      bool ac = (g[a][c] == 1);
      bool ca = (g[c][a] == 1);
      bool bc = (g[b][c] == 1);
      bool cb = (g[c][b] == 1);
      b1 |= ac && bc;
      b2 |= ca && cb;
      b3 |= ca && bc;
      b4 |= ac && cb;
  }

  return (b1 || b2) && !b3 && !b4;
}

void floyd(Graph& g)
{
    for (int i = 0; i < g.size(); ++i) { g[i][i] = 0; }
    
    for (int k = 0; k < g.size(); ++k) {  
        for (int i = 0; i < g.size(); ++i) {
            for (int j = 0; j < g.size(); ++j) {
                if (reachable(g, i, k) && reachable(g, k, j)) {
                    if (reachable(g, i, j)) {
                        g[i][j] = min(g[i][j], g[i][k] + g[k][j]);            
                    } else {
                        g[i][j] = g[i][k] + g[k][j];
                    }
                }
            }
        }
    }
}

void createGraph(const vector<IdPair>& crossing, const Identifier& ident, Graph& g)
{
    // create an edge A -> B with cost 1 when there exists A-B in the input.
    for (int i = 0; i < crossing.size(); ++i) {
        g[crossing[i].first][crossing[i].second] = 1;
    }
    
    // create edges A <-> B with cost 0 when A is the same order as B.
    for (int i = 0; i < ident.size(); ++i) {
        for (int j = i + 1; j < ident.size(); ++j) {
            if (isSameOrder(g, i, j)) {
                g[i][j] = 0;
                g[j][i] = 0;
            }
        }
    }

    // create complete graph with Warshall-Floyd
    floyd(g);
}

int main(void)
{
    for (int N; cin >> N, N; ) {
        Identifier ident;
        vector<IdPair> crossing(N);
        
        for (int i = 0; i < N; ++i) {
            string str; cin >> str;
            StrPair s = split(str);
            crossing[i] = IdPair(ident.id(s.first), ident.id(s.second));
        }

        // create graph
        Graph g(ident.size(), vector<int>(ident.size(), -1));
        createGraph(crossing, ident, g);

        cout << ident.size() << endl;
    
        int M; cin >> M;
        for (int i = 0; i < M; ++i) {
            string str; cin >> str;
            StrPair s = split(str);
            if (ident.isMember(s.first) && ident.isMember(s.second)) {
                int a = ident.id(s.first);
                int b = ident.id(s.second);
                cout << ((reachable(g, a, b) && g[a][b] % 2 == 1) ? "YES" : "NO") << endl;
            } else {
                cout << "NO" << endl;
            }
        }
    }

    return 0;
}
