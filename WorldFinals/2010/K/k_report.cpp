#include<iostream>
#include<vector>
#include<cmath>
#include<complex>
#include<cstdio>

using namespace std;

#define DEBUG	cout<<"DEBUG:"<<__LINE__<<endl

typedef vector<double> point;

typedef vector<vector<double> > rotator;

typedef complex<double> cd;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DIMENSION	3

#define EPS		1e-9

#define A	0
#define B	1
#define C	2
#define D	3
#define E	4
#define F	5
#define G	6

#define ROTATION_SIZE	9
#define POINT_SIZE		7

point makePoint(double x, double y, double z);
void printPoint(const point& p);
point addPoints(const point& a, const point &b);
point subPoints(const point&a, const point &b);
point scalePoint(const point &a, double s);
point outerProductPoints(const point &p1, const point &p2);
double innerProductPoints(const point &p1, const point &p2);
void printRotator(const rotator& r);
point rotate(const rotator &r, const point &p);
rotator multiplyRotators(const rotator &r1, const rotator &r2);
rotator getZeroRotator();
rotator getZAxisRotator(double t);
rotator getXAxisRotator(double t);
rotator getYAxisRotator(double t);
rotator getRotatorPointMoveXAxis(const point &p);
rotator getRotatorPointMoveXYSurface(const point &p);
double calculateTetrahedraVolume(const point &a, const point &b, const point &c, const point &d);
point calculaTetetrahedraCenter(const point &a, const point &b, const point &c, const point &d);
point calculateTwoTetrahedraCenter(const point &a, const point &b, const point &c, const point &d, const point &e);
void printPoints(const vector<point> &points);
bool normalizePosition(vector<point> &points);
bool isPointInTriangle2D(cd p0, cd p1, cd p2, cd target);
bool isPointInTriangle2D(const vector<cd> &triangle, cd target);
vector<cd> normalizeBasePoints(const vector<cd>& points);
bool crossSegmentAndCircle(cd p0, cd p1, cd c, double r);
bool isCircleInTriangle(const vector<cd> &points, cd target, double r);
bool isCircleInBox(const vector<cd> &points, cd target, double r);
bool isCircleInTriangleOrBox(const vector<cd> &points, cd target, double r);
bool isStable(const vector<point> &points);
pair<double, double> solve(const int* input);

int ROTATION[ROTATION_SIZE][POINT_SIZE] = {
	{A, B, D, E, C, F, G},
	{A, B, E, D, C, F, G},
	{B, C, D, E, A, F, G},
	{B, C, E, D, A, F, G},
	{C, A, D, E, B, F, G},
	{C, A, E, D, B, F, G},
	
	{A, D, E, B, C, F, G},
	{B, D, E, C, A, F, G},
	{C, D, E, A, B, F, G},
};

point makePoint(double x, double y, double z) {
	point p;
	p.push_back(x);
	p.push_back(y);
	p.push_back(z);
	return p;
}

void printPoint(const point& p) {
	printf("(%6.3f,%6.3f,%6.3f)\n", p[0], p[1], p[2]);
}

point addPoints(const point& a, const point &b) {
	point p;
	for (int i = 0; i < DIMENSION; i++) {
		p.push_back(a[i] + b[i]);
	}
	return p;
}

//a - b
point subPoints(const point&a, const point &b) {
	point p;
	for (int i = 0; i < DIMENSION; i++) {
		p.push_back(a[i] - b[i]);
	}
	return p;
}

point scalePoint(const point &a, double s) {
	point p;
	for (int i = 0; i < DIMENSION; i++) {
		p.push_back(a[i] * s);
	}
	return p;
}

point outerProductPoints(const point &p1, const point &p2) {
	point p;
	for (int i = 0; i < DIMENSION; i++) {
		int index1 = (i+1)%DIMENSION;
		int index2 = (i+2)%DIMENSION;
		p.push_back(p1[index1]*p2[index2] - p1[index2]*p2[index1]);
	}
	return p;
}

double innerProductPoints(const point &p1, const point &p2) {
	double sum = 0.0;
	for (int i= 0; i < DIMENSION; i++) {
		sum += p1[i]*p2[i];
	}
	return sum;
}

void printRotator(const rotator& r) {
	printf("----------------------\n");
	for (int i = 0; i < DIMENSION; i++) {
		printPoint(r[i]);
	}
	printf("----------------------\n");
}

point rotate(const rotator &r, const point &p) {
	point result;
	for (int i = 0; i < DIMENSION; i++) {
		result.push_back(0.0);
		for (int j = 0; j < DIMENSION; j++) {
			result[i] += r[i][j] * p[j];
		}
	}
	return result;
}

rotator multiplyRotators(const rotator &r1, const rotator &r2) {
	rotator result;
	
	for (int i = 0; i < DIMENSION; i++) {
		result.push_back(vector<double>());
		for (int j = 0; j < DIMENSION; j++) {
			result[i].push_back(0.0);
			for (int k = 0; k < DIMENSION; k++) {
				result[i][j] += r1[i][k] * r2[k][j];
			}
		}
	}
	return result;
}

rotator getZeroRotator() {
	rotator r;
	for (int i = 0; i < DIMENSION; i++) {
		r.push_back(vector<double>());
		for (int j = 0; j < DIMENSION; j++) {
			r[i].push_back(0.0);
		}
	}
	return r;
}

rotator getZAxisRotator(double t) {
	rotator r = getZeroRotator();
	
	r[0][0] = cos(t);
	r[0][1] = sin(t);
	r[1][0] = -sin(t);
	r[1][1] = cos(t);
	r[2][2] = 1.0;
	
	return r;
}

rotator getXAxisRotator(double t) {
	rotator r = getZeroRotator();
	
	r[1][1] = cos(t);
	r[1][2] = sin(t);
	r[2][1] = -sin(t);
	r[2][2] = cos(t);
	r[0][0] = 1.0;
	
	return r;
}


rotator getYAxisRotator(double t) {
	rotator r = getZeroRotator();
	
	r[2][2] = cos(t);
	r[2][0] = sin(t);
	r[0][2] = -sin(t);
	r[0][0] = cos(t);
	r[1][1] = 1.0;
	
	return r;
}

rotator getRotatorPointMoveXAxis(const point &p) {
	double t1 = -atan2(p[2], p[0]);
	rotator r1 = getYAxisRotator(t1);
	point temp = rotate(r1, p);
//	printPoint(temp);
	
	double t2 = -atan2(temp[0], temp[1]) + M_PI/2;
	rotator r2 = getZAxisRotator(t2);
//	printPoint(rotate(r2, temp));
	
	rotator r = multiplyRotators(r2, r1);
	return r;
}

rotator getRotatorPointMoveXYSurface(const point &p) {
	double t = -atan2(p[1], p[2]) + M_PI/2;
	rotator r = getXAxisRotator(t);
	return r;
}

double calculateTetrahedraVolume(const point &a, const point &b, const point &c, const point &d) {
	point ba = subPoints(b, a);
	point ca = subPoints(c, a);
	point da = subPoints(d, a);
	return abs(innerProductPoints(outerProductPoints(ba, ca), da) / 6.0);
}

point calculaTetetrahedraCenter(const point &a, const point &b, const point &c, const point &d) {
	point g = addPoints(a, b);
	g = addPoints(g, c);
	g = addPoints(g, d);
	return scalePoint(g, 0.25);
}

point calculateTwoTetrahedraCenter(const point &a, const point &b, const point &c, const point &d, const point &e) {
	double volumeABCD = calculateTetrahedraVolume(a, b, c, d);
	double volumeABCE = calculateTetrahedraVolume(a, b, c, e);
	point centerABCD = calculaTetetrahedraCenter(a, b, c, d);
	point centerABCE = calculaTetetrahedraCenter(a, b, c, e);
	
	return scalePoint(addPoints(scalePoint(centerABCD, volumeABCD), scalePoint(centerABCE, volumeABCE)), 1.0 / (volumeABCD + volumeABCE));
}


void printPoints(const vector<point> &points) {
	DEBUG;
	for (unsigned int i = 0; i < points.size(); i++) {
		printPoint(points[i]);
	}
}

bool normalizePosition(vector<point> &points) {
	point originalAPosition = points[0];
	
//	printPoints(points);
	for (int i = 0; i < POINT_SIZE; i++) {
		points[i] = subPoints(points[i], originalAPosition);
	}
//	printPoints(points);
	rotator r1 = getRotatorPointMoveXAxis(points[1]);
	for (int i = 0; i < POINT_SIZE; i++) {
		points[i] = rotate(r1, points[i]);
	}
//	printPoints(points);
	if (abs(points[2][1]) < EPS && abs(points[2][2]) < EPS) {
		return false;
	}
	rotator r2 = getRotatorPointMoveXYSurface(points[2]);
	for (int i = 0; i < POINT_SIZE; i++) {
		points[i] = rotate(r2, points[i]);
	}
//	printPoints(points);
	
	double maxHeight = -100000.0;
	double minHeight =  100000.0;
	for (int i = 0; i < POINT_SIZE; i++) {
		maxHeight = max(maxHeight, points[i][2]);
		minHeight = min(minHeight, points[i][2]);
	}
	if (maxHeight > EPS && minHeight < -EPS){
		return false;
	} else if (maxHeight > EPS) {
		return true;
	} else if (minHeight < -EPS) {
		for (int i = 0; i < POINT_SIZE; i++) {
			points[i] = scalePoint(points[i], -1.0);
		}
		return true;
	} else {
		//(maxHeight <= EPS && minHeight >= -EPS)
		//maybe input is broken.
		return false;
	}
	
}

bool isPointInTriangle2D(cd p0, cd p1, cd p2, cd target) {
	cd v1 = p1 - p0;
	cd v2 = p2 - p0;
	cd t = target - p0;
	
	if (abs(imag(v1/v2)) < EPS) {
		return true;
	}
	double a1 = imag(t/v2) / imag(v1/v2);
	double a2 = imag(t/v1) / imag(v2/v1);
	
	return a1 > -EPS && a2 > -EPS && a1 + a2 < 1.0 + EPS;
}

bool isPointInTriangle2D(const vector<cd> &triangle, cd target) {
	return isPointInTriangle2D(triangle[0], triangle[1], triangle[2], target);
}


vector<cd> normalizeBasePoints(const vector<cd>& points) {
	if (points.size() == 3) {
		return points;
	}
	
	for (int i = 0; i < 4; i++) {
		vector<cd> triangle;
		for (int j = 0; j < 4; j++) {
			if (j == i) continue;
			triangle.push_back(points[j]);
		}
		if (isPointInTriangle2D(triangle, points[i])) {
			return triangle;
		}
	}
	
	for (int i = 1; i < 4; i++) {
		vector<int> restOfIndex;
		for (int j = 1; j < 4; j++) {
			if (j == i) continue;
			restOfIndex.push_back(j);
		}
		vector<cd> output;
		output.push_back(points[0]);
		output.push_back(points[restOfIndex[0]]);
		output.push_back(points[i]);
		output.push_back(points[restOfIndex[1]]);
		
		cd v1 = output[1] - output[0];
		cd v2 = output[2] - output[0];
		cd v3 = output[3] - output[0];
		
		cd temp1 = v1 / v2;
		cd temp3 = v3 / v2;
		
		if (imag(temp1) * imag(temp3) < 0.0) {
			return output;
		}
	}
	// maybe error
	return points;
}

bool crossSegmentAndCircle(cd p0, cd p1, cd c, double r) {
	if (abs(p0 - c) < r) {
		return true;
	}
	if (abs(p1 - c) < r) {
		return true;
	}
	
	cd v1 = p1 - p0;
	cd vc = c - p0;
	
	double lenv1 = abs(v1);
	cd normv1 = v1 / lenv1;
	cd normvc = vc / normv1;
	
	return 0.0 < real(normvc) && real(normvc) < lenv1 && abs(imag(normvc)) < r;
}

bool isCircleInTriangle(const vector<cd> &points, cd target, double r) {
	if (!isPointInTriangle2D(points, target)) {
		return false;
	}
	
	for (int i = 0; i < 3; i++) {
		int index0 = i;
		int index1 = (i+1) % 3;
		if (crossSegmentAndCircle(points[index0], points[index1], target, r)) {
			return false;
		}
	}
	return true;
}

bool isPointInPolygon(const vector<cd> &points, cd target) {
	double minArea = 1.0;
	double maxArea = -1.0;
	
	for (int i = 0; i < points.size(); i++) {
		int index0 = i;
		int index1 = (i+1) % points.size();
		cd v0 = points[index0] - target;
		cd v1 = points[index1] - target;
		double area = imag(v1/v0);
		minArea = min(minArea, area);
		maxArea = max(maxArea, area);
//		cout << area << endl;
	}
	
	return minArea * maxArea >= 0.0;
}

bool isCircleInBox(const vector<cd> &points, cd target, double r) {
	if (!isPointInPolygon(points, target)) {
//		DEBUG;
		return false;
	}
	/*
	if (!isPointInTriangle2D(points[0], points[1], points[2], target)){
		return false;
	}
	if (!isPointInTriangle2D(points[2], points[3], points[0], target)){
		return false;
	}
	*/
	
	for (int i = 0; i < 4; i++) {
		int index0 = i;
		int index1 = (i+1) % 4;
		if (crossSegmentAndCircle(points[index0], points[index1], target, r)) {
			return false;
		}
	}
	return true;
}

bool isCircleInTriangleOrBox(const vector<cd> &points, cd target, double r) {
	if (points.size() == 3) {
		return isCircleInTriangle(points, target, r);
	} else if (points.size() == 4) {
		return isCircleInBox(points, target, r);
	} else {
		//maybe error
		return false;
	}
}

bool isStable(const vector<point> &points) {
	vector<cd> basePoints;
	for (int i = 0; i < 5; i++) {
		if (abs(points[i][2]) < EPS) {
			basePoints.push_back(cd(points[i][0], points[i][1]));
		}
	}
	basePoints = normalizeBasePoints(basePoints);
	
	cd g = cd(points[6][0], points[6][1]);
	
	return isCircleInTriangleOrBox(basePoints, g, 0.2);
}

pair<double, double> solve(const int* input) {
	vector<point> points;
	for (int i = 0; i < 18; i+=3) {
		points.push_back(makePoint(input[i+0], input[i+1], input[i+2]));
	}
	points.push_back(calculateTwoTetrahedraCenter(points[0],points[1],points[2],points[3],points[4]));
	
	double minHeight = 100000.0;
	double maxHeight = -1.0;
	for (int i = 0; i < ROTATION_SIZE; i++) {
		vector<point> rotated;
		for (int j = 0; j < POINT_SIZE; j++) {
			rotated.push_back(points[ROTATION[i][j]]);
		}
		if (!normalizePosition(rotated)) {
			continue;
		}
		
		if (isStable(rotated)) {
			minHeight = min(minHeight, rotated[5][2]);
			maxHeight = max(maxHeight, rotated[5][2]);
		}
	}
	
	return make_pair(minHeight, maxHeight);
}

int main(void) {
	for (int casenumber = 1; true; casenumber++) {
		int input[18];
		for (int i = 0; i < 18; i++) {
			if (!(cin >> input[i])) {
				return 0;
			}
		}
		pair<double, double> answer = solve(input);
		printf("Case %d: %.5f %.5f\n", casenumber, answer.first, answer.second);
	}
}
