#include <iostream>
#include <vector>

using namespace std;

int solve(const vector<int>& table, const vector<vector<int> >& nxt)
{
    int minPenalty = table.size();

    // cout << "TBL:" << table.size() << endl;
    for (int i = 0; i < table.size(); ++i) {
        for (int j = 0; j < nxt[i].size(); ++j) {
            if (nxt[i][j] >= table.size()) { continue; }
            if (table[i] == table[nxt[i][j]]) {
                vector<int> t;
                t.reserve(table.size() - 2);
                for (int k = 0; k < table.size(); ++k) {
                    if (k != i && k != nxt[i][j]) {
                        t.push_back(table[k]);
                    }
                }
                
                int p = solve(t, nxt);
                if (p < minPenalty) { minPenalty = p; }
            }
        }
    }

    return minPenalty;
}

int main(void)
{
    // enumerate all places to be compared.
    vector<vector<int> > nxt(20);
    for (int i = 0; i < 20; ++i) {
        int x = i % 4;
        int y = i / 4;
        if (x + 1 < 4)               { nxt[i].push_back(i + 1); }
        if (y + 1 < 5 && x - 1 >= 0) { nxt[i].push_back(i + 3); }
        if (y + 1 < 5)               { nxt[i].push_back(i + 4); }
        if (y + 1 < 5 && x + 1 < 4)  { nxt[i].push_back(i + 5); }
    }
    
    int N; cin >> N;
    for (int i = 0; i < N; ++i) {
        vector<int> tbl(20);
        for (int j = 0; j < 20; ++j) {
            cin >> tbl[j];
        }
        cout << solve(tbl, nxt) << endl;
    }
}
