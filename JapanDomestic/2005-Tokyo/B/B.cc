#include <iostream>
#include <vector>
#include <complex>
using namespace std;

typedef complex<double> P;

void normalize(vector<P>& v)
{
    P orig = v[0];
    P vect = (v[1] - v[0]) / abs(v[1] - v[0]);
    for (int i = 0; i < v.size(); ++i) {
        v[i] = (v[i] - orig) / vect;
    }
}

bool is_same(const vector<P>& v1, const vector<P>& v2)
{
    if (v1.size() != v2.size()) { return false; }

    vector<P> v3(v2.begin(), v2.end());
    vector<P> v4(v2.rbegin(), v2.rend());

    normalize(v3);
    normalize(v4);

    int i;
    for (i = 0; i < v3.size(); ++i) {
        if (abs(v1[i] - v3[i]) >= 1E-10) { break; }
    }
    if (i == v3.size()) { return true; }

    for (i = 0; i < v4.size(); ++i) {
        if (abs(v1[i] - v4[i]) >= 1E-10) { break; }
    }
    if (i == v4.size()) { return true; }

    return false;
}

int main(void)
{
    for (int n; cin >> n, n; ) {
        vector<P> v;
        int m; cin >> m;
        while (m--) { double x, y; cin >> x >> y; v.push_back(P(x, y)); }
        normalize(v);

        for (int i = 0; i < n; ++i) {
            vector<P> vv;
            int m; cin >> m;
            while (m--) { double x, y; cin >> x >> y; vv.push_back(P(x, y)); }
            if (is_same(v, vv)) {
                cout << (i + 1) << endl;
            }
        }

        cout << "+++++" << endl;
    }

    return 0;
}
