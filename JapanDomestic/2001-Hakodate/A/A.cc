#include <iostream>
#include <vector>

using namespace std;

int main(void)
{
    int N; cin >> N;
    while (N--) {
        int t[5][5];
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                cin >> t[i][j];
            }
        }

        int size = 0;
        for (int left = 0; left < 5; ++left) {
            for (int right = left; right < 5; ++right) {
                for (int top = 0; top < 5; ++top) {
                    for (int bottom = top; bottom < 5; ++bottom) {
                        bool allTrue = true;
                        for (int w = left; allTrue && w <= right; ++w) {
                            for (int h = top; allTrue && h <= bottom; ++h) {
                                allTrue &= t[w][h];
                            }
                        }
                        if (allTrue) {
                            size = max(size, (right - left + 1) * (bottom - top + 1));
                        }
                    }
                }
            }
        }

        cout << size << endl;
    }
    return 0;
}
