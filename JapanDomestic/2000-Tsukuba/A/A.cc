#include <iostream>

using namespace std;

int solve(int z)
{
    int z3 = z * z * z;

    int maxXY = 0;
    for (int x = 1; x < z; ++x) {
        int x3 = x * x * x;
        for (int y = 1; y <= x; ++y) {
            int y3 = y * y * y;
            if (x3 + y3 <= z3) {
                maxXY = max(x3 + y3, maxXY);
            }
        }
    }

    return z3 - maxXY;
}

int main(void)
{
    for (int z; cin >> z, z; ) {
        cout << solve(z) << endl;
    }
    
    return 0;
}
