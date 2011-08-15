#include <iostream>
#include <vector>

using namespace std;

int main(void)
{
    for (int N, Q; cin >> N >> Q, (N || Q); ) {
        vector<int> v(100);
        for (int i = 0; i < N; ++i) {
            int n; cin >> n;
            for (int j = 0; j < n; ++j) {
                int t; cin >> t;
                v[t] += 1;
            }
        }

        int day = 0;
        int num = 0;
        for (int i = 0; i < 100; ++i) {
            if (v[i] >= Q) {
                if (v[i] > num) {
                    day = i;
                    num = v[i];
                }
            }
        }

        cout << day << endl;
    }
    
}
