#include <iostream>
#include <vector>
#include <complex>

using namespace std;
typedef complex<double> P;

double EPS = 1E-8;

double cross(const P& p1, const P& p2)
{
    return imag(conj(p1) * p2);
}

bool linelineinter(P& inter, const P& As, const P& Ae, const P& Bs, const P& Be)
{
    P A = Ae - As, B = Be - Bs, S = Bs - As;
    double f = cross(A, B);
    if (-EPS <= f && f <= EPS) { return false; }

    double p = cross(S, B) / f;
    inter = As + p * A;
    return true;
}

P mirror(const P& p, const P& s, const P& e)
{
    P ps = p - s, es = e - s;
    return conj(ps / es) * es + s;
}

double minHeight(const P& p0, const P& p1, const P& p2, 
                 const P& pa, const P& pb, const P& pc)
{
    //cout << pa << pb << pc << endl;

    P pah = mirror(pa, p0, p1);
    P pbh = mirror(pb, p1, p2);
    
    P pi;
    if (!linelineinter(pi, pa, pah, pb, pbh)) { return -1; }
    
    double r2 = norm(pa - p0);
    double d2 = norm(pi - p0);
    double h2 = r2 - d2;
    if (h2 < EPS) { return -1; }
    return sqrt(h2);
}

vector<P> crossCircle(const P& o1, double r1, const P& o2, double r2)
{
    //cout << "crossCircle:" << o1 << r1 << o2 << r2 << endl;
    if (abs(o2 - o1) < abs(r2 - r1) || abs(o2 - o1) > abs(r2 + r1)) { return vector<P>(); }
    
    P rot = o2 - o1;
    double a = abs(rot); rot /= a;
    
    double x = (a + (r1 * r1 - r2 * r2) / a) / 2;
    double y2 = abs(r1 * r1 - x * x);
    P xp(x, sqrt(y2));

    vector<P> rslt;
    rslt.push_back(xp * rot + o1);
    //cout << "CCA" << (xp * rot + o1) << endl;
    if (abs(xp - conj(xp)) > EPS) {
        rslt.push_back(conj(xp) * rot + o1);
        //cout << "CCB" << (conj(xp) * rot + o1) << endl;
    }
    return rslt;
}

double solveB(const P& p0, const P& p1, const P& p2, const P& pa, const P& pb)
{
    //cout << "SOLVE B: " << pa << pb << endl;
    double minH = 1E+10;

    vector<P> pcs = crossCircle(p0, abs(pa - p0), p2, abs(pb - p2));
    for (int i = 0; i < pcs.size(); ++i) {
        double x = pcs[i].real();
        double y = pcs[i].imag();
        if (abs(x - floor(x + 0.5)) >= EPS || abs(y - floor(y + 0.5)) >= EPS) { continue; }
        int pcx = static_cast<int>(floor(x + 0.5));
        int pcy = static_cast<int>(floor(y + 0.5));
        if (pcx < -100 || 100 < pcx || pcy < -100 || 100 < pcy) { continue; }
        P pc(pcx, pcy);
        //cout << pc << endl;
        if (cross(pc - p2, p0 - p2) <= 0) { continue; }
        //cout << pc << endl;
        double h = minHeight(p0, p1, p2, pa, pb, pc);
        if (h > 0.0 && h < minH) { minH = h; }
    }

    return minH;
}

double solveA(const P& p0, const P& p1, const P& p2, const P& pa)
{
    //cout << p0 << p1 << p2 << endl;
    double minH = 1E+10;
    P pa1 = pa - p1;
    int pbx_min = std::max(-100, static_cast<int>(floor(p1.real() - abs(pa1))));
    int pbx_max = std::min(100, static_cast<int>(ceil(p1.real() + abs(pa1))));
    
    //cout << pbx_min << " - " << pbx_max << endl;
    for (int pbx = pbx_min; pbx <= pbx_max; ++pbx) {
        int dpby2 = floor(norm(pa1) + 0.5) - (pbx - p1.real()) * (pbx - p1.real());
        // cout << "*dpby2" << dpby2 << endl;
        if (dpby2 < 0) { continue; }
        double dpby = sqrt(dpby2);
        //cout << "*dpby " << dpby << endl;
        if (abs(dpby - floor(dpby + 0.5)) >= EPS) { continue; }
        int dpbyi = static_cast<int>(floor(dpby + 0.5));
        //cout << "*dpbyi " << dpbyi << endl;
        for (int kb = 0; kb < 2; ++kb) {
            int pby = p1.imag() + (kb == 0 ? 1 : -1) * dpbyi;
            if (pby < -100 || 100 < pby) { continue; }
            P pb(pbx, pby);
            //cout << "B: " << pb << endl;
            if (cross(pb - p1, p2 - p1) <= 0) { continue; }
            double h = solveB(p0, p1, p2, pa, pb);
            if (h > 0.0 && h < minH) { minH = h; }
        }
    }

    return minH;
}

double solve(const P& p0, const P& p1, const P& p2)
{
    double minH = 1E+10;


    for (int pax = -100; pax <= 100; ++pax) {
        for (int pay = -100; pay <= 100; ++pay) {
            P pa(pax, pay);
            if (cross(pa - p0, p1 - p0) <= 0) { continue; }
            double h = solveA(p0, p1, p2, pa);
            if (h > 0.0 && h < minH) { minH = h; }
        }
    }

    return (minH == 1E+10) ? -1 : minH;
}

int main(void)
{
    for (int x0, y0, x1, y1, x2, y2; cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2, (x0 || y0 || x1 || y1 || x2 || y2); ) {
        P p0(x0, y0), p1(x1, y1), p2(x2, y2);

        double answer = solve(p0, p1, p2);
        if (answer < 0) {
            cout << -1 << endl;
        } else {
            char buf[256];
            sprintf(buf, "%.7f", answer);
            cout << buf << endl;
        }
    }

    return 0;
}
