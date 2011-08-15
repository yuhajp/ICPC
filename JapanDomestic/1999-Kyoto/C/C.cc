#include <iostream>
#include <cmath>

using namespace std;

//
// Note that 0 < a <= 10000, -10000 <= b, c <= 10000
// and, p > 0, r > 0, ((p > r) or (p = r and q >= s))
//
//     ax^2 + bx + c = (px + q)(rx + s)
// <=> pr x^2 + (ps + qr) x + qs
bool solve(int a, int b, int c, int& p, int& q, int& r, int& s)
{
    if (b == 0 && c == 0) {
        // note: a > 0 ==> a >= 1 ==> a >= r ==> p >= r
        q = s = 0; p = a; r = 1;
        return true;
    }
    if (c == 0) {
        p = a; q = b;
        r = 1; s = 0;
        // consider: a = 1, b = -1.
        if (p == r && q < s) { swap(q, s); }
        return true;
    }

    int maxP = a;
    int minQ = -abs(c);
    int maxQ = abs(c);

    // cout << "DEBUG: " << maxP << ' ' << minQ << ' ' << maxQ << endl;
    
    for (p = 1; p <= maxP; ++p) {
        if (a % p != 0) { continue; }
        r = a / p;
        if (p < r) { continue; }

        for (q = minQ; q <= maxQ; ++q) {
            if (q == 0) { continue; }
            if (c % q != 0) { continue; }
            s = c / q;
            if (p * s + q * r == b) {
                if (p == r && q < s) { continue; }
                return true;
            }
        }
    }

    return false;
}

int main(void)
{
    for (int a, b, c; cin >> a >> b >> c, (a || b || c); ) {
        int p, q, r, s;
        if (solve(a, b, c, p, q, r, s)) {
            cout << p << ' ' << q << ' ' << r << ' ' << s << endl;
        } else {
            cout << "Impossible" << endl;
        }
    }
}
