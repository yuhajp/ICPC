#include<iostream>
#include<algorithm>
#include<cmath>
#include<cstdio>


using namespace std;

#define DEBUG cout<<"DEBUG:"<<__LINE__<<endl

#define NUM_OF_POINTS	200

#define OUTSIDE_HEIGHT	(1<<20)

int s, p, d, h;
int height[NUM_OF_POINTS][NUM_OF_POINTS];

int getHeight(int i, int j) {
	if (0 <= i && i < s && i/2 <= j && j < i/2+p+i%2) {
		return height[i][j];
	}
	return OUTSIDE_HEIGHT;
}

double calculateLength(double d1, double d2) {
	return sqrt(d1*d1 + d2*d2 - d1*d2);
}

double solveOneTriangleSorted(int minh, int midh, int maxh) {
	if (maxh == OUTSIDE_HEIGHT) {
		return 0.0;
	}
	if (minh == maxh) {
		return 0.0;
	}
	int beginMinH = (minh / h + 1) * h;
	int endMinH   = (midh / h    ) * h;
	if (endMinH == maxh) {
		endMinH -= h;
	}
	double output = 0.0;
	if (beginMinH <= endMinH && minh != midh) {
		int countMinH = (endMinH - beginMinH) / h + 1;
		double dh13BeginMin = d*(double)(beginMinH - minh) / (maxh - minh);
		double dh12BeginMin = d*(double)(beginMinH - minh) / (midh - minh);
		double dh13EndMin = d*(double)(endMinH - minh) / (maxh - minh);
		double dh12EndMin = d*(double)(endMinH - minh) / (midh - minh);
		double lenBeginMin = calculateLength(dh13BeginMin, dh12BeginMin);
		double lenEndMin = calculateLength(dh13EndMin, dh12EndMin);
		output += (lenBeginMin + lenEndMin) * countMinH / 2.0;
	}
	
	int endMaxH   = (midh / h + 1) * h;
	int beginMaxH = (maxh / h    ) * h;
	if (endMaxH <= beginMaxH && midh != maxh) {
		
		int countMaxH = (beginMaxH - endMaxH) / h + 1;
		double dh31BeginMax = d*(double)(beginMaxH - maxh) / (minh - maxh);
		double dh21BeginMax = d*(double)(beginMaxH - maxh) / (midh - maxh);
		double dh31EndMax = d*(double)(endMaxH - maxh) / (minh - maxh);
		double dh21EndMax = d*(double)(endMaxH - maxh) / (midh - maxh);
		double lenBeginMax = calculateLength(dh31BeginMax, dh21BeginMax);
		double lenEndMax = calculateLength(dh31EndMax, dh21EndMax);
		output += (lenBeginMax + lenEndMax) * countMaxH / 2.0;
	}
	
	return output;
}

double solveOneTriangle(int h1, int h2, int h3) {
	int temparray[3];
	temparray[0] = h1;
	temparray[1] = h2;
	temparray[2] = h3;
	sort(temparray, temparray+3);
	
	return solveOneTriangleSorted(temparray[0], temparray[1], temparray[2]);
}

bool isContourEdge(int h1, int h2, int otherSideH1, int otherSideH2) {
	if (h1 == OUTSIDE_HEIGHT) return false;
	if (h1 % h != 0) return false;
	if (h1 != h2) return false;
	return h1 != otherSideH1 || h1 != otherSideH2;
}

double solve() {
	double sum = 0.0;
	for (int i = 0; i < s; i++) {
		for (int j = i/2; j < i/2+p+i%2; j++) {
			sum += solveOneTriangle(getHeight(i, j), getHeight(i, j+1), getHeight(i+1, j+1));
			sum += solveOneTriangle(getHeight(i, j), getHeight(i+1, j), getHeight(i+1, j+1));
			if (isContourEdge(getHeight(i, j), getHeight(i, j+1), getHeight(i-1, j), getHeight(i+1, j+1))) sum += d;
			if (isContourEdge(getHeight(i, j), getHeight(i+1, j), getHeight(i, j-1), getHeight(i+1, j+1))) sum += d;
			if (isContourEdge(getHeight(i, j), getHeight(i+1, j+1), getHeight(i, j+1), getHeight(i+1, j))) sum += d;
		}
	}
	
	return sum;
}

int main(void) {
	for (int casenumber = 1; true; casenumber++) {
		void* result = (cin >> s >> p >> d >> h);
		if (result == NULL) {
			break;
		}
		
		for (int i = 0; i < s; i++) {
			for (int j = 0; j < p + i%2; j++) {
				cin >> height[i][j + i/2];
			}
		}
		printf("Case %d: %.0f\n", casenumber, solve());
	}
}
