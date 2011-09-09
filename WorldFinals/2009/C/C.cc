#include<algorithm>
#include<cmath>
#include<complex>
#include<cstdio>
#include<iostream>
#include<utility>

using namespace std;

#define EPS 1e-8
#define EDGE 10
#ifndef M_PI
	#define M_PI 3.1415926535897932384626433832795028841971
#endif
#define HALF_M_PI (0.5*M_PI)
#define RANGE(degree, mini, maxi) (((double)(mini)*0.5*M_PI-EPS) <= (degree) && (degree) <= ((double)(maxi)*0.5*M_PI+EPS))

typedef struct Pos3D{
	double x, y, z;
} pos3D;


typedef complex<double> cd;

//spherical coordinate(theta, phi)
typedef pair<double,double> sc;

sc mirror(sc s) {
	s.second = M_PI - s.second;
	return s;
}

//-3 <= r <= 3
sc rotate(sc s, int r) {
	s.first += r*HALF_M_PI;
	if(s.first > 2.0*M_PI + EPS) {
		s.first -= 2.0*M_PI;
	}
	if(s.first < -EPS) {
		s.first += 2.0*M_PI;
	}
	return s;
}

pos3D getPos3D(sc s) {
	double x = cos(s.first) * sin(s.second);
	double y = sin(s.first) * sin(s.second);
	double z = cos(s.second);
	
	double sum = abs(x) + abs(y) + abs(z);
	double rate = (double)EDGE / sqrt(2.0) / sum;
	
	pos3D p;
	p.x = rate * x;
	p.y = rate * y;
	p.z = rate * z;
	return p;
}

/*
	x >= 0, y >= 0, z >= 0‚É‚ ‚é ³”ª–Ê‘Ì‚Ì–Ê‚ğ
	(   0,    0, 5ã2)‚ª( 0,  0)
	(5ã2,    0,    0)‚ª(10,  0)
	(   0, 5ã2,    0)‚ª( 0, 10)
	‚É‚È‚é‚æ‚¤‚ÉˆÚ“®‚³‚¹‚½‚Æ‚«‚É s ‚Ì•\‚·“_‚ª‚»‚ÌˆÚ“®‚Å•¡‘f•½–Ê‚Ì‚Ç‚±‚É‚­‚é‚©‚ğ•Ô‚·
	
	s ‚ªx >= 0, y >= 0, z >= 0 ã‚É–³‚¢‚Æ‚«‚à
	z < 0 ‚Ìê‡‚Í x²’†S‚É‰ñ“]ˆÚ“®‚µA
	x >= 0 y >= 0 ‚É‚È‚¢ê‡‚Í x == y == 0 ‚Ì²‚ÉŠÖ‚µ‚Ä‰ñ“]ˆÚ“®‚µ‚Ä
	x >= 0, y >= 0, z >= 0 ‚É‚à‚Á‚Ä‚«‚Äl‚¦‚é
*/

cd getTriangleCoordinate(sc s, bool flip, int rotate) {
	if(flip) {
		s.second = M_PI - s.second;
		s.first = 2.0*M_PI - s.first;
	}
	s.first += rotate * HALF_M_PI;
	
	pos3D p = getPos3D(s);
	return polar(p.x*sqrt(2.0), 0.0) + polar(p.y*sqrt(2.0), M_PI/3.0);
}

/*
	xy•½–Êã‚Ì•ûŒü‚ğ x²³‚Ì•ûŒü‚©‚ç‚Ì‘å‚«‚³‚ª s.first
	z²‚©‚ç‚ÌŒX‚«‚ª s.second
	
	‚©‚ç
	
	yz•½–Êã‚Ì•ûŒü‚ğ x²³‚Ì•ûŒü‚©‚ç‚Ì‘å‚«‚³‚ª s.first
	x²‚©‚ç‚ÌŒX‚«‚ª s.second
	
	‚É•ÏŠ·‚·‚é
*/

sc changeAxis(sc s) {
	double x = cos(s.first) * sin(s.second);
	double y = sin(s.first) * sin(s.second);
	double z = cos(s.second);
	
	// y = cos(s.first) * sin(s.second);
	// z = sin(s.first) * sin(s.second);
	// x = cos(s.second);
	
	s.second = acos(x);
	
	// cos(s.first) = y / sin(s.second);
	// sin(s.first) = z / sin(s.second);
	if(abs(sin(s.second)) < EPS) {
		s.first = 0.0;
	} else {
		s.first = atan2(z/sin(s.second), y/sin(s.second));
	}
	
	if(s.first < -EPS) {
		s.first += 2.0*M_PI;
	}
	
	return s;
}

double solve3HopFaceThroughYZPlainZXPlain(sc s1, sc s2) {
	cd t1 = getTriangleCoordinate(s1, false, 0);
	cd t2 = getTriangleCoordinate(s2, true, -1);
	
	cd p2 =  - 10.0 + polar(10.0, 2.0*M_PI/3.0) + t2 * polar(1.0, -M_PI/3.0);
	
	return abs(t1 - p2);
}

double solve3HopFaceThrouhYZPlain(sc s1, sc s2) {
	double len1 = solve3HopFaceThroughYZPlainZXPlain(s1, s2);
	
	s1 = mirror(s1);
	s2 = mirror(s2);
	s1 = rotate(s1, 2);
	s2 = rotate(s2, 2);
	double len2 = solve3HopFaceThroughYZPlainZXPlain(s2, s1);
	return min(len1, len2);
}


//theta1	[  0,  90]
//phi1		[  0,  90]
//theta2	[180, 270]
//phi2		[ 90, 180]
double solve3HopFace(sc s1, sc s2) {
	double output = 1000.0;
	
	output = min(output, solve3HopFaceThrouhYZPlain(s1, s2));
	
	s1 = changeAxis(s1);
	s2 = changeAxis(s2);
	output = min(output, solve3HopFaceThrouhYZPlain(s1, s2));
	
	s1 = changeAxis(s1);
	s2 = changeAxis(s2);
	output = min(output, solve3HopFaceThrouhYZPlain(s1, s2));
	
	return output;
}

double solve2HopFaceByCounterClockwise(sc s1, sc s2) {
	cd t1 = getTriangleCoordinate(s1, false, 0);
	cd t2 = getTriangleCoordinate(s2, false, -2);
	
	return abs(t1 - t2 * polar(1.0, 2.0*M_PI/3.0));
}

//theta1	[  0,  90]
//phi1		[  0,  90]
//theta2	[180, 270]
//phi2		[  0,  90]
double solve2HopFace(sc s1, sc s2) {
	double len1 = solve2HopFaceByCounterClockwise(s1, s2);
	
	s1 = rotate(s1, 2);
	s2 = rotate(s2, 2);
	double len2 = solve2HopFaceByCounterClockwise(s2, s1);
	
	return min(len1, len2);
}


double solve1HopFace(sc s1, sc s2) {
	cd t1 = getTriangleCoordinate(s1, false, 0);
	cd t2 = getTriangleCoordinate(s2, false, -1);
	
	return abs(t1 - t2 * polar(1.0, M_PI/3.0));
}

//theta1	[  0,  90]
//phi1		[  0,  90]
//theta2	[  0,  90]
//phi2		[  0,  90]
double solve0HopFace(sc s1, sc s2) {
	pos3D pos1 = getPos3D(s1);
	pos3D pos2 = getPos3D(s2);
	
	double dx = pos1.x - pos2.x;
	double dy = pos1.y - pos2.y;
	double dz = pos1.z - pos2.z;
	
	return sqrt(dx*dx + dy*dy + dz*dz);
}

//theta1	[  0,  90]
//phi1		[  0,  90]
double solveNormalizedInput(sc s1, sc s2) {
	if(RANGE(s2.first, 0, 1) && s2.second <= HALF_M_PI) {		// 0 Hop
		return solve0HopFace(s1, s2);
	} else if(RANGE(s2.first, 1, 2) && s2.second <= HALF_M_PI) {	// 1 Hop
		return solve1HopFace(s1, s2);
	} else if(RANGE(s2.first, 3, 4) && s2.second <= HALF_M_PI) {
		s1 = rotate(s1, 1);
		s2 = rotate(s2, -3);
		return solve1HopFace(s2, s1);
	} else if(RANGE(s2.first, 0, 1) && s2.second >= HALF_M_PI) {
		s1 = changeAxis(s1);
		s2 = changeAxis(s2);
		
		s1 = rotate(s1, 1);
		s2 = rotate(s2, -3);
		return solve1HopFace(s2, s1);
	} else if(RANGE(s2.first, 2, 3) && s2.second <= HALF_M_PI) {	// 2 Hop
		return solve2HopFace(s1, s2);
	} else if(RANGE(s2.first, 3, 4) && s2.second >= HALF_M_PI) {
		s1 = changeAxis(s1);
		s2 = changeAxis(s2);
		
		return solve2HopFace(s1, s2);
	} else if(RANGE(s2.first, 1, 2) && s2.second >= HALF_M_PI) {
		s1 = changeAxis(s1);
		s2 = changeAxis(s2);
		
		s1 = changeAxis(s1);
		s2 = changeAxis(s2);
		
		return solve2HopFace(s1, s2);
	} else if(RANGE(s2.first, 2, 3) && s2.second >= HALF_M_PI) {	// 3 Hop
		return solve3HopFace(s1, s2);
	} else {
		//error
		return -1.0;
	}
}

double solve(sc s1, sc s2) {
	if(s1.second >= HALF_M_PI) {
		s1 = mirror(s1);
		s2 = mirror(s2);
	}
	
	while(s1.first > HALF_M_PI + EPS) {
		s1 = rotate(s1, 1);
		s2 = rotate(s2, 1);
	}
	return solveNormalizedInput(s1, s2);
}


double getRadian(int x) {
	return (double)x / 180.0 * M_PI;
}

int main(void) {
	for(int caseNumber = 1; true; caseNumber++) {
	int theta1, phi1, theta2, phi2;
		cin >> theta1 >> phi1 >> theta2 >> phi2;
		if(theta1 == -1 && phi1 == -1 && theta2 == -1 && phi2 == -1) break;
		
		double answer = solve(sc(getRadian(theta1), getRadian(phi1)), sc(getRadian(theta2), getRadian(phi2)));
		printf("Case %d: %.3f\n", caseNumber, answer);
	}
}
