#include <iostream>
#include <vector>

using namespace std;

int main()
{
    for (int n, r; cin >> n >> r, n || r; ) {
        // create card
        vector<int> card(n);
        for (int i = 0; i < n; ++i) { card[i] = n - i; }
        
        // shuffle
        for (int i = 0; i < r; ++i) {
            vector<int> target(card);
            int p, c; cin >> p >> c;
            for (int j = 0; j < p - 1; ++j) {
                target[c + j] = card[j];
            }
            for (int j = p - 1; j < p + c - 1; ++j) {
                target[j - p + 1] = card[j];
            }
            swap(card, target);
        }
        
        cout << card[0] << endl;
    }
}
