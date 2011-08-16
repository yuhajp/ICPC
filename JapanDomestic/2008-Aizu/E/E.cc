#include <iostream>
#include <vector>
#include <complex>

using namespace std;

typedef complex<double> point;
const double EPS = 0.000001;

struct line {
  point s, e;
  double h;
  line(point s, point e, double h = 0.0) : s(s), e(e), h(h) {};
};

double cross(point p, point q) { return imag(conj(p) * q); }

bool is_crossed(const line& e1, const line& e2)
{
  point A = e1.e - e1.s, B = e2.e - e2.s, S = e2.s - e1.s;
  double f = cross(A, B);
  if (-EPS < f && f < EPS) { return false; }
  double p = cross(S, B) / f, q = cross(S, A) / f;
  return (0 <= p && p <= 1 && 0 <= q && q <= 1);
}

double dist(line e, point p)
{
  e.e -= e.s; p -= e.s;
  
  double d = abs(e.e);
  p = d * (p / e.e);

  if (0 <= p.real() && p.real() <= d) { return abs(p.imag()); }
  return min(abs(p), abs(p - d));
}

double dist(const line& e1, const line& e2)
{
  if (is_crossed(e1, e2)) { return 0.0; }
  return std::min(std::min(dist(e1, e2.s), dist(e1, e2.e)), 
		  std::min(dist(e2, e1.s), dist(e2, e1.e)));
}

double solve(const line& course, const vector<line>& obs)
{
  double answer = 10000.0;
  for (int i = 0; i < obs.size(); ++i) {
    double d = dist(course, obs[i]);
    double h = obs[i].h;
    if (h > d) { answer = min(answer, d); }
    else { answer = min(answer, (h * h + d * d) / (2 * h)); }
  }

  return answer;
}

int main(void)
{
  for (int N; cin >> N, N; ) {
    double sx, sy, ex, ey; cin >> sx >> sy >> ex >> ey;
    line course = line(point(sx, sy), point(ex, ey));

    vector<line> obs;
    bool ok = true;
    for (int i = 0; i < N; ++i) {
      double x1, y1, x2, y2, h; 
      cin >> x1 >> y1 >> x2 >> y2 >> h;
      obs.push_back(line(point(x1, y1), point(x1, y2), h));
      obs.push_back(line(point(x1, y1), point(x2, y1), h));
      obs.push_back(line(point(x2, y2), point(x1, y2), h));
      obs.push_back(line(point(x2, y2), point(x2, y1), h));
      if (x1 <= sx && sx <= x2 && y1 <= sy && sy <= y2) { ok = false; }
      if (x1 <= ex && ex <= x2 && y1 <= ey && ey <= y2) { ok = false; }      
    }

    double answer = ok ? solve(course, obs) : 0.0;
    char buf[256];
    sprintf(buf, "%.4f", answer);
    cout << buf << endl;
  }
  return 0;
}
