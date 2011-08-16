#include <iostream>
#include <vector>
#include <set>
#include <queue>

using namespace std;

typedef pair<int, int> P;
const int dx[] = { 0,  0, 1,  1, -1, -1};
const int dy[] = { 1, -1, 0, -1,  0,  1};


bool near(const P& x, const P& y)
{
    if (x.first == y.first) {
        return x.second - 1 == y.second || x.second + 1 == y.second;
    } else if (x.first - 1 == y.first) {
        return x.second == y.second || x.second + 1 == y.second;
    } else if (x.first + 1 == y.first) {
        return x.second - 1 == y.second || x.second == y.second;
    } else {
        return false;
    }
}

bool fatal(int ith, const P& sect, const vector<P>& curSerpent)
{
    for (int i = 0; i < ith; ++i) {
        if (curSerpent[i] == sect) { return true; }
    }
    for (int i = 0; i < ith - 1; ++i) {
        if (near(curSerpent[i], sect)) { return true; }
    }
    if (ith - 1 >= 0 && !near(curSerpent[ith - 1], sect)) { return true; }

    return false;
}

void nextStateIter(vector<vector<P> >& rslt,
                   int ith, vector<P>& curSerpent, bool movable, const vector<P>& serpent, const set<P>& rocks)
{
    if (ith == serpent.size()) {
        // 移動しきった
        rslt.push_back(curSerpent);
        return;
    }

    if (movable) {
        for (int i = 0; i < 6; ++i) {
            P sect = P(serpent[ith].first + dx[i], serpent[ith].second + dy[i]);
            //cout << i << " : (" << sect.first << "," << sect.second << ")" << endl;
            if (rocks.count(sect)) { continue; }
            if (fatal(ith, sect, curSerpent)) { continue; }
            curSerpent[ith] = sect;
            nextStateIter(rslt, ith + 1, curSerpent, false, serpent, rocks);
        }
    }
    
    // In case this section don't move
    if (fatal(ith, serpent[ith], curSerpent)) { return; }
    curSerpent[ith] = serpent[ith];
    nextStateIter(rslt, ith + 1, curSerpent, true, serpent, rocks);
}

vector<vector<P> > nextState(const vector<P>& serpent, const set<P>& rocks)
{
    vector<P> curSerpent(serpent);
    vector<vector<P> > rslt;
    nextStateIter(rslt, 0, curSerpent, true, serpent, rocks);
    return rslt;
}

int solve(const vector<P>& serpent, const set<P>& rocks, const P& goal)
{
    queue<pair<vector<P>, int> > Q;
    set<vector<P> > visited;
    Q.push(make_pair(serpent, 0));
    visited.insert(serpent);

    while (!Q.empty()) {
        vector<P> s = Q.front().first;
        int ith = Q.front().second;
        Q.pop();

        // serpent が取りうる状態を全列挙
#if 0
        cout << ith << endl;
        for (int i = 0; i < s.size(); ++i) {
            cout << "(" << s[i].first << "," << s[i].second << ")";
        }
        cout << endl;
#endif
        vector<vector<P> > nexts = nextState(s, rocks);
        for (int i = 0; i < nexts.size(); ++i) {
            if (visited.count(nexts[i])) { continue; }
            visited.insert(nexts[i]);
            if (nexts[i][0] == goal) { return ith + 1; }
            if (ith + 1 <= 20) {
                Q.push(make_pair(nexts[i], ith + 1));
            }
        }
    }

    abort(); // shouldn't happen.
    return -1;
}

int main(void)
{
    for (int N; cin >> N, N; ) {
        vector<P> serpent(N);
        for (int i = 0; i < N; ++i) {
            int x, y; cin >> x >> y; 
            serpent[i] = P(x, y);
        }

        int K; cin >> K;
        set<P> rocks;
        for (int i = 0; i < K; ++i) {
            int x, y; cin >> x >> y;
            rocks.insert(P(x, y));
        }

        int X, Y; cin >> X >> Y;
        
        cout << solve(serpent, rocks, P(X, Y)) << endl;
    }

}
