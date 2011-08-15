#include <iostream>
#include <vector>
#include <utility>

using namespace std;

typedef vector<pair<int, int> > constraint_t;
typedef vector<vector<int> >    table_t;

int iter(int card, const constraint_t& p, table_t& t)
{
    // when all cards are arranged correctly.
    if (card >= 8) { return 1; }

    // the range of constraint is limited, so we can ignore
    // reverse constraints such as (1, 1) <-> (-1, -1)
    int rslt = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (t[i][j] != -1) { continue; }
            
            t[i][j] = card;
            for (int k = 0; k < p.size(); ++k) {
                int x = i + p[k].first;
                int y = j + p[k].second;
                if (x < 0 || 4 <= x || y < 0 || 4 <= y) { continue; }
                if (t[x][y] != -1) { continue; }
                t[x][y] = card;
                rslt += iter(card + 1, p, t);
                t[x][y] = -1;
            }
            t[i][j] = -1;
            // if found, we don't try other candicates.
            if (rslt > 0) { return rslt; }
        }
    }

    return 0;
}

int solve(const constraint_t& p)
{
    table_t t(4, vector<int>(4, -1));
    return iter(0, p, t);
}

int main(void)
{
    while (true) {
        constraint_t p;
        for (int i = 0; i < 4; ++i) {
            int x, y;
            cin >> x; if (x > 4) { return 0; }
            cin >> y;
            p.push_back(make_pair(x, y));
        }
        cout << solve(p) << endl;
    }
    
    return 0;
}
