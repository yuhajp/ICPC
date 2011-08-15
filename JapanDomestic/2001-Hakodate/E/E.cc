#include <iostream>
#include <vector>

using namespace std;

enum Face { TOP, BOTTOM, FRONT, BACK, LEFT, RIGHT };

class Dice {
private:
    int val[6];
    vector<int> id;

public:
    Dice() : id(6) {
        id[TOP] = 0; id[FRONT] = 1; id[LEFT] = 2;
        id[RIGHT] = 3; id[BACK] = 4; id[BOTTOM] = 5;
        fill(val, val + 6, 0);
    }
    
    int& operator[] (Face f) { return val[id[f]]; }
    const int& operator[] (Face f) const { return val[id[f]]; }

    // 手前にまわす
    void rollToFront() { roll(TOP, BACK, BOTTOM, FRONT); }
    // 左に
    void rollToLeft() { roll(TOP, RIGHT, BOTTOM, LEFT); }
    // 右に
    void rollToRight() { roll(TOP, LEFT, BOTTOM, RIGHT); }
    // 奥に
    void rollToBack() { roll(TOP, FRONT, BOTTOM, BACK); }

    vector<int> state() {
        return id;
    }

    void restore(const vector<int>& st) {
        id = st;
    }

private:
    void roll(Face a, Face b, Face c, Face d) {
        int tmp = id[a];
        id[a] = id[b]; id[b] = id[c];
        id[c] = id[d]; id[d] = tmp;
    }
};

void iter(Dice& dice, int t[5][5], int y, int x)
{
    if (x < 0 || 5 <= x || y < 0 || 5 <= y) { return; }
    if (!t[y][x]) { return; }
    if (dice[BOTTOM] == t[y][x]) { return; }
    if (dice[BOTTOM]) { throw "FAIL"; } // dice[BOTTOM] がすでにあり、t[y][x] と違う。
    dice[BOTTOM] = t[y][x];
    
    vector<int> state = dice.state();
    // front
    dice.restore(state);
    dice.rollToFront();
    iter(dice, t, y + 1, x);

    // left
    dice.restore(state);
    dice.rollToLeft();
    iter(dice, t, y, x - 1);

    // right
    dice.restore(state);
    dice.rollToRight();
    iter(dice, t, y, x + 1);

    // back
    dice.restore(state);
    dice.rollToBack();
    iter(dice, t, y - 1, x);
}

bool solve(int t[5][5], int y, int x)
{
    if (x < 0 || y < 0) { return false; }
    
    vector<int> num(6, 0);
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (0 < t[i][j] && t[i][j] < 7) {
                num[t[i][j] - 1]++;
            }
        }
    }
    for (int i = 0; i < 6; ++i) {
        if (num[i] != 1) { return false; }
    }

    Dice dice;
    try {
        iter(dice, t, y, x);
        if (!(dice[TOP] && dice[BOTTOM] && dice[LEFT] && dice[RIGHT] && dice[FRONT] && dice[BACK])) {
            return false;
        }
        if (dice[TOP] + dice[BOTTOM] != 7) { return false; }
        if (dice[LEFT] + dice[RIGHT] != 7) { return false; }
        if (dice[FRONT] + dice[BACK] != 7) { return false; }
        return true;
    } catch (const char* e) {
        return false;
    }
}

int main(void)
{
    int N; cin >> N;
    while (N--) {
        int t[5][5];
        
        int x = -1, y = -1;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (!(cin >> t[i][j])) { return 0; }
                if (t[i][j] == 1) {
                    x = j, y = i;
                }
            }
        }

        cout << (solve(t, y, x) ? "true" : "false") << endl;
    }

    return 0;
}

