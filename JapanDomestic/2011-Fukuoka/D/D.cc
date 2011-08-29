#include <iostream>
#include <vector>
#include <cassert>

using namespace std;
typedef int bits;

struct Circle {
    int x, y, r, c;
    int numUpperCircles;
    vector<int> children;

    Circle(int x, int y, int r, int c)
        : x(x), y(y), r(r), c(c), numUpperCircles(0)
    {
    }

    bool intersects(const Circle& circle) {
        int distance2 = (y - circle.y) * (y - circle.y) + (x - circle.x) * (x - circle.x);
        int r2 = (r + circle.r) * (r + circle.r);
        return distance2 < r2;
    }

    void remove(vector<Circle>& circles) {
        for (int k = 0; k < children.size(); ++k) {
            circles[children[k]].numUpperCircles -= 1;
            assert(circles[children[k]].numUpperCircles >= 0);
        }
    }

    void unremove(vector<Circle>& circles) {
        for (int k = 0; k < children.size(); ++k) {
            circles[children[k]].numUpperCircles += 1;
            assert(circles[children[k]].numUpperCircles >= 0);
        }
    }
};

void updateHierarchy(vector<Circle>& circles)
{
    const int N = circles.size();
    for (int i = 0; i < N; ++i)
        circles[i].numUpperCircles = 0;

    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            if (circles[i].intersects(circles[j])) {
                circles[j].numUpperCircles += 1;
                circles[i].children.push_back(j);
            }
        }
    }
}

void iter(vector<Circle>& circles, int status, int numRemoved, vector<int>& dp)
{
    assert(dp[status] < 0);
    dp[status] = numRemoved;

    for (int i = 0; i < circles.size(); ++i) {
        if ((status & (1 << i)) || circles[i].numUpperCircles)
            continue;
        for (int j = i + 1; j < circles.size(); ++j) {
            if ((status & (1 << j)) || circles[j].numUpperCircles)
                continue;
            if (circles[i].c != circles[j].c)
                continue;

            circles[i].remove(circles);
            status |= (1 << i);
            circles[j].remove(circles);
            status |= (1 << j);

            if (dp[status] < 0)
                iter(circles, status, numRemoved + 2, dp);

            status &= ~(1 << j);
            circles[j].unremove(circles);
            status &= ~(1 << i);
            circles[i].unremove(circles);
        }
    }
}

int solve(vector<Circle> circles)
{
    vector<int> dp(1 << circles.size(), -1);
    updateHierarchy(circles);

    iter(circles, 0, 0, dp);
    
    return *max_element(dp.begin(), dp.end());
}

int main(void)
{
    for (int n; cin >> n, n; ) {
        vector<Circle> circles;
        circles.reserve(n);

        for (int i = 0; i < n; ++i) {
            int x, y, r, c; cin >> x >> y >> r >> c;
            Circle circle(x, y, r, c-1);
            circles.push_back(circle);
        }

        cout << solve(circles) << endl;
    }
    return 0;
}

