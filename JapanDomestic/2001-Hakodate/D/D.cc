#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

typedef vector<vector<pair<bool, int> > > equation_t;
const int QUESTION = -10000000;

bool solve(equation_t& t, int H, int W)
{
    bool changed;
    bool foundQuestion;
    do {
        changed = false;
        foundQuestion = false;
        
        for (int h = 0; h < H; ++h) {
            int widx = -1;
            int sum = 0;
            for (int w = 0; w < W; ++w) {
                if (t[h][w].second == QUESTION) {
                    foundQuestion = true;
                    if (widx >= 0) { widx = -1; break; }
                    else { widx = w; }
                } else {
                    sum += t[h][w].second;
                }
            }
            
            if (widx >= 0) {
                t[h][widx].second = t[h][W].second - sum;
                changed = true;
            }
        }

        for (int w = 0; w < W; ++w) {
            int hidx = -1;
            int sum = 0;
            for (int h = 0; h < H; ++h) {
                if (t[h][w].second == QUESTION) {
                    foundQuestion = true;
                    if (hidx >= 0) { hidx = -1; break; }
                    else { hidx = h; }
                } else {
                    sum += t[h][w].second;
                }
            }
            
            if (hidx >= 0) {
                t[hidx][w].second = t[H][w].second - sum;
                changed = true;
            }
        }
    } while (changed);

    return !foundQuestion;
}

int main(void)
{
    bool first = true;
    while (true) {
        int H, W;
        cin >> H; if (!H) { return 0; }
        cin >> W;

        if (first) { first = false; }
        else { cout << endl; }
        
        equation_t t(H + 1, vector<pair<bool, int> >(W + 1));
                     
        for (int h = 0; h <= H; ++h) {
            for (int w = 0; w <= W; ++w) {
                string str; cin >> str;
                if (str == "?") {
                    t[h][w] = make_pair(true, QUESTION);
                } else {
                    t[h][w] = make_pair(false, atoi(str.c_str()));
                }
            }
        }
                     
        if (solve(t, H, W)) {
            for (int h = 0; h <= H; ++h) {
                for (int w = 0; w <= W; ++w) {
                    if (t[h][w].first) {
                        cout << t[h][w].second << endl;
                    }
                }
            }
        } else {
            cout << "NO" << endl;
        }
    }
}
