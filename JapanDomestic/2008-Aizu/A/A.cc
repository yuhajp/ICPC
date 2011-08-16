#include <iostream>
#include <vector>

using namespace std;

int main(void)
{
  for (int N, M; cin >> N >> M, (N || M); ) {
    vector<int> l(N), r(M);
    int ls = 0, rs = 0;
    for (int i = 0; i < N; ++i) { cin >> l[i]; ls += l[i];}
    for (int i = 0; i < M; ++i) { cin >> r[i]; rs += r[i];}

    int cs = 1000;
    int ans_l, ans_r;
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < M; ++j) {
	if (ls - l[i] + r[j] == rs + l[i] - r[j]) {
	  if (cs > l[i] + r[j]) {
	    cs = l[i] + r[j];
	    ans_l = l[i], ans_r = r[j];
	  }
	}
      }
    }

    if (cs == 1000) {
      cout << -1 << endl;
    } else {
      cout << ans_l << ' ' << ans_r << endl;
    }
  }

  return 0;
}
