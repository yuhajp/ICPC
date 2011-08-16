#include <iostream>
#include <vector>

using namespace std;

struct Range {
    int pc, ts, te;
    Range(int pc, int ts) : pc(pc), ts(ts), te(0) {}
};

int main(void)
{
    for (int N, M; cin >> N >> M, (N || M); ) {
        vector<vector<Range> > st(M + 1);
        int r; cin >> r;
        for (int i = 0; i < r; ++i) {
            int t, n, m, s;
            cin >> t >> n >> m >> s;
            if (s) {
                st[m].push_back(Range(n, t));
            } else {
                for (int j = st[m].size() - 1; j >= 0; --j) {
                    if (st[m][j].pc == n) {
                        st[m][j].te = t;
                        break;
                    }
                }
            }
        }

        int q; cin >> q;
        for (int i = 0; i < q; ++i) {
            vector<bool> used(1300);
            int ts, te, m; cin >> ts >> te >> m;
            for (int j = 0; j < st[m].size(); ++j) {
                fill(used.begin() + st[m][j].ts, used.begin() + st[m][j].te, true);
            }

            cout << count(used.begin() + ts, used.begin() + te, true) << endl;
        }
    }

    return 0;
}

