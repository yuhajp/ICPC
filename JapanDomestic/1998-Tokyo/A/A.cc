#include <iostream>
#include <cstdio>
#include <complex>
#include <vector>

using namespace std;

typedef complex<double> P;

double cross(const P& a, const P& b)
{
    return imag(conj(a) * b);
}

double area(const P& a, const P& b, const P& c)
{
    return 0.5 * cross(b - a, c - a);
}

int main(void)
{
    int NUM = 0;
    for (int N; cin >> N, N; ) {
        vector<P> p;
        for (int i = 0; i < N; ++i) {
            int x, y; cin >> x >> y;
            p.push_back(P(x, y));
        }

        double sum = 0.0;
        for (int i = 1; i < N - 1; ++i) {
            sum += area(p[0], p[i + 1], p[i]);
        }

        char buf[256];
        sprintf(buf, "%.1f", sum);
        cout << ++NUM << " " << buf << endl;
    }

    return 0;
}
