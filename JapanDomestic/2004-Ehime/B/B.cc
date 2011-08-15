#include <iostream>
#include <string>
#include <vector>

using namespace std;

int go(vector<vector<int> >& table, int h, int w, int H, int W)
{
    if (h < 0 || H <= h || w < 0 || W <= w) { return 0; }
    if (table[h][w] == 1) { return 0; }
    
    table[h][w] = 1;
    return 1 + 
        go(table, h, w + 1, H, W) +
        go(table, h, w - 1, H, W) +
        go(table, h + 1, w, H, W) +
        go(table, h - 1, w, H, W);
}

int main(void)
{
    for (int H, W; cin >> W >> H, W || H; ) {
        // create
        vector<vector<int> > table(H, vector<int>(W));
        int hw, hh;
        for (int h = 0; h < H; ++h) {
            for (int w = 0; w < W; ++w) {
                char c; cin >> c;
                if (c == '#') { table[h][w] = 1; }
                if (c == '.') { table[h][w] = 0; }
                if (c == '@') { table[h][w] = 0; hw = w; hh = h;}
            }
        }

        cout << go(table, hh, hw, H, W) << endl;
    }

    return 0;
}
