#include<algorithm>
#include<iostream>
#include<cstdio>
#include<complex>
#include<vector>

using namespace std;

#define EPS 1e-6
#ifndef M_PI
	#define M_PI 3.1415926535897932384626433832795028841971
#endif

typedef complex<double> cd;

int bitCount(int x) {
	return x == 0 ? 0 : (x&1) + bitCount(x>>1);
}

int getLeftMostIndex(vector<cd> p) {
	int output = 0;
	for(int i = 1; i < p.size(); i++) {
		if(p[output].real() == p[i].real()) {
			if(p[output].imag() > p[i].imag()) {
				output = i;
			}
		} else if(p[output].real() > p[i].real()) {
			output = i;
		}
	}
	return output;
}

double calculateLengthOfConvexHull(vector<cd> p){
	double length = 0.0;
	int start = getLeftMostIndex(p);
	int pos = start;
	cd prevDrct = cd(0.0, 1.0);
	while(true) {
		int nextPos = (pos == 0) ? 1 : 0;
		cd nextDrct = p[nextPos] - p[pos];
		double nextArg = arg(nextDrct / prevDrct);
		if(abs(nextArg - M_PI) < EPS) {
			nextArg = -M_PI;
		}
		double nextLen = abs(p[nextPos] - p[pos]);
		for(int i = 0; i < p.size(); i++) {
			if(i == pos)continue;
			
			cd tempDrct = p[i] - p[pos];
			double tempArg = arg(tempDrct / prevDrct);
			if(abs(tempArg - M_PI) < EPS) {
				tempArg = -M_PI;
			}
			double tempLen = abs(p[i] - p[pos]);
			if (abs(nextArg - tempArg) < EPS) {
				if(tempLen < nextLen) {
					nextPos = i;
					nextArg = tempArg;
					nextLen = tempLen;
				}
			} else if(nextArg < tempArg) {
				nextPos = i;
				nextArg = tempArg;
				nextLen = tempLen;
			}
		}
		length += nextLen;
		prevDrct = p[nextPos] - p[pos];
		pos = nextPos;
		
		if(pos == start) {
			return length;
		}
	}
}

double calcualteCostSingleFence(int group, const cd* originp, int m, int n) {
	vector<cd> p;
	for(int i = 0; i < n; i++) {
		if((group & (1 << i)) != 0) {
			p.push_back(originp[i]);
		}
	}
	if(p.size() == 1) {
		return 2.0 * m * M_PI;
	}
	return 2.0 * m * M_PI + calculateLengthOfConvexHull(p);
}

vector<int> getDPOrder(int n){
	vector<pair<int, int> > array;
	int dpsize = 1 << n;
	for(int i = 1; i < dpsize; i++) {
		array.push_back(make_pair(bitCount(i), i));
	}
	sort(array.begin(), array.end());
	
	vector<int> output;
	for(vector<pair<int, int> >::iterator it = array.begin(); it != array.end(); it++) {
		output.push_back(it->second);
	}
	
	return output;
}

double solve(int n, int m, const cd* p) {
	int dpsize = 1<<n;
	double dptable[dpsize];
	dptable[0] = 0.0;
	
	vector<int> order = getDPOrder(n);
	for(vector<int>::iterator it = order.begin(); it != order.end(); it++) {
		dptable[*it] = calcualteCostSingleFence(*it, p, m, n);
		for(vector<int>::iterator jt = order.begin(); jt != it; jt++) {
			if((*it & *jt) == *jt) {
				dptable[*it] = min(dptable[*it], dptable[*jt] + dptable[*it - *jt]);
			}
		}
	}
	return dptable[dpsize - 1];
}

int main(void) {
	for(int caseNumber = 1; true; caseNumber++) {
		int n, m;
		cin >> n >> m;
		if(n == 0 && m == 0)break;
		
		cd p[n];
		for(int i = 0; i < n; i++) {
			int x, y;
			cin >> x >> y;
			p[i] = cd(x, y);
		}
		printf("Case %d: length = %.2f\n", caseNumber, solve(n, m, p));
	}
}
