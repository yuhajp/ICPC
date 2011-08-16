#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Cake {
    int w, d;
    Cake(int w, int d) : w(w), d(d) {}

    // return small cut.
    Cake cut(int s) {
        s %= (w + d);
        if (s < w) {
            Cake ck = Cake(min(s, w - s), d);
            w = max(s, w - s);
            return ck;
        } else {
            Cake ck = Cake(w, min(s - w, d - (s - w)));
            d = max(s - w, d - (s - w));
            return ck;
        }
    }

    friend bool operator<(const Cake& lhs, const Cake& rhs) {
        if (lhs.w * lhs.d != rhs.w * rhs.d) { return lhs.w * lhs.d < rhs.w * rhs.d; }
        if (lhs.w != rhs.w) { return lhs.w < rhs.w; }
        return lhs.d < rhs.d;
    }
};

int main(void)
{
    for (int n, w, d; cin >> n >> w >> d, (n || w || d); ) {
        vector<Cake> cakes;
        cakes.push_back(Cake(w, d));
        for (int i = 0; i < n; ++i) {
            int p, s; cin >> p >> s;
            --p;
            Cake cake = cakes[p];
            cakes.erase(cakes.begin() + p);
            Cake cake2 = cake.cut(s);
            cakes.push_back(cake2);
            cakes.push_back(cake);
        }

        sort(cakes.begin(), cakes.end());
        for (int i = 0; i < cakes.size(); ++i) {
            if (i) { cout << ' '; }
            cout << (cakes[i].w * cakes[i].d);
        }
        cout << endl;
    }
}
