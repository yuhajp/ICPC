#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(void)
{
    for (int N, s, t; cin >> N >> s >> t, (N || s || t); ) {
        vector<bool> tbl(N + 1, true);
        for (int i = 0; i * s <= N; ++i) {
            for (int j = 0; i * s + j * t <= N; ++j) {
                if (!tbl[i * s + j * t]) { break; }
                tbl[i * s + j * t] = false;
            }
        }
        cout << count(tbl.begin(), tbl.end(), true) << endl;
    }
    
    return 0;
}
