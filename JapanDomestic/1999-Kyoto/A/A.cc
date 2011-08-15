#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum Dir {
    NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3,
};

int dx[] = { 0, 1, 0, -1, };
int dy[] = { 1, 0, -1, 0, };

void go(int& w, int& h, int d, int dir, int m, int n)
{
    w += dx[dir] * d;
    h += dy[dir] * d;
    if (w <= 0) { w = 1; }
    if (w >  m) { w = m; }
    if (h <= 0) { h = 1; }
    if (h >  n) { h = n; }
}

int main(void)
{
    for (int m, n; cin >> m >> n, (m || n); ) {
        int h = 1, w = 1;
        Dir dir = NORTH;
        while (true) {
            string str;
            cin >> str;
            if (str == "STOP") {
                break;
            } else if (str == "FORWARD") {
                int d; cin >> d;
                go(w, h, d, dir, m, n);
            } else if (str == "BACKWARD") {
                int d; cin >> d;
                go(w, h, -d, dir, m, n);
            } else if (str == "LEFT") {
                dir = Dir((dir + 3) % 4);
            } else if (str == "RIGHT") {
                dir = Dir((dir + 1) % 4);
            } else {
                throw "shouldn't happen";
            }
        }
        cout << w << ' ' << h << endl;
    }
}
