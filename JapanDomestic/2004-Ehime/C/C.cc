#include <iostream>

using namespace std;

// 次に試す単位分数を 1/d とする。
// 分数の積を cp/cq で表す。cq は分母の積も兼ねる。
// 残り n 個の分数で表す必要が有る。
int solve(int p, int q, int a, int n, int d, int cp, int cq )
{
    if (n <= 0) { return 0; }

    int sum = 0;
    while (true) {
        // cp/cq + 1/d =  (cp * d + cq) / (cp * d)
        int cp2 = cp * d + cq;
        int cq2 = cq * d;
        if (cq2 > a) { break; }
        
        if (cp2 * q == cq2 * p) { 
            ++sum; 
        } else {
            sum += solve(p, q, a, n - 1, d, cp2, cq2);
        }
        ++d;
    }
    
    return sum;
}

int main()
{
    for (int p, q, a, n; cin >> p >> q >> a >> n, (p || q || a || n); ) {
        cout << solve(p, q, a, n, 1, 0, 1) << endl;
    }
    return 0;
}
