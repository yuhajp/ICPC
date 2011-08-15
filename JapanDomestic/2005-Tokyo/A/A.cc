#include <iostream>

using namespace std;

/**
 * @param   initial     初期資産
 * @param   year        運用年数
 * @param   t   単利 -> 0 複利 -> 1
 * @param   r   利率
 * @param   f   手数料
 * @return  運用したときのお金
 */
double solve(double initial, int year, int t, double r, double f)
{
    if (t) {
        for (int i = 0; i < year; ++i) {
            double benefit = static_cast<int>(initial * r);
            initial = initial + benefit - f;
        }
        return initial;
    } else {
        double benefit = 0;
        for (int i = 0; i < year; ++i) {
            benefit += static_cast<int>(initial * r);
            initial = initial - f;
        }
        return initial + benefit;
    }
}

int main(void)
{
    int m; cin >> m;
    while (m--) {
        double initial;
        int year, n;
        cin >> initial >> year >> n;

        double max_money = 0.0;
        
        int t;
        double r, f;
        while (n--) {
            cin >> t >> r >> f;
            double d = solve(initial, year, t, r, f);
            max_money = max(max_money, d);
        }

        cout << static_cast<int>(max_money) << endl;
    }
    return 0;
}
