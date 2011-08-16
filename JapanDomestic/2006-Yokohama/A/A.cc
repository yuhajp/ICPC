#include <iostream>
#include <vector>

using namespace std;

int main(void)
{
    vector<bool> ps(1000000, true);
    ps[0] = ps[1] = false;
    for (int i = 2; i < 1000000; ++i) {
        if (!ps[i]) { continue; }
        for (int j = 2 * i; j < 1000000; j += i) {
            ps[j] = false;
        }
    }

    for (int a, d, n; cin >> a >> d >> n, (a || d || n); ) {
        int i = 0;
        while (true) {
            if (ps[a + d * i]) { 
                --n; 
                if (!n) { break; }
            }
            ++i;
        }
        cout << (a + d * i) << endl;
    }
}
