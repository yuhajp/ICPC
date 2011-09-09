#include <iostream>
#include <vector>
#include <list>
#include <string>

using namespace std;

struct State {
    int N;
    pair<int, int> ps[3];

    State(int N) : N(N) {}
};

// typedef vector<pair<int, int> > State;
int dh[] = { 0, 1, 0, -1,  0};
int dw[] = { 0, 0, 1,  0, -1};

int idx(int W, int H, const State& st)
{
    int r = 0;
    for (int i = 0; i < st.N; ++i) {
        r = (r * 256) + st.ps[i].first * W + st.ps[i].second;
    }
    return r;
}

void nextSub(const vector<vector<int> >& field, const State& st, State& s, vector<State>& ns, int i)
{
    if (i < st.N) {
        for (int d = 0; d < 5; ++d) {
            s.ps[i].first += dh[d];
            s.ps[i].second += dw[d];
            if (!field[s.ps[i].first][s.ps[i].second]) {
                nextSub(field, st, s, ns, i + 1);
            }
            s.ps[i].first -= dh[d];
            s.ps[i].second -= dw[d];            
        }
    } else {
        for (int k1 = 0; k1 < s.N; ++k1) {
            for (int k2 = k1 + 1; k2 < s.N; ++k2) {
                if (s.ps[k1] == s.ps[k2]) { return; } // on the same position
                if (s.ps[k1] == st.ps[k2] && s.ps[k2] == st.ps[k1]) { return; } // swapped.
            }
        }
        ns.push_back(s);
    }
}

vector<State> next(const vector<vector<int> > & field, const State& st)
{
    vector<State> ns;
    State s(st);
    nextSub(field, st, s, ns, 0);

    return ns;
}

int solve(const vector<vector<int> >& f, int W, int H, int N, 
          const State& initial, const State& goals)
{
    vector<int> cache(256 * 256 * 256, -1);
    list<State> sts;

    int goal_idx = idx(W, H, goals);
    cache[idx(W, H, initial)] = 0;
    sts.push_back(initial);

    while (!sts.empty()) {
        State st = sts.front(); sts.pop_front();
        int idxst = idx(W, H, st);        
        int cost = cache[idxst];
        if (idxst == goal_idx) { return cost; }
        const vector<State>& nextStates = next(f, st);
        for (int i = 0; i < nextStates.size(); ++i) {
            int idxnst = idx(W, H, nextStates[i]);
            if (cache[idxnst] >= 0) { continue; }
            cache[idxnst] = cost + 1;
            sts.push_back(nextStates[i]);
        }
    }

    return -1;
}


int main(void)
{
    for (int W, H, N; cin >> W >> H >> N, (W || H || N); ) {
        vector<vector<int> > field(H, vector<int>(W));
        State initial(N);
        State goal(N);

        string str;
        getline(cin, str); // chop
        for (int h = 0; h < H; ++h) {
            getline(cin, str);
            // cout << str << endl;
            for (int w = 0; w < W; ++w) {
                switch (str[w]) {
                case '#': field[h][w] = true; break;
                case ' ': field[h][w] = false; break;
                case 'A': field[h][w] = false; goal.ps[0]    = make_pair(h, w); break;
                case 'B': field[h][w] = false; goal.ps[1]    = make_pair(h, w); break;
                case 'C': field[h][w] = false; goal.ps[2]    = make_pair(h, w); break;
                case 'a': field[h][w] = false; initial.ps[0] = make_pair(h, w); break;
                case 'b': field[h][w] = false; initial.ps[1] = make_pair(h, w); break;
                case 'c': field[h][w] = false; initial.ps[2] = make_pair(h, w); break;
                default: abort(); break;
                }
            }
        }

        cout << solve(field, W, H, N, initial, goal) << endl;
    }
    
}
