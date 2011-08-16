#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <algorithm>
#include <numeric>
#include <utility>
#include <complex>
#include <cmath>
#include <cassert>

using namespace std;
typedef complex<double> P;
typedef vector<int> vi;
typedef vector<vector<int> > vvi;

int solve(int n, int p)
{
    int wan = p;
    vector<int> ss(n);
    int i = 0;
    while (true) {
        if (wan == 0) {
            wan = ss[i];
            ss[i] = 0;
        } else {
            ss[i]++; wan--;
            if (wan == 0 && ss[i] == p) {
                return i;
            }
        }
        i = (i + 1) % n;
    }
    return -1;
}

int main(void)
{
    for (int n, p; cin >> n >> p, (n || p); ) {
        cout << solve(n, p) << endl;
    }
    return 0;
}

