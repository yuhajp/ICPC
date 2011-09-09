#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
const int MAX = 1300000;


int main(void)
{
  vector<bool> pn(MAX, true);
  pn[0] = pn[1] = false;

  for (int i = 2; i < MAX; ++i) {
    if (!pn[i]) { continue; }
    for (int t = i * 2; t < MAX; t += i) {
      pn[t] = false;
    }
  }

  vector<int> primes;
  for (int i = 0; i < MAX; ++i) {
    if (pn[i]) { primes.push_back(i); }
  }

  for (int x; cin >> x, x; ) {
    vector<int>::iterator it = lower_bound(primes.begin(), primes.end(), x);
    if (*it == x) { cout << 0 << endl; }
    else {
      cout << (*it - *(it - 1)) << endl;
    }
  }

  return 0;
}
