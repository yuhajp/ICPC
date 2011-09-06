#include <iostream>
#include <complex>
#include <cassert>

using namespace std;
typedef complex<double> P;

P crossCircles(double x1, double y1, double r1,
               double x2, double y2, double r2)
{
    if (abs(P(x2, y2) - P(x1, y1)) > r1 + r2) {
        cout << abs(P(x2, y2) - P(x1, y1)) << ' ' << r1 + r2 << endl;
        abort();
    }
    if (abs(P(x2, y2) - P(x1, y1)) < abs(r2 - r1)) {
        cout << abs(P(x2, y2) - P(x1, y1)) << ' ' << r1 + r2 << endl;
        abort();
    }

    P rotator = P(x2, y2) - P(x1, y1);
    double a = abs(rotator); rotator /= a;

    double x = (a + (r1 * r1 - r2 * r2) / a) / 2;
    double yy = abs(r1 * r1 - x * x);
    P xp(x, sqrt(yy));

    return xp * rotator + P(x1, y1);
}

double solveE(double x1, double y1, double x2, double y2,
              double r1, double r2, double theta1, double theta2)
{
    double sum = 0;
    if (r1 > 0 && r2 > 0 && r1 + r2 > y2 - y1) {
        P crossed = crossCircles(x2, y2, r1, x2, y1, r2);
        sum += r1 * -arg(crossed - P(x2, y2));
        sum += r2 * (arg(crossed - P(x2, y1)) - theta2);
    } else {
        if (r1 > 0)
            sum += r1 * M_PI / 2;
        if (r2 > 0)
            sum += r2 * (M_PI / 2 - theta2);
    }
    return sum;
}

double solveD(double x1, double y1, double x2, double y2,
              double r1, double r2, double theta1, double theta2)
{
    // 左上と右上まで伸ばしたところ。
    double sum = 0;
    if (r1 > 0 && r2 > 0 && r1 + r2 > x2 - x1) {
        P crossed = crossCircles(x1, y2, r1, x2, y2, r2);
        sum += r1 * (theta1 - arg(crossed - P(x1, y2)));
        sum += r2 * (arg(crossed - P(x2, y2)) - M_PI / 2);
    } else {
        if (r1 > 0)
            sum += r1 * theta1;
        if (r2 > 0)
            sum += r2 * M_PI / 2;
    }
    return sum;
}

double solveC(double x1, double y1, double x2, double y2,
              double r1, double r2, double theta1, double theta2)
{
    // 左上と右下まで伸ばしたところ。
    // それぞれ、x 軸とのなす角が theta1, theta2 の角度まで処理済み

    double sum = 0;

    if (r1 > (x2 - x1) && r2 > (y2 - y1)) {
        P crossed = crossCircles(x1, y2, r1, x2, y1, r2);
        if (crossed.real() > x2 && crossed.imag() > y2) {
            sum += r1 * (theta1 - arg(crossed - P(x1, y2)));
            sum += r2 * (arg(crossed - P(x2, y1)) - theta2);
            return sum;
        }

        if (crossed.real() <= x2) {
            sum += r2 * (M_PI / 2 - theta2);
            r2 -= y2 - y1;
            return sum + solveD(x1, y1, x2, y2, r1, r2, theta1, theta2);
        } else {
            sum += r1 * theta1;
            r1 -= x2 - x1;
            return sum + solveE(x1, y1, x2, y2, r1, r2, theta1, theta2);
        }

    }

    if (r1 > x2 - x1) {
        sum += r1 * theta1;
        r1 -= x2 - x1;
        return sum + solveE(x1, y1, x2, y2, r1, r2, theta1, theta2);
    }

    if (r2 > y2 - y1) {
        sum += r2 * (M_PI / 2 - theta2);
        r2 -= y2 - y1;
        return sum + solveD(x1, y1, x2, y2, r1, r2, theta1, theta2);
    }

    if (r1 > 0)
        sum += r1 * theta1;
    if (r2 > 0)
        sum += r2 * (M_PI / 2 - theta2);

    return sum;
}

// 第一象限と第二象限を両方使う場合
double solveA(double r, double x1, double y1, double x2, double y2)
{
    assert(x1 <= 0);
    assert(x1 < x2);
    assert(y1 < y2);
    assert(0 < y1);

    if (r <= y1)
        return r * M_PI * 2;    

    double sum = r * M_PI;

    double theta1;
    double r1;
    if (r <= abs(P(x1, y1))) {
        theta1 = arg(P(-sqrt(r*r - y1*y1), y1));
        r1 = -1;
        sum += r * (M_PI - theta1);
    } else {
        theta1 = arg(P(x1, y1));
        r1 = r - abs(P(x1, y1));
        sum += r * (M_PI - theta1);

        sum += r1 * (theta1 - M_PI / 2);
        r1 -= y2 - y1;
    }

    double theta2;
    double r2;
    if (r <= abs(P(x2, y1))) {
        theta2 = arg(P(sqrt(r*r - y1*y1), y1));
        r2 = -1;
        sum += r * theta2;
    } else {
        theta2 = arg(P(x2, y1));
        r2 = r - abs(P(x2, y1));
        sum += r * theta2;
    }

    // 左上と右下まで処理。あとは solve C に任せる。
    return sum + solveC(x1, y1, x2, y2, r1, r2, M_PI / 2, theta2);
}

// 第一象限のみを使う場合
double solveB(double r, double x1, double y1, double x2, double y2)
{
    if (r <= abs(P(x1, y1)))
        return r * 2 * M_PI;

    double sum = r * M_PI;
    
    double theta1;
    if (r <= abs(P(x1, y2)))
        theta1 = arg(P(x1, sqrt(r*r - x1*x1)));
    else
        theta1 = arg(P(x1, y2));
    sum += r * (M_PI - theta1);
    double r1 = r - abs(P(x1, y2));

    double theta2;
    if (r <= abs(P(x2, y1)))
        theta2 = arg(P(sqrt(r*r - y1*y1), y1));
    else
        theta2 = arg(P(x2, y1));
    sum += r * theta2;
    double r2 = r - abs(P(x2, y1));

    return sum + solveC(x1, y1, x2, y2, r1, r2, theta1, theta2);
}

double solve(double r, double x1, double y1, double x2, double y2)
{
    assert (x1 < x2);
    assert (y1 < y2);
    
    // Assume: x1 < x < x2, y1 < y < y2
    // 
    if (y1 <= 0 && 0 <= y2) swap(x1, y1), swap(x2, y2);
    if (y2 < 0) { y1 = -y1, y2 = -y2; swap(y1, y2); }
    if (x2 < 0) { x1 = -x1, x2 = -x2; swap(x1, x2); }
    
    if (x1 < 0) {
        // 計算が楽なので、重心を右によせておく
        if (-x1 < x2) 
            return solveA(r, x1, y1, x2, y2);
        else
            return solveA(r, -x2, y1, -x1, y2);
    } else 
        return solveB(r, x1, y1, x2, y2);
}

int main(void)
{
    for (double r, x1, y1, x2, y2; cin >> r >> x1 >> y1 >> x2 >> y2, (r || x1 || y1 || x2 || y2);) {
        double answer =  solve(r, x1, y1, x2, y2);
        char buf[80];
        sprintf(buf, "%.6f", answer);
        cout << buf << endl;
    }
    return 0;
}
