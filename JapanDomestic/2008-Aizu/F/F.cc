#include <iostream>
#include <vector>

using namespace std;

const int dh[] = {-1, 0, 1,  0};
const int dw[] = { 0, 1, 0, -1};

const int lf[4][2] = {{-1, -1}, {-1, 0}, {0,  0}, { 0, -1}};
const int rf[4][2] = {{-1,  0}, { 0, 0}, {0, -1}, {-1, -1}}; 

struct Piece
{
  int p, l, r;
  Piece(int p = 0, int l = 0, int r = 0) : p(p), l(l), r(r) {}
};

struct Cutter
{
  vector<vector<Piece> > f;
  const int S;  
  int h, w, d;
  const int init_h, init_w, init_d;

  Cutter(const vector<vector<Piece> >& f, int S, int h, int w, int d) : 
    f(f), S(S), h(h), w(w), d(d), init_h(h), init_w(w), init_d(d)
  {
  }

  Piece& left_front() {
    return f[h + lf[d][0]][w + lf[d][1]];
  }

  Piece& right_front() {
    return f[h + rf[d][0]][w + rf[d][1]];
  }

  bool filled_front() {
    return left_front().p && right_front().p;
  }

  bool advancable() {
    return left_front().p && right_front().p && !left_front().r && !right_front().l;
  }

  void advance() {
    left_front().l += 1; right_front().r += 1;
    h += dh[d]; w += dw[d];
  }

  void undo() {
    w -= dw[d]; h -= dh[d];
    right_front().r -= 1; left_front().l -= 1; 
  }

  bool equal_pieces() {
    vector<vector<bool> > piece_left(S + 2, vector<bool>(S + 2));
    vector<vector<bool> > piece_right(S + 2, vector<bool>(S + 2));    
    int cnt_left = paint_left(piece_left,  init_h + lf[init_d][0], init_w + lf[init_d][1]);
    if (cnt_left == 0) { return false; }    
    int cnt_right = paint_right(piece_right, init_h + rf[init_d][0], init_w + rf[init_d][1]);
    if (cnt_right == 0) { return false; }
    if (cnt_left != cnt_right) { return false; }

#if 0
    cout << "==============================" << endl;
    for (int h = 0; h < S + 2; ++h) {
      for (int w = 0; w < S + 2; ++w) {
	cout << (piece_left[h][w] ? 'L' : ' ');
      }
      cout << endl;
    }
    cout << "------------------------------" << endl;
    for (int h = 0; h < S + 2; ++h) {
      for (int w = 0; w < S + 2; ++w) {
	cout << (piece_right[h][w] ? 'R' : ' ');
      }
      cout << endl;
    }
    cout << "==============================" << endl;
#endif

    return compare(piece_left, piece_right);
  }

  int paint_left(vector<vector<bool> >& piece, int hh, int ww)
  {
    if (piece[hh][ww] || !f[hh][ww].p || f[hh][ww].r > 0) { return 0; }

    piece[hh][ww] = 1;
    return 1 +
      paint_left(piece, hh - 1, ww) +
      paint_left(piece, hh + 1, ww) +
      paint_left(piece, hh, ww - 1) +
      paint_left(piece, hh, ww + 1);
  }

  int paint_right(vector<vector<bool> >& piece, int hh, int ww) 
  {
    if (piece[hh][ww] || !f[hh][ww].p || f[hh][ww].l > 0) { return 0; }

    piece[hh][ww] = 1;
    return 1 +
      paint_right(piece, hh - 1, ww) +
      paint_right(piece, hh + 1, ww) +
      paint_right(piece, hh, ww - 1) +
      paint_right(piece, hh, ww + 1);
  }

  bool compare(vector<vector<bool> >& left, vector<vector<bool> >& right)
  {
    normalize(left);
    for (int i = 0; i < 4; ++i) {
      normalize(right);
      if (left == right) { return true; }
      turn_right(right);
    }
    reverse(right);
    for (int i = 0; i < 4; ++i) {
      normalize(right);
      if (left == right) { return true; }
      turn_right(right);
    }

    return false;
  }

  void reverse(vector<vector<bool> >& vv)
  {
    vector<vector<bool> > ans(S + 2, vector<bool>(S + 2));
    for (int h = 0; h < S + 2; ++h) {
      for (int w = 0; w < S + 2; ++w) {
	ans[h][w] = vv[h][S + 1 - w];
      }
    }

    swap(vv, ans);
  }

  void turn_right(vector<vector<bool> >& vv)
  {
    vector<vector<bool> > ans(S + 2, vector<bool>(S + 2));
    for (int h = 0; h < S + 2; ++h) {
      for (int w = 0; w < S + 2; ++w) {
	ans[h][w] = vv[w][S + 1 - h];
      }
    }

    swap(vv, ans);
  }

  void normalize(vector<vector<bool> >& vv)
  {
    int minh = S + 2, minw = S + 2;
    for (int h = 0; h < S + 2; ++h) {
      for (int w = 0; w < S + 2; ++w) {
	if (vv[h][w]) {
	  minh = min(minh, h);
	  minw = min(minw, w);
	}
      }
    }

    for (int h = 0; h < S + 2; ++h) {
      for (int w = 0; w < S + 2; ++w) {
	if (h + minh < S + 2 && w + minw < S + 2) {
	  vv[h][w] = vv[h + minh][w + minw];
	} else {
	  vv[h][w] = 0;
	}
      }
    }
  }

  friend ostream& operator<<(ostream& out, const Cutter& c) {
    out << "Cutter(" << c.h << "," << c.w << "," << c.d << ")";
    return out;
  }
};

bool iter(Cutter& c)
{
  if (!c.filled_front()) { return c.equal_pieces(); }

  // straight
  if (c.advancable()) { 
    c.advance();
    if (iter(c)) { return true; }
    c.undo();
  }

  // turn-right
  if (!c.left_front().r) {
    c.d = (c.d + 1) % 4;
    if (c.advancable()) {
      c.advance();
      if (iter(c)) { return true; }
      c.undo();
    }
    c.d = (c.d + 3) % 4;

  }

  // turn-left
  if (!c.right_front().l) {
    c.d = (c.d + 3) % 4;
    if (c.advancable()) {
      c.advance();
      if (iter(c)) { return true; }
      c.undo();
    }
    c.d = (c.d + 1) % 4;
  }

  return false;
}

bool solve(const vector<vector<Piece> >& f, int S)
{
  for (int h = 1; h <= S; ++h) {
    for (int w = 1; w <= S; ++w) {
      for (int d = 0; d < 4; ++d) {
	Cutter c1(f, S, h, w, d);
	Cutter c2(f, S, h - dh[d], w - dw[d], d);
	if (c1.filled_front() && !c2.filled_front()) {
	  if (iter(c1)) { return true; }
	}
      }
    }
  }

  return false;
}

int main(void)
{
  for (int W, H; cin >> W >> H, (W || H); ) {
    int cnt = 0;
    int S = max(W, H);
    vector<vector<Piece> > f(S + 2, vector<Piece>(S + 2));
    for (int h = 0; h < H; ++h) {
      for (int w = 0; w < W; ++w) {
	cin >> f[h + 1][w + 1].p;
	cnt += f[h + 1][w + 1].p;
      }
    }

    if (cnt % 2 == 1) {
      cout << "NO" << endl; continue;
    }
    cout << (solve(f, S) ? "YES" : "NO") << endl;
  }

  return 0;
}
