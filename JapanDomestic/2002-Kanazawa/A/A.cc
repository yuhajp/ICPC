#include <iostream>

using namespace std;

int main(void)
{
    int N; cin >> N;
    while (N--) {
        int x = 0, y = 0;
        int tx = 0, ty = 0;
        for (int dx, dy; cin >> dx >> dy; ) {
            if (dx == 0 && dy == 0) { break; }
            x += dx; y += dy;
            if (x * x + y * y == tx * tx + ty * ty && x > tx) {
                tx = x; ty = y;
            } else if (x * x + y * y > tx * tx + ty * ty) {
                tx = x; ty = y;
            }
        }

        cout << tx << ' ' << ty << endl;
    }
}
