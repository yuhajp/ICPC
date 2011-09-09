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
