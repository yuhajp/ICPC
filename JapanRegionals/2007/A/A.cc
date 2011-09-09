#include <iostream>
#include <vector>

using namespace std;

int solve(int n, int k, int m)
{
  vector<int> stones;
  for (int i = 0; i < n; ++i) { stones.push_back(i + 1); }
  
  int idx = m - 1;
  do {
    // slow? not so slow!
    stones.erase(stones.begin() + idx);
    idx += k - 1;
    idx %= stones.size();
  } while (stones.size() > 1);

  return stones[0];
}

int main(void)
{
  for (int n, k, m; (cin >> n >> k >> m) && (n && k && m); ) {
    cout << solve(n, k, m) << endl;
  }
  return 0;
}
