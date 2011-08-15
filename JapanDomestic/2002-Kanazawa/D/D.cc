#include <iostream>
#include <vector>

using namespace std;

struct Hint {
    vector<int> num;
    vector<bool> used;
    int hit;
    int blow;

    Hint(int L) : num(L), used(10, false) {}
};

bool satisfy(int L, const vector<int>& n, const vector<bool>& used, const Hint& h)
{
    int hit = 0;
    for (int i = 0; i < L; ++i) {
        if (n[i] == h.num[i]) { ++hit; }
    }

    int blow = 0;
    for (int i = 0; i < 10; ++i) {
        if (used[i] && h.used[i]) { ++blow; }
    }
    blow -= hit;

    return (hit == h.hit && blow == h.blow);
}

void iter(int ith, int L, vector<int>& n, vector<bool>& used,
          const vector<Hint>& hints, int& found, vector<int>& foundNum)
{
    if (ith == L) {
        for (int i = 0; i < hints.size(); ++i) {
            if (!satisfy(L, n, used, hints[i])) {
                return;
            }
        }

        // all hints are satisfied
        ++found;
        foundNum = n;
        return;
    }

    for (int i = 0; i < 10; ++i) {
        if (used[i]) { continue; }
        n[ith] = i;
        used[i] = true;
        iter(ith + 1, L, n, used, hints, found, foundNum);
        used[i] = false;
    }
}

void solve(int L, const vector<Hint>& hints)
{
    vector<int> n(L);
    vector<bool> used(10, false);
    int found = 0; 
    vector<int> foundNum;

    iter(0, L, n, used, hints, found, foundNum);

    if (found == 1) {
        for (int i = 0; i < L; ++i) {
            cout << foundNum[i];
        }
        cout << endl;
    } else {
        cout << "NO" << endl;
    }
}

int main(void)
{
    for (int L, H; cin >> L >> H, (L || H); ) {
        vector<Hint> hints(H, Hint(L));
        for (int i = 0; i < H; ++i) {
            string num;
            cin >> num >> hints[i].hit >> hints[i].blow;

            for (int j = 0; j < L; ++j) {
                int x = num[j] - '0';
                hints[i].used[x] = true;
                hints[i].num[j] = x;
            }
        }

        solve(L, hints);
    }
}
