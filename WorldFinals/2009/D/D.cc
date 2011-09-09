#include<iostream>
#include<cmath>
#include<complex>
#include<cstdio>
#include<algorithm>

using namespace std;

typedef complex<double> cd;

#define EPS 1e-6
#define DEBUG cout<<"DEBUG:"<<__LINE__<<endl

double calculateRadian(double r1, double r2, double r) {
	double a = r - r1;
	double b = r - r2;
	double c = r1 + r2;
	double cosValue = (a*a + b*b - c*c) / (2.0 * a * b);
	return acos(cosValue);
}

bool allocatable(const double *innerR, double diameter){
	double outerR = 0.5*diameter;
	cd p[4];
	p[0] = outerR - innerR[0];
	double theta[4];
	
	for(int i = 1; i < 4; i++) {
		theta[i] = 0.0;
		for(int j = 0; j < i; j++){
			double tempTheta = theta[j] + calculateRadian(innerR[j], innerR[i], outerR);
			theta[i] = max(theta[i], tempTheta);
		}
		p[i] = polar(outerR - innerR[i], theta[i]);
	}
	for(int i = 0; i < 4; i++) {
		for(int j = i+1; j < 4; j++) {
			if(abs(p[i] - p[j]) < innerR[i] + innerR[j] - EPS) {
				return false;
			}
		}
	}
	return true;
}

int solveFixedOrders(const double *permutatedInputs, double lowerBoundOfAnswer, int minimumKnownAnswer) {
	if(!allocatable(permutatedInputs, (double)minimumKnownAnswer - 0.5))return minimumKnownAnswer;
	
	double min = lowerBoundOfAnswer;
	double max = (double)minimumKnownAnswer - 0.5;
	
	while(true) {
		int minIntValue = (int)(floor(min+0.5));
		int maxIntValue = (int)(floor(max+0.5));
		
		if(minIntValue == maxIntValue){
			return minIntValue;
		}
		
		double mid = 0.5*(min + max);
		if(allocatable(permutatedInputs, mid)) {
			max = mid;
		} else {
			min = mid;
		}
	}
}

int solveAllOrders(double *ascendingOrderInputs) {
	int answer = 0;
	double permutatedInputs[4];
	double lowerBoundOfAnswer = 2.0*ascendingOrderInputs[2] + 2.0*ascendingOrderInputs[3];
	
	for(int i = 0; i < 4; i++) {
		permutatedInputs[i] = ascendingOrderInputs[i];
		answer += (int)(2.0*ascendingOrderInputs[i]);
	}
	do {
		answer = solveFixedOrders(permutatedInputs, lowerBoundOfAnswer, answer);
	} while(next_permutation(permutatedInputs, permutatedInputs + 4));
	
	return answer;
}

int main(void) {
	double input[4];
	for(int caseNumber = 1; true; caseNumber++) {
		cin >> input[3];
		if(input[3] == 0)break;
		cin >> input[2] >> input[1] >> input[0];
		
		for(int i = 0; i < 4; i++)input[i] *= 0.5;
		
		printf("Case %d: %d\n", caseNumber, solveAllOrders(input));
	}
}
