#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <utility>
#include <map>
#include <set>
#include <queue>
#include <complex>
#include <cmath>
#include <cassert>
#include <cstdio>

using namespace std;
typedef complex<double> P;

struct Window {
    P a, r; // absolute or relative
    double w, h;
    bool sv, sh, st, sb, sl, sr;

    Window* parent;
    vector<Window*> children;

    Window(double x, double y, double w, double h,
           bool sv, bool sh, bool st, bool sb, bool sl, bool sr) :
        a(x, y), r(x, y), w(w), h(h),
        sv(sv), sh(sh), st(st), sb(sb), sl(sl), sr(sr),
        parent(NULL)
    {
        if (!sh && !st && !sb) { this->st = true; } // at least one of sh, st, or sb should be spring.
        if (!sv && !sl && !sr) { this->sr = true; } // at least one of sv, sl, or sr should be spring.
    }

    bool isIn(const Window* p) const {
        const Window& lhs = *this;
        const Window& rhs = *p;

        return (real(rhs.a) < real(lhs.a) && real(lhs.a) + lhs.w < real(rhs.a) + rhs.w &&
                imag(rhs.a) < imag(lhs.a) && imag(lhs.a) + lhs.h < imag(rhs.a) + rhs.h);
    }

    void updateRelative(const P& absLeftTop) {
        r = a - absLeftTop;
        for (int i = 0; i < children.size(); ++i) {
            children[i]->updateRelative(absLeftTop + r);
        }
    }

    void updateAbsolute(const P& absLeftTop) {
        a = r + absLeftTop;
        for (int i = 0; i < children.size(); ++i) {
            children[i]->updateAbsolute(absLeftTop + r);
        }
    }

    void update(double oldParentW, double oldParentH, double newParentW, double newParentH) {
        if (parent == NULL) {
            // won't change.
            for (int i = 0; i < children.size(); ++i) {
                children[i]->update(oldParentW, oldParentH, newParentW, newParentH);
            }
            this->w = newParentW;
            this->h = newParentH;
            return;
        }

        double oldW = this->w, oldH = this->h;
        double l = real(this->r), r = oldParentW - real(this->r) - this->w;
        double t = imag(this->r), b = oldParentH - imag(this->r) - this->h;

        double scalableOldW = (sv ? w : 0) + (sl ? l : 0) + (sr ? r : 0);
        double fixedOldW    = oldParentW - scalableOldW;
        double scalableNewW = newParentW - fixedOldW;
        double scalableOldH = (sh ? h : 0) + (st ? t : 0) + (sb ? b : 0);
        double fixedOldH    = oldParentH - scalableOldH;
        double scalableNewH = newParentH - fixedOldH;
        double ratioW       = scalableNewW / scalableOldW;
        double ratioH       = scalableNewH / scalableOldH;

        double newL = sl ? l * ratioW : l;
        double newW = sv ? w * ratioW : w;
        double newR = sr ? r * ratioW : r;
        double newT = st ? t * ratioH : t;
        double newH = sh ? h * ratioH : h;
        double newB = sb ? b * ratioH : b;

        this->r = P(newL, newT);
        this->w = newW;
        this->h = newH;

        for (int i = 0; i < children.size(); ++i) {
            children[i]->update(oldW, oldH, newW, newH);
        }
    }
};

void createHierarchy(vector<Window*>& ws)
{
    for (int i = 1; i < ws.size(); ++i) {
        for (int p = 0; p < ws.size(); ++p) {
            if (i == p) { continue; }
            if (ws[i]->isIn(ws[p])) {
                if (ws[i]->parent == NULL) { ws[i]->parent = ws[p]; }
                else if (ws[p]->isIn(ws[i]->parent)) { ws[i]->parent = ws[p]; }
            }
        }
    }

    for (int i = 0; i < ws.size(); ++i) {
        if (ws[i]->parent == NULL) { continue; }
        ws[i]->parent->children.push_back(ws[i]);
    }
}

int main(void)
{
    int caseNo = 0;
    for (int N, R, W, H; cin >> N >> R >> W >> H, (N || R || W || H); ) {
        ++caseNo;

        vector<Window*> ws;
        ws.push_back(new Window(0, 0, W, H, 0, 0, 0, 0, 0, 0));
        for (int i = 0; i < N; ++i) {
            int x, y, w, h, sv, sh, st, sb, sl, sr;
            cin >> x >> y >> w >> h >> sv >> sh >> st >> sb >> sl >> sr;
            ws.push_back(new Window(x, y, w, h, sv, sh, st, sb, sl, sr));
        }

        createHierarchy(ws);
        ws[0]->updateRelative(P(0, 0));

        for (int i = 0; i < R; ++i) {
            int newW, newH; cin >> newW >> newH;
            ws[0]->update(ws[0]->w, ws[0]->h, newW, newH);
            ws[0]->updateAbsolute(P(0, 0));

            cout << "Case " << caseNo << ", resize operation " << (i + 1) << ":" << endl;
            for (int w = 1; w < ws.size(); ++w) {
                char buf[256];
                sprintf(buf, "    Window %d, x = %.0f, y = %.0f, width = %.0f, height = %.0f",
                        w, ws[w]->a.real(), ws[w]->a.imag(), ws[w]->w, ws[w]->h);

                cout << buf << endl;
            }
        }

        for (int i = 0; i < ws.size(); ++i) { delete ws[i]; }
    }
    return 0;
}

