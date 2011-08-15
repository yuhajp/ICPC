#include <iostream>
#include <vector>

using namespace std;

struct Piece {
    char n, e, s, w;
    
    Piece() {}
    Piece(char n, char e, char s, char w) : n(n), e(e), s(s), w(w) {}

    Piece rot() const {
        return Piece(e, s, w, n);
    }
};

bool compare(char s, char t)
{
    switch (s) {
    case 'r': return (t == 'R');
    case 'g': return (t == 'G');
    case 'b': return (t == 'B');
    case 'w': return (t == 'W');
    case 'R': return (t == 'r');
    case 'G': return (t == 'g');
    case 'B': return (t == 'b');
    case 'W': return (t == 'w');        
    }
    throw "shouldn't happen";
}

int iter(int ith, Piece board[][3], const vector<Piece>& pieces, vector<bool>& used)
{
    if (ith == pieces.size()) { return 1; }

    int sum = 0;
    int w = ith % 3;
    int h = ith / 3;
    for (int i = 0; i < pieces.size(); ++i) {
        if (used[i]) { continue; }
        Piece p = pieces[i];
        for (int j = 0; j < 4; ++j) {
            p = p.rot();
            if (w - 1 >= 0) {
                if (!compare(board[h][w - 1].e, p.w)) { continue; }
            }
            if (h - 1 >= 0) {
                if (!compare(board[h - 1][w].s, p.n)) { continue; }
            }
            board[h][w] = p;
            used[i] = true;
            sum += iter(ith + 1, board, pieces, used);
            used[i] = false;
        }
    }

    return sum;
}

int solve(const vector<Piece>& pieces)
{
    Piece board[3][3];
    vector<bool> used(pieces.size());
    return iter(0, board, pieces, used);
}

int main(void)
{
    int N; cin >> N;
    while (N--) {
        vector<Piece> pieces(9);
        for (int i = 0; i < 9; ++i) {
            char n, e, s, w;
            cin >> n >> e >> s >> w;
            pieces[i] = Piece(n, e, s, w);
        }

        cout << solve(pieces) << endl;
    }

    return 0;
}
