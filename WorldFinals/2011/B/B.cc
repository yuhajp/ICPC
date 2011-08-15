#include<algorithm>
#include<complex>
#include<cstdio>
#include<iostream>
#include <vector>
#include <cassert>

using namespace std;

typedef complex<double> cd;
typedef complex<int> ci;

#define DEBUG cerr<<"Debug: "<<__LINE__<<endl
#define NUMBER_OF_POINTS 3
#define EPS 1e-6
#define EQUIVALENT_SOLUTIONS ("equivalent solutions")
#define INCONSISTENT_SOLUTIONS ("inconsistent solutions")
#define NO_SOLUTION ("no solution")

struct Affine {
    double m[3][3];

    Affine() {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                m[i][j] = 0.0;
    }

    static Affine translation(int x, int y) {
        Affine a;
        a.m[0][0] = 1;
        a.m[0][2] = x;
        a.m[1][1] = 1;
        a.m[1][2] = y;
        a.m[2][2] = 1;
        return a;
    }

    static Affine scale(int x, int y) {
        Affine a;
        a.m[0][0] = x;
        a.m[1][1] = y;
        a.m[2][2] = 1;
        return a;
    }

    static Affine rotate(double theta) {
        Affine a;
        a.m[0][0] = cos(theta);
        a.m[0][1] = -sin(theta);
        a.m[1][0] = sin(theta);
        a.m[1][1] = cos(theta);
        a.m[2][2] = 1;
        return a;
    }

    friend Affine operator*(const Affine& lhs, const Affine& rhs) {
        Affine a;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                for (int k = 0; k < 3; ++k)
                    a.m[i][j] += lhs.m[i][k] * rhs.m[k][j];
        return a;
    }

    bool equals(const Affine& rhs) const {
        const Affine& lhs = *this;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                if (abs(lhs.m[i][j] - rhs.m[i][j]) > EPS) return false;
        return true;
    }
};

int gcd(int a, int b) {
    return a%b?gcd(b,a%b):b;
}

bool acceptableRotationTarget(int x, int y) {
    return abs(x) == 10 || abs(y) == 10;
}

void reorder(const int* matching, const ci* src, ci* reordered) {
    for (int i = 0; i < NUMBER_OF_POINTS; i++) {
        reordered[i] = src[matching[i]];
    }
}

ci rotate(int x, int y, const ci& reordered) {
    cd p = cd(x, y);
    p /= abs(p);

    cd r(reordered.real(), reordered.imag());
    r *= p;
    // reordered /= abs(p);
    return ci((int)round(real(r)), (int)round(imag(r)));
}

void rotate(int x, int y, const ci* reordered, ci* rotated) {
    for (int i = 0; i < NUMBER_OF_POINTS; i++) {
        rotated[i] = rotate(x, y, reordered[i]);
    }
}

int determineScaleSub(const int* src, const int* dst, int& scale) {
    for (int i = 0; i < NUMBER_OF_POINTS; i++) {
        int j = (i+1)%NUMBER_OF_POINTS;
        int diffsrc = src[i] - src[j];
        int diffdst = dst[i] - dst[j];
        if (diffsrc == 0 && diffdst == 0) {
            continue;
        } else if (diffsrc == 0 || diffdst == 0) {            
            return 0;
        } else if (diffdst % diffsrc == 0) {
            scale = diffdst / diffsrc; 
            return 1;
        } else {
            return 0;
        }
    }

    // found many factors.
    return 2;
}

// scale の数を返す
int determineScale(const ci* rotated, const ci* dst, int& scaleX, int& scaleY) {
    int srcxs[NUMBER_OF_POINTS];
    int srcys[NUMBER_OF_POINTS];
    int dstxs[NUMBER_OF_POINTS];
    int dstys[NUMBER_OF_POINTS];
    for (int i = 0; i < NUMBER_OF_POINTS; i++) {
        srcxs[i] = real(rotated[i]);
        srcys[i] = imag(rotated[i]);
        dstxs[i] = real(dst[i]);
        dstys[i] = imag(dst[i]);
    }
    int scaleXCount = determineScaleSub(srcxs, dstxs, scaleX);
    int scaleYCount = determineScaleSub(srcys, dstys, scaleY);
    if (scaleXCount * scaleYCount != 1)
        return scaleXCount * scaleYCount;

    return 1;
}

int determineTranslation(const ci* scaled, const ci* dst, int& transX, int& transY) {
    for (int i = 1; i < NUMBER_OF_POINTS; i++) {
        if (dst[0] - scaled[0] != dst[i] - scaled[i])
            return 0;
    }

    transX = real(dst[0] - scaled[0]);
    transY = imag(dst[0] - scaled[0]);
    return 1;
}

int findAcceptableScaleAndTranslation(const ci* rotated, const ci* dst, Affine& affine) {    
    int scaleX, scaleY;
    int scaleCount = determineScale(rotated, dst, scaleX, scaleY);
    if (scaleCount != 1)
        return scaleCount;

    ci scaled[NUMBER_OF_POINTS];
    for (int i = 0; i < NUMBER_OF_POINTS; i++) {
        scaled[i] = ci(scaleX*real(rotated[i]), scaleY*imag(rotated[i]));
    }

    int transX, transY;
    int translationCount = determineTranslation(scaled, dst, transX, transY);
    if (translationCount != 1)
        return translationCount;

    // rotate -> scale -> translation
    affine = Affine::translation(transX, transY) * Affine::scale(scaleX, scaleY) * affine;
    return 1;
}

int findSolutionWithFixedOrder(const ci* reordered, const ci* dst) {
    vector<Affine> affines;
    for (int x = -10; x <= 10; x++) {
        for (int y = -10; y <= 10; y++) {
            if (!acceptableRotationTarget(x, y)) { continue; }
            ci rotated[NUMBER_OF_POINTS];
            rotate(x, y, reordered, rotated);
            Affine affine = Affine::rotate(atan2((double)y, (double)x));
            int cnt = findAcceptableScaleAndTranslation(rotated, dst, affine);
            if (cnt >= 2) { return cnt; }
            if (cnt == 1) {
                if (affines.empty())
                    affines.push_back(affine);
                else if (!affines[0].equals(affine))
                    return 2; // found another affine.
            }
        }
    }

    return affines.size();
}

string solve(const int* srcxy, const int* dstxy) {
    ci src[NUMBER_OF_POINTS], dst[NUMBER_OF_POINTS];
    for (int i = 0; i < NUMBER_OF_POINTS; i++) {
        src[i] = ci(srcxy[2*i], srcxy[2*i+1]);
        dst[i] = ci(dstxy[2*i], dstxy[2*i+1]);
    }

    int solutionCount = 0;
    int matching[NUMBER_OF_POINTS] = {0, 1, 2};
    do {
        ci reordered[NUMBER_OF_POINTS];
        reorder(matching, src, reordered);
        solutionCount += findSolutionWithFixedOrder(reordered, dst);
        if (solutionCount >= 2)
            return INCONSISTENT_SOLUTIONS;
    } while(next_permutation(matching, matching+3));

    assert(solutionCount <= 1);
    return solutionCount == 1 ? EQUIVALENT_SOLUTIONS : NO_SOLUTION;
}

int main(void) {
    for (int casenumber=1; true; casenumber++) {
        int srcxy[6];
        bool allZero = true;
        for (int i = 0; i < 6; i++) {
            cin >> srcxy[i];
            if (srcxy[i] != 0) {
                allZero = false;
            }
        }

        if (allZero) {
            break;
        }

        int dstxy[6];
        for (int i = 0; i < 6; i++) {
            cin >> dstxy[i];
        }
        printf("Case %d: %s\n", casenumber, solve(srcxy, dstxy).c_str());
    }
}
