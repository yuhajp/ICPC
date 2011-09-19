#include<iostream>
#include<cmath>
#include<cstdio>

using namespace std;

double integral(int n, const double* p, double r1, double r2) {
	double sum = 0.0;
	for(int i = 0; i <= n; i++) {
		sum += p[i] * (pow(r1, i+1) - pow(r2, i+1)) / (i+1);
	}
	return sum;
}

void solve(int n, int caseNumber, const double* p1, const double* p2, double c) {
	double c0 = (1-c*c*c)/3.0;
	double c1 = 1-c*c;
	double c2 = 1-c;
	double c3 = -2.0*integral(n+1, p2, 1.0, c);
	double c4 = -2.0*integral(n, p1, 1.0, c);
	double c5 = (c*c*c+1)/3.0;
	double c6 = c*c-1;
	double c7 = c+1;
	double c8 = -2.0*integral(n+1, p2, c, -1.0);
	double c9 = -2.0*integral(n, p1, c, -1.0);
	
	double d1 = c0 + c7*c*c;
	double d2 = c2 + c7;
	double d3 = c5 - c6*c + c7*c*c;
	double d4 = c1 + 2.0*c7*c;
	double d5 = c6 - 2.0*c7*c;
	double d6 = c6*c - 2.0*c7*c*c;
	double d7 = c3 + c9*c;
	double d8 = c4 + c9;
	double d9 = c8 - c9*c;
	
	double det= 8.0*d1*d2*d3 + 2.0*d4*d5*d6 - 2.0*d1*d5*d5 - 2.0*d2*d6*d6 - 2.0*d3*d4*d4;
	double b1 = -((4.0*d2*d3-d5*d5)*d7 + (d5*d6-2.0*d3*d4)*d8 + (d4*d5-2.0*d2*d6)*d9) / det;
	double b0 = -((d5*d6-2.0*d3*d4)*d7 + (4.0*d1*d3-d6*d6)*d8 + (d4*d6-2.0*d1*d5)*d9) / det;
	double a1 = -((d4*d5-2.0*d2*d6)*d7 + (d4*d6-2.0*d1*d5)*d8 + (4.0*d1*d2-d4*d4)*d9) / det;
	double a0 = b1*c+b0-a1*c;
	
	printf("Case %d: %.3f %.3f %.3f %.3f\n", caseNumber, a1, a0, b1, b0);
}

int main(void) {
	int caseNumber = 1;
	while(true) {
		int n;
		cin >> n;
		if(n == 0)return 0;
		
		double p1[n+1];
		double p2[n+2];
		p2[0] = 0.0;
		for(int i = 0; i <= n; i++) {
			cin >> p1[n-i];
			p2[n+1-i] = p1[n-i];
		}
		double c;
		cin >> c;
		solve(n, caseNumber, p1, p2, c);
		caseNumber++;
	}
}
