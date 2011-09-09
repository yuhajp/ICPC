#include<iostream>
#include<cmath>
#include<complex>
#include<vector>

using namespace std;

typedef complex<double> cd;
typedef pair<cd, cd> line;

typedef struct arc {
	cd center;
	double radius;
	cd p1;
	cd p2;
	double thetaMin;
	double thetaMax;
	double sign;
} ARC;

typedef pair<vector<line>, vector<ARC> > parsed;

#define EPS 1e-8

ARC makeArc(cd p1, cd p2, int r) {
	ARC arc;
	arc.radius = abs(r);
	arc.p1 = p1;
	arc.p2 = p2;
	double h = sqrt(r*r - norm(0.5*(p2-p1)));
	cd v = (p2 - p1) * cd(0, 1);
	v /= abs(v);
	arc.sign = r < 0 ? 1 : -1;
	arc.center = 0.5*(p1+p2) + arc.sign*v*h;
	if(arc.sign > 0) {
		arc.thetaMin = arg(p1 - arc.center);
		arc.thetaMax = arg(p2 - arc.center);
	} else {
		arc.thetaMin = arg(p2 - arc.center);
		arc.thetaMax = arg(p1 - arc.center);
	}
	if(arc.thetaMin > arc.thetaMax) {
		arc.thetaMax += 2.0*M_PI;
	}
	return arc;
}

parsed parse() {
	int startx, starty;
	cin >> startx >> starty;
	cd start = cd(startx, starty);
	cd p = start;
	vector<line> lines;
	vector<ARC> arcs;
	while(true) {
		string command;
		cin >> command;
		if(command == "line") {
			int x, y;
			cin >> x >> y;
			cd nextp = cd(x, y);
			lines.push_back(make_pair(p, nextp));
			p = nextp;
		} else if(command == "arc") {
			int x, y, r;
			cin >> x >> y >> r;
			cd nextp = cd(x, y);
			arcs.push_back(makeArc(p, nextp, r));
			p = nextp;
		} else if(command == "close") {
			if(abs(p - start) > EPS) {
				lines.push_back(make_pair(p, start));
			}
			return make_pair(lines, arcs);
		} else {
			cerr << "command error!" << endl;
			exit(-1);
		}
	}
}

double area(cd p1, cd p2) {
	return 0.5*(imag(p2)*real(p1) - real(p2)*imag(p1));
}

double area(line l) {
	return area(l.first, l.second);
}

double area(ARC& arc) {
	return arc.sign * 0.5*arc.radius*arc.radius*(arc.thetaMax - arc.thetaMin);
}

cd centerOfMass(cd p1, cd p2) {
	return area(p1, p2) * (p1 + p2) / 3.0;
}

cd centerOfMass(line l) {
	return centerOfMass(l.first, l.second);
}

cd centerOfMass(ARC& arc) {
	return area(arc) * (cd(0, -2.0/3.0) * arc.sign * (arc.p2 - arc.p1) / (arc.thetaMax - arc.thetaMin) + arc.center);
}

cd centerOfMass(parsed p) {
	double sumArea = 0.0;
	cd sumCenterOfMass = cd(0.0, 0.0);
	for(vector<line>::iterator it = p.first.begin(); it != p.first.end(); it++) {
		sumArea += area(*it);
		sumCenterOfMass += centerOfMass(*it);
	}
	for(vector<ARC>::iterator it = p.second.begin(); it != p.second.end(); it++) {
		sumArea += area(it->p1, it->center);
		sumCenterOfMass += centerOfMass(it->p1, it->center);
		sumArea += area(it->center, it->p2);
		sumCenterOfMass += centerOfMass(it->center, it->p2);
		sumArea += area(*it);
		sumCenterOfMass += centerOfMass(*it);
	}
	return sumCenterOfMass / sumArea;
}

#define ERROR -1
#define IN 0
#define OUT 1

bool online(cd c, cd v, cd target) {
	target -= c;
	target /= v;
	return abs(imag(target)) < EPS && -EPS < real(target);
}

int countCross(line l, cd c, cd v) {
	if(online(c, v, l.first))return ERROR;
	if(online(c, v, l.second))return ERROR;
	cd p1 = l.first;
	cd v1 = l.second - l.first;
	cd p2 = c;
	cd v2 = v;
	if(abs(imag(v2/v1)) < EPS)return 0;
	double a1 = imag((p2-p1)/v2) / imag(v1/v2);
	double a2 = imag((p1-p2)/v1) / imag(v2/v1);
	if(-EPS < a1 && a1 < 1.0+EPS && -EPS < a2)return 1;
	return 0;
}

//|c+tv-arc.center| = arc.radius

int countCross(ARC& arc, cd c, cd v) {
	if(online(c, v, arc.p1))return ERROR;
	if(online(c, v, arc.p2))return ERROR;
	c -= arc.center;
	double r = arc.radius;
	double a2 = norm(v);
	double a1 = real(v*conj(c));
	double a0 = norm(c) - r*r;
	a1 /= a2;
	a0 /= a2;
	double d = a1*a1 - a0;
	if(d < 0.0)return 0;
	int count = 0;
	double sign[] = {1.0, -1.0};
	for(int i = 0; i < 2; i++) {
		double t = -a1 + sign[i] * sqrt(d);
		if(t < 0.0)continue;
		double theta = arg(c + t*v);
		if(arc.thetaMin < theta && theta < arc.thetaMax)count++;
		if(arc.thetaMin < theta+2.0*M_PI && theta+2.0*M_PI < arc.thetaMax)count++;
	}
	return count;
}

int includeSub(parsed p, cd c, cd v) {
	int count = 0;
	for(vector<line>::iterator it = p.first.begin(); it != p.first.end(); it++) {
		int num = countCross(*it, c, v);
		if(num == ERROR)return ERROR;
		count += num;
	}
	for(vector<ARC>::iterator it = p.second.begin(); it != p.second.end(); it++) {
		int num = countCross(*it, c, v);
		if(num == ERROR)return ERROR;
		count += num;
	}
	return (count % 2 == 0) ? OUT : IN;
}

char include(parsed p, cd c) {
	while(true) {
		cd v = polar(1.0, 2.0 * M_PI * rand() / RAND_MAX);
		int result = includeSub(p, c, v);
		switch(result) {
		case IN:
			return '+';
		case OUT:
			return '-';
		}
	}
}

void solve() {
	parsed p = parse();
	cd c = centerOfMass(p);
	printf("%.4f %.4f %c\n", real(c), imag(c), include(p, c));
}

int main(void) {
	srand(time(NULL));
	while(true) {
		string command;
		cin >> command;
		if(command == "start") {
			solve();
		} else if(command == "end") {
			break;
		} else {
			cerr << "command error!" << endl;
			break;
		}
	}
}
