#include<iostream>
#include<complex>

using namespace std;

typedef complex<double> cd;

#define EPS 1e-6

void readPoints(cd* p, int size) {
	for (int i = 0; i < size; i++) {
		int x, y;
		cin >> x >> y;
		p[i] = cd(x, y);
	}
}

#define NO_POSITIVE_SIDE 1
#define NO_NEGATIVE_SIDE 2

int ccwAllPositiveOrNegative(cd p0, cd p1, const cd* points, int size, int skipIndex) {
	cd v = p1 - p0;
	bool hasPositive = false;
	bool hasNegative = false;
	for (int i = 0; i < size; i++) {
		if (i == skipIndex) continue;
		cd temp = (points[i] - p0) / v;
		if (imag(temp) > EPS) {
			hasPositive = true;
		} else if(imag(temp) < -EPS) {
			hasNegative = true;
		} else if (real(temp) < 1.0) {
			return 0;
		}
		
		if (hasPositive && hasNegative) {
			return 0;
		}
	}
	if (!hasPositive && !hasNegative) {
		return NO_POSITIVE_SIDE | NO_NEGATIVE_SIDE;
	} else if(!hasPositive) {
		return NO_POSITIVE_SIDE;
	} else if(!hasNegative) {
		return NO_NEGATIVE_SIDE;
	} else {
		return 0;
	}
}

bool solve(const cd* b, int bSize, const cd* w, int wSize) {
	for (int bIndex = 0; bIndex < bSize; bIndex++) {
		for (int wIndex = 0; wIndex < wSize; wIndex++) {
			int result1 = ccwAllPositiveOrNegative(b[bIndex], w[wIndex], w, wSize, wIndex);
			if (result1 == 0) continue;
			int result2 = ccwAllPositiveOrNegative(w[wIndex], b[bIndex], b, bSize, bIndex);
			if ((result1 & result2) != 0) {
				return true;
			}
		}
	}
	return false;
}

int main(void) {
	while (true) {
		int bSize, wSize;
		cin >> bSize >> wSize;
		if (bSize == 0 && wSize == 0) {
			break;
		}
		cd b[bSize], w[wSize];
		readPoints(b, bSize);
		readPoints(w, wSize);
		if (solve(b, bSize, w, wSize)) {
			cout << "YES" << endl;
		} else {
			cout << "NO" << endl;
		}
	}
}
