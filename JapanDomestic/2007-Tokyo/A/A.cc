#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main(void)
{
    for (int N; cin >> N, N; ) {
        vector<int> v(N);
        int sum = 0;
        for (int i = 0; i < N; ++i) {
            cin >> v[i];
            sum += v[i];
        }
        
        sum -= *max_element(v.begin(), v.end());
        sum -= *min_element(v.begin(), v.end());
        cout << (sum / (N - 2)) << endl;
    }

    return 0;
}
