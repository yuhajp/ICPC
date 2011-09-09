#include<iostream>
#include<algorithm>
#include<cmath>
#include<cstdio>

using namespace std;

typedef pair<int, int> range;

bool isLandingPossible(int n, const range* ranges, double interval) {
	double landingTime = ranges[0].first;
	
	for(int i = 1; i < n; i++) {
		double nextLandingTimeMin = landingTime + interval;
		if(nextLandingTimeMin < ranges[i].first) {
			landingTime = ranges[i].first;
		} else if(ranges[i].first <= nextLandingTimeMin && nextLandingTimeMin <= ranges[i].second) {
			landingTime = nextLandingTimeMin;
		} else {
			return false;
		}
	}
	return true;
}


int binarySearch(int n, const range* ranges, double min, double max) {
	int imin = (int)(floor(min+0.5));
	int imax = (int)(floor(max+0.5));
	if(imin == imax) {
		return imin;
	}
	
	double mid = 0.5*(min + max);
	if(isLandingPossible(n, ranges, mid)) {
		return binarySearch(n, ranges, mid, max);
	} else {
		return binarySearch(n, ranges, min, mid);
	}
}


int main(void) {
	int caseNumber = 1;
	while(true) {
		int n;
		cin >> n;
		if(n == 0)break;
		
		range ranges[n];
		for(int i = 0; i < n; i++) {
			int b, e;
			cin >> b >> e;
			ranges[i] = make_pair(60*b, 60*e);
		}
		
		int output = 0;
		
		sort(ranges, ranges + n);
		do {
			double outputPlusPoint5 = (double)output + 0.5;
			if(isLandingPossible(n, ranges, outputPlusPoint5)) {
				output = binarySearch(n, ranges, outputPlusPoint5, (double)(ranges[0].first + ranges[n-1].second) / (n-1));
			}
		} while(next_permutation(ranges, ranges + n));
		printf("Case %d: %d:%02d\n", caseNumber++, output/60, output%60);
	}
}

