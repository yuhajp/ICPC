#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <climits>
using namespace std;

typedef pair<double, double> DPair;

struct Cell {
    double lp, lh, rp, rh, w;

    Cell() {}
    Cell(double lp, double lh, double rp, double rh, double w) :
        lp(lp), lh(lh), rp(rp), rh(rh), w(w)
    {
    }

    double maxWater() const {
        return (rp - lp) * min(lh, rh) * 30.0;
    }

    double waterHeight() const {
        double height = w / 30.0 / (rp - lp);
        return height >= 50.0 ? 50.0 : height;
    }

    friend ostream& operator<<(ostream& out, const Cell& cell) {
        out << "(" 
            << cell.lp << ", " << cell.lh << ", " 
            << cell.rp << ", " << cell.rh << ", " 
            << cell.w
            << ")";
        return out;
    }
};

void print(vector<Cell>& tank)
{
    cout << "---" << endl;
    for (int i = 0; i < tank.size(); ++i) {
        cout << tank[i] << endl;
    }
}

bool flow(vector<Cell>& tank)
{
    if (tank.size() <= 1) { return false; }

    bool changed = false;
    for (int i = 0; i < tank.size(); ++i) {
        if (tank[i].w <= tank[i].maxWater()) { continue; }
        double flownWater = tank[i].w - tank[i].maxWater();
        tank[i].w = tank[i].maxWater();
        changed = true;
        if (tank[i].lh < tank[i].rh) {
            tank[i - 1].w += flownWater;
        } else {
            tank[i + 1].w += flownWater;
        }
    }

    return changed;
}

bool merge(vector<Cell>& tank)
{
    bool changed = false;
    
    int i = 0;
    while (i + 1 < tank.size()) {
        if (tank[i].w >= tank[i].maxWater() && tank[i + 1].w >= tank[i + 1].maxWater() &&
            tank[i].lh > tank[i].rh && tank[i + 1].lh < tank[i + 1].rh) 
        {
            tank[i].rp = tank[i + 1].rp;
            tank[i].rh = tank[i + 1].rh;
            tank[i].w += tank[i + 1].w;
            tank.erase(tank.begin() + i + 1);
            changed = true;
        } else {
            ++i;
        }
    }
    
    return changed;
}

double solve(vector<Cell>& tank, vector<DPair>& jag, double p, double t)
{
    // poor water
    for (int j = 0; j < jag.size(); ++j) {
        for (int k = 0; k < tank.size(); ++k) {
            if (tank[k].lp < jag[j].first && jag[j].first < tank[k].rp) {
                tank[k].w += jag[j].second * t;
                break;
            }
        }
    }

    vector<double> answer;
    bool changed = false;
    do {
        bool b1 = flow(tank);
        bool b2 = merge(tank);
        changed = b1 || b2;
    } while (changed);


    for (int j = 0; j < tank.size(); ++j) {
        if (tank[j].lp <= p && p < tank[j].rp) {
            return tank[j].waterHeight();
        }
    }

    abort(); // shouldn't happen.
    return -1;
}


int main()
{
    int D; cin >> D;
    while (D--) {
        vector<Cell> tank;
        
        int N; cin >> N;
        double lp = 0.0, lh = 50.0, rp, rh;
        for (int i = 0; i < N; ++i) {
            cin >> rp >> rh;
            tank.push_back(Cell(lp, lh, rp, rh, 0.0));
            lp = rp;
            lh = rh;
        }
        tank.push_back(Cell(lp, lh, 100.0, 50.0, 0.0));
        
        vector<DPair> jag;
        int M; cin >> M;
        for (int i = 0; i < M; ++i) {
            double f, a; cin >> f >> a;
            jag.push_back(DPair(f, a));
        }
        
        int L; cin >> L;
        for (int i = 0; i < L; ++i) {
            double p, t; cin >> p >> t;
            vector<Cell> newTank(tank);
            cout << solve(newTank, jag, p, t) << endl;
        }
    }

    return 0;
}
