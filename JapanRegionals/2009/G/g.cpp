#include<iostream>
#include<complex>
#include<cstdio>

using namespace std;

typedef complex<double> cd;

double getArgFromLawOfCosines(double a, double b, double c) {
	return acos((a*a + b*b - c*c) / (2*a*b));
}

double getT(double a, double b, double len, double t3) {
	double d = len - t3*cos(b);
	double r = t3*sin(b);
	
	double a2 = cos(a) * cos(a);
	double a1 = - (2*d*cos(a)+4*r*sin(a));
	double a0 = d*d;
	
	return (-a1-sqrt(a1*a1-4*a2*a0)) / (2*a2);
}

cd getAngleBisection(cd p, cd p1, cd p2) {
	cd v1 = p1 - p;
	cd v2 = p2 - p;
	cd v1len1 = v1 / abs(v1);
	cd v2len1 = v2 / abs(v2);
	cd v12 = v1len1 + v2len1;
	return v12 / abs(v12);
}

void solve(double x1, double y1, double x2, double y2, double x3, double y3) {
	cd p1 = cd(x1, y1);
	cd p2 = cd(x2, y2);
	cd p3 = cd(x3, y3);
	
	double len12 = abs(p1 - p2);
	double len23 = abs(p2 - p3);
	double len31 = abs(p3 - p1);
	
	double arg123 = getArgFromLawOfCosines(len12, len23, len31);
	double arg231 = getArgFromLawOfCosines(len23, len31, len12);
	double arg312 = getArgFromLawOfCosines(len31, len12, len23);
	
	double halfOfArg123 = arg123 / 2;
	double halfOfArg231 = arg231 / 2;
	double halfOfArg312 = arg312 / 2;
	
	cd angleBisection1 = getAngleBisection(p1, p2, p3);
	cd angleBisection2 = getAngleBisection(p2, p3, p1);
	
	double maxT3 = len31 / (sin(halfOfArg231) / tan(halfOfArg312) + cos(halfOfArg231));
	double minT3 = 0.0;
	
	double r1;
	double r2;
	double r3;
	while (maxT3 - minT3 > 1e-8) {
		double t3 = (maxT3 + minT3) / 2;
		
		double t1 = getT(halfOfArg312, halfOfArg231, len31, t3);
		double t2 = getT(halfOfArg123, halfOfArg231, len23, t3);
		
		r1 = t1*sin(halfOfArg312);
		r2 = t2*sin(halfOfArg123);
		r3 = t3*sin(halfOfArg231);
		
		cd c1 = p1 + t1*angleBisection1;
		cd c2 = p2 + t2*angleBisection2;
		
		if (r1 + r2 > abs(c2 - c1)) {
			minT3 = t3;
		} else {
			maxT3 = t3;
		}
	}
	printf("%.6f %.6f %.6f\n", r1, r2, r3);
}

int main(void) {
	while (true) {
		double x1, y1, x2, y2, x3, y3;
		cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
		if (x1 == 0 && y1 == 0 && x2 == 0 && y2 == 0 && x3 == 0 && y3 == 0) {
			break;
		}
		solve(x1, y1, x2, y2, x3, y3);
	}
}
