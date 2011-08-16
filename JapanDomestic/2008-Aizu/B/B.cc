#include <iostream>
#include <vector>

using namespace std;

bool is_prime(int k, const vector<int>& ps)
{
  for (int i = 0; i < ps.size(); ++i) {
    if (k % ps[i] == 0) { return false; }
  }

  return true;
}

int main(void)
{
  vector<int> ps;
  for (int i = 1; 7 * i - 1 < 300000; ++i) {
    int s = 7 * i - 1; if (is_prime(s, ps)) { ps.push_back(s); }
    int t = 7 * i + 1; if (is_prime(t, ps)) { ps.push_back(t); }
  }

  // cout << "OK" << endl;
  for (int x; cin >> x, x != 1; ) {
    cout << x << ":";
    for (int i = 0; i < ps.size() && x >= ps[i]; ++i) {
      if (x % ps[i] == 0) {
	cout << ' ' << ps[i];
      }
    }
    cout << endl;
  }
}
