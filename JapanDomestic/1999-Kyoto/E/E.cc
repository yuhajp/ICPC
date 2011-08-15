#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Edge {
    int no;
    int t, d;

    Edge(int no, int t, int d) : no(no), t(t), d(d) {}

    friend bool operator<(const Edge& lhs, const Edge& rhs) {
        if (lhs.t != rhs.t) { return lhs.t < rhs.t; }
        if (lhs.d != rhs.d) { return lhs.d < rhs.d; }
        return lhs.no < rhs.no;
    }
};


int iter(int s, const vector<vector<Edge> >& edges,  
         int curD, vector<int>& curPath, vector<bool>& used, 
         int& maxD, vector<int>& answer)
{
    const vector<Edge>& es = edges[s];
    for (int i = 0; i < es.size(); ++i) {
        const Edge& e = es[i];
        if (used[e.no]) { continue; }
        used[e.no] = true;
        curPath.push_back(e.t);
        iter(e.t, edges, curD + e.d, curPath, used, maxD, answer);
        curPath.pop_back();
        used[e.no] = false;
    }

    if (curD > maxD) {
        maxD = curD;
        answer = curPath;
    }
}

int solve(int N, int L, const vector<vector<Edge> >& edges, vector<int>& answer)
{
    int maxD = 0;
    
    for (int i = 1; i <= N; ++i) {
        vector<bool> used(L, false);
        vector<int> curPath;
        curPath.push_back(i);
        
        iter(i, edges, 0, curPath, used, maxD, answer);
    }

    return maxD;
}

int main(void)
{
    for (int N, L; cin >> N >> L, (N || L); ) {
        vector<vector<Edge> > edges(N + 1);
        for (int i = 0; i < L; ++i) {
            int s, t, d; cin >> s >> t >> d;
            edges[s].push_back(Edge(i, t, d));
            edges[t].push_back(Edge(i, s, d));
        }
        for (int i = 0; i < N; ++i) {
            sort(edges[i].begin(), edges[i].end());
        }

        vector<int> answer;
        int distance = solve(N, L, edges, answer);
        cout << distance << endl;
        for (int i = 0; i < answer.size(); ++i) {
            if (i != 0) { cout << ' '; }
            cout << answer[i];
        }
        cout << endl;
    }

    return 0;
}
