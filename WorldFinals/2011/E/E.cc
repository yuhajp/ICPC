#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>

using namespace std;
typedef vector<int> vi;
typedef vector<vi> vvi;

#if 0
void show(const vvi& vs)
{
    for (int i = 0; i < vs.size(); ++i) {
        for (int j = 0; j < vs[i].size(); ++j) {
            cout << setw(1) << vs[i][j];
        }
        cout << endl;
    }

    cout << endl;
}
#endif

void calculateSum(int dx, int dy, const vvi& vs, vvi& sums)
{
    const int len = 2*dx + 2*dy;
    
    for (int s = 0; s < len; ++s) {
        int y = s, x = 0;
        while (y >= 0) {
            sums[y][x] = vs[y][x];
            if (x-1 >= 0)
                sums[y][x] += vs[y][x-1];
            if (y-1 >= 0 && x-1 >= 0)
                sums[y][x] += sums[y-1][x-1];
            else if (x-2 >= 0)
                sums[y][x] += sums[y][x-2];
            
            if (y+1 < len && x-1 >= 0)
                sums[y][x] += sums[y+1][x-1];
            if (x-2 >= 0)
                sums[y][x] -= sums[y][x-2];
            ++x, --y;
        }

#if 0
        show(sums);
#endif
    }
}

int count(const vvi& sums, int dy, int dx, int y, int x)
{
//    cout << "count : " << dy << " : " << dx << " : " << y << " : " << x << endl;
    const int len = 2 * (dx + dy);

    if (x + y < 0)
        return 0;

    // 左上に向かって動かす
    if (x + y >= len) {
        int z = (x + y) - (len - 2);
        assert(z % 2 == 0);
        x -= z / 2;
        y -= z / 2;
    }

//    cout << x << ' ' << y << ' ' << len << endl;
    assert(0 <= x + y && x + y < len);

    if (x < 0) 
        return 0; 

    if (y < 0) {
        x += y;
        y -= y;
    }

    return sums[y][x];
}

int totalCount(const vvi& sums, int dx, int dy, int x, int y, int d)
{
    int cnt = 0;
    cnt += count(sums, dy, dx, 2*y, 2*x+2*d);
    cnt -= count(sums, dy, dx, 2*y-2*d-1, 2*x-1);
    cnt -= count(sums, dy, dx, 2*y+2*d+1, 2*x-1);
    cnt += count(sums, dy, dx, 2*y, 2*x-2*d-2);
    return cnt;
}

int solve(int dx, int dy, const vvi& sums, int d, int& ansX, int& ansY)
{
    // NOTE: count するとき、(x, y) を count するのは
    // x + y は 2 の倍数になっている
    int maxCount = -1;
    for (int y = 0; y < dy; ++y) {
        for (int x = 0; x < dx; ++x) {
            int cnt = totalCount(sums, dx, dy, x, y, d);
            if (cnt > maxCount) {
                maxCount = cnt;
                ansX = x;
                ansY = y;
            }
        }
    }

    return maxCount;
}


int main(void)
{
    int caseNo = 0;
    
    while (true) {
        int dx, dy, n, q; cin >> dx >> dy >> n >> q;
        if (!(dx || dy || n || q))
            break;
        
        const int len = 2*dx + 2*dy;
        
        vvi shops(len, vi(len));
        for (int i = 0; i < n; ++i) {
            int x, y; cin >> x >> y;
            shops[2*(y-1)][2*(x-1)] = 1;
        }

#if 0        
        show(shops);
#endif
        vvi sums(len);
        for (int i = 0; i < len; ++i) {
            sums[i] = vi(len - i);
        }
        calculateSum(dx, dy, shops, sums);

#if 0        
        show(sums);
#endif
        cout << "Case " << ++caseNo << ":" << endl;
        for (int i = 0; i < q; ++i) {
            int d; cin >> d;
            int ansX, ansY;
            int maxCount = solve(dx, dy, sums, d, ansX, ansY);
            cout << maxCount << " (" << ansX+1 << ',' << ansY+1 << ')' <<endl;
        }        
    }
    
    return 0;
}
