#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;

const int N = 1000000;

void makePrimeSums(vi& sums)
{    
    vector<bool> primes(N, true);
    primes[0] = primes[1] = false;

    for (int i = 2; i < N; ++i) {
        if (!primes[i])
            continue;
        for (int j = i + i; j < N; j += i)
            primes[j] = false;
    }

    for (int i = 1; i < N; ++i) {
        sums[i] = sums[i-1] + primes[i];
    }
}

int main(void)
{
    vi sums(N);
    makePrimeSums(sums);

    for (int n; cin >> n, n; ) {
        cout << sums[2*n] - sums[n] << endl;
    }

    return 0;
}
