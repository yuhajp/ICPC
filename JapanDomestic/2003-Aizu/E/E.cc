#include <iostream>
#include <vector>
#include <valarray>
#include <utility>
#include <set>

using namespace std;

inline
int idx(int h, int w, int H, int W)
{
    return h * W + w;
}

bool iter(const valarray<bool>& board,
          const vector<valarray<bool> >& squares,
          const int restIter)
{
    if (restIter == 0) {
        // if 'true' is found 
        for (int i = 0; i < board.size(); ++i) {
            if (board[i]) { return false; }
        }
        return true;
    }
    if (squares.size() < restIter) {
        return false; 
    }

    vector<valarray<bool> > unused;
    for (int i = 0; i < squares.size(); ++i) {
        if (iter(board && !squares[i], unused, restIter - 1)) {
            return true;
        }
        unused.push_back(squares[i]);
    }

    return false;
}

template<typename T>
bool equals(const valarray<T>& a, const valarray<T>& b)
{
    if (a.size() != b.size()) { return false; }
    int size = a.size();
    for (int i = 0; i < size; ++i) {
        if (a[i] != b[i]) { return false; }
    }

    return true;
}

int solve(const vector<vector<int> >& t, int H, int W)
{
    // step 1.
    valarray<bool> board(H * W);
    valarray<bool> allFalse(H * W);

    vector<valarray<bool> > squares;
    for (int h = 0; h < H; ++h) {
        for (int w = 0; w < W; ++w) {
            board[idx(h, w, H, W)] = t[h][w];
            
            bool allTrue = true;
            int maxSqr = 0;
            for (int s = 1; allTrue && h + s <= H && w + s <= W; ++s) {
                for (int hh = h; allTrue && hh < h + s; ++hh) {
                    for (int ww = w; allTrue && ww < w + s; ++ww) {
                        allTrue &= t[hh][ww];
                    }
                }
                if (allTrue) {
                    maxSqr = s;
                }
            }

            valarray<bool> square(H * W);
            for (int hh = h; hh < h + maxSqr; ++hh) {
                for (int ww = w; ww < w + maxSqr; ++ww) {
                    square[idx(hh, ww, H, W)] = true;
                }
            }
            squares.push_back(square);
        }
    }

    int used = 0;
    bool changed;
    do {
        changed = false;
        for (int i = 0; i < H * W; ++i) {
            if (!board[i]) { continue; }
            int idx = -1;
            for (int j = 0; j < squares.size(); ++j) {
                if (squares[j][i]) {
                    if (idx >= 0) { idx = -1; break; }
                    idx = j;
                }
            }
            if (idx >= 0) {
                board = board && !squares[idx];
                ++used;
                squares.erase(squares.begin() + idx);
                changed = true;
            }
        }

        // remove contained.
        for (int i = 0; i < squares.size(); ++i) {
            valarray<bool> a = squares[i] && board;
            if (equals(a, allFalse)) {
                squares.erase(squares.begin() + i);
                --i;
                changed = true;
                continue;
            }
            for (int j = 0; j < squares.size(); ++j) {
                if (i == j) { continue; }
                valarray<bool> v = (squares[i] || squares[j]) && board;
                valarray<bool> k = squares[j] && board;
                if (equals(v, k)) {
                    squares.erase(squares.begin() + i);
                    --i;
                    changed = true;
                    break;
                }
            }
        }
    } while (changed);
    
    for (int i = 0; i <= squares.size(); ++i) {
        if (iter(board, squares, i)) {
            return used + i;
        }
    }

    throw "shouldn't happen";
}

int main(void)
{
    for (int W, H; cin >> W >> H, (W || H); ) {
        vector<vector<int> > t(H, vector<int>(W));
        for (int h = 0; h < H; ++h) {
            for (int w = 0; w < W; ++w) {
                cin >> t[h][w];
            }
        }

        cout << solve(t, H, W) << endl;
    }
}
