#include<iostream>
#include<cstdlib>
#include<complex>
#include<vector>
#include<queue>
#include<algorithm>

using namespace std;

typedef complex<double> cd;
typedef pair<cd, cd> Line;

typedef struct edge {
	Line line;
	int connectedFaceIndex;
} Edge;

typedef vector<Edge> Polygon;

typedef struct face {
	Polygon edges;		//all points must be counter-clockwise
	bool visible;
	bool visited;
} Face;

#define NULL_INDEX	-1

#define EPS		1e-6
#define INF			1e+8
#define NOT_POINT	cd(INF, INF)

#define DEBUG cout<<"DEBUG:"<<__LINE__<<endl

void printLine(const Line &line) {
	cout << "[" << line.first << " - " << line.second << "]";
}

void printEdge(const Edge &e) {
	cout << "(" << e.connectedFaceIndex << ") ";
	printLine(e.line);
}

void printPolygon(const Polygon &p) {
	for (Polygon::const_iterator it = p.begin(); it != p.end(); it++) {
		printEdge(*it);
		cout << endl;
	}
}

void printFace(const Face &face) {
	DEBUG;
	cout << "Visible:" << face.visible << endl;
	cout << "Visited:" << face.visited << endl;
	if (face.visible) {
		printPolygon(face.edges);
	}
}

void printFaces(const Face* faces, int facesSize) {
	for (int i = 0; i < facesSize; i++) {
		printFace(faces[i]);
	}
}

Edge makeEdge(cd p1, cd p2, int index) {
	Edge e;
	e.line = make_pair(p1, p2);
	e.connectedFaceIndex = index;
	return e;
}

Edge makeEdge(double x1, double y1, double x2, double y2, int index) {
	return makeEdge(cd(x1, y1), cd(x2, y2), index);
}

Edge makeEdge(double x1, double y1, double x2, double y2) {
	return makeEdge(x1, y1, x2, y2, NULL_INDEX);
}


void setInitialFace(Face &face) {
	face.visible = true;
	face.edges.push_back(makeEdge(  0.0,   0.0, 100.0,   0.0));
	face.edges.push_back(makeEdge(100.0,   0.0, 100.0, 100.0));
	face.edges.push_back(makeEdge(100.0, 100.0,   0.0, 100.0));
	face.edges.push_back(makeEdge(  0.0, 100.0,   0.0,   0.0));
}

bool isCounterClockwise(cd p, Line l) {
	cd p1 = l.first - p;
	cd p2 = l.second - p;
	return imag(p2 / p1) >= 0.0;
}

bool insidePoint(const Polygon &poly, cd p) {
	for (Polygon::const_iterator it = poly.begin(); it != poly.end(); it++) {
		if (!isCounterClockwise(p, it->line)) {
			return false;
		}
	}
	return true;
}

/*
	getHoldedFaceIndex(0,1)	->	1
	getHoldedFaceIndex(0,2)	->	3
	getHoldedFaceIndex(1,2)	->	2
	getHoldedFaceIndex(0,4)	->	7
	getHoldedFaceIndex(1,4)	->	6
	getHoldedFaceIndex(2,4)	->	5
	getHoldedFaceIndex(3,4)	->	4
*/
int getHoldedFaceIndex(int index, int facesSize) {
	if (index == NULL_INDEX) {
		return NULL_INDEX;
	}
	return 2*facesSize - index - 1;
}

bool isPSide(cd target, Line line) {
	cd v = line.second - line.first;
	cd vTarget = target - line.first;
	return imag(vTarget / v) > 0.0;
}


//p1+a1*v1 = p2+a2*v2
//a1*v1 = p2-p1+a2*v2
//a1*v1/v2 = (p2-p1)/v2+a2
//a1*imag(v1/v2) = imag((p2-p1)/v2)
cd crossLinePoint(Line line1, Line line2) {
	cd p1 = line1.first;
	cd v1 = line1.second - line1.first;
	cd p2 = line2.first;
	cd v2 = line2.second - line2.first;
	
	if (abs(imag(v1/v2)) < EPS) {
		return NOT_POINT;
	}
	
	
	double a1 = imag((p2-p1)/v2) / imag(v1/v2);
	double a2 = imag((p1-p2)/v1) / imag(v2/v1);
	
	return p1+a1*v1;
}

bool crossSegment(Line line1, Line line2) {
	cd p1 = line1.first;
	cd v1 = line1.second - line1.first;
	cd p2 = line2.first;
	cd v2 = line2.second - line2.first;
	
	if (abs(imag(v1/v2)) < EPS) {
		return false;
	}
	
	double a1 = imag((p2-p1)/v2) / imag(v1/v2);
	double a2 = imag((p1-p2)/v1) / imag(v2/v1);
	return 0.0 < a1 && a1 < 1.0 && 0.0 < a2 && a2 < 1.0;
}

double edgeLength(const Edge &e) {
	return abs(e.line.second - e.line.first);
}

void cut(int srcIndex, Face *faces, int facesSize, cd p, Line line) {
	int dstIndex = getHoldedFaceIndex(srcIndex, facesSize);
	
	Polygon pSideNextPolygon;
	Polygon qSideNextPolygon;
	Edge pSideEdge;
	Edge qSideEdge;
	pSideEdge = makeEdge(NOT_POINT, NOT_POINT, srcIndex);
	qSideEdge = makeEdge(NOT_POINT, NOT_POINT, dstIndex);
	
	bool visitTF = false;
	bool visitFT = false;
	for (Polygon::const_iterator it = faces[srcIndex].edges.begin(); it != faces[srcIndex].edges.end(); it++) {
		cd pf = it->line.first;
		cd ps = it->line.second;
		bool pfPSide = isPSide(pf, line);
		bool psPSide = isPSide(ps, line);
		
		if (pfPSide && psPSide) {
			pSideNextPolygon.push_back(*it);
		} else if (pfPSide && !psPSide) {
			cd crossPoint = crossLinePoint(line, it->line);
			
			pSideEdge.line.first = crossPoint;
			qSideEdge.line.second = crossPoint;
			
			pSideNextPolygon.push_back(makeEdge(pf, crossPoint, it->connectedFaceIndex));
			if (visitFT) {
				pSideNextPolygon.push_back(pSideEdge);
				qSideNextPolygon.push_back(qSideEdge);
			}
			qSideNextPolygon.push_back(makeEdge(crossPoint, ps, it->connectedFaceIndex));
			
			visitTF = true;
		} else if (!pfPSide && psPSide) {
			cd crossPoint = crossLinePoint(line, it->line);
			
			pSideEdge.line.second = crossPoint;
			qSideEdge.line.first = crossPoint;
			
			qSideNextPolygon.push_back(makeEdge(pf, crossPoint, it->connectedFaceIndex));
			if (visitTF) {
				pSideNextPolygon.push_back(pSideEdge);
				qSideNextPolygon.push_back(qSideEdge);
			}
			pSideNextPolygon.push_back(makeEdge(crossPoint, ps, it->connectedFaceIndex));
			
			visitFT = true;
		} else {	//!pfPSide && !psPSide
			qSideNextPolygon.push_back(*it);
		}
	}
	
	faces[srcIndex].edges = qSideNextPolygon;
	if (qSideNextPolygon.size() == 0) {
		faces[srcIndex].visible = false;
	}
	faces[dstIndex].edges = pSideNextPolygon;
	faces[dstIndex].visible = true;
}

cd innerPoint(const Polygon &poly) {
	return (poly[0].line.first + poly[1].line.first + poly[2].line.first) / 3.0;
}

bool commonPoint(const Polygon &poly1, const Polygon &poly2) {
	for (Polygon::const_iterator it1 = poly1.begin(); it1 != poly1.end(); it1++) {
		for (Polygon::const_iterator it2 = poly2.begin(); it2 != poly2.end(); it2++) {
			if (crossSegment(it1->line, it2->line)) {
				return true;
			}
		}
	}
	
	return insidePoint(poly1, innerPoint(poly2)) || insidePoint(poly2, innerPoint(poly1));
}

void cutDfs(int srcIndex, Face *faces, int facesSize, cd p, Line line) {
	if (faces[srcIndex].visited) {
		return;
	}
	faces[srcIndex].visited = true;
	if (!faces[srcIndex].visible) {
		return;
	}
	
	cut(srcIndex, faces, facesSize, p, line);
	int dstIndex = getHoldedFaceIndex(srcIndex, facesSize);
	
	for (Polygon::const_iterator it = faces[dstIndex].edges.begin(); it != faces[dstIndex].edges.end(); it++) {
		int nextIndex = it->connectedFaceIndex;
		if (nextIndex != NULL_INDEX && !faces[nextIndex].visited && faces[nextIndex].visible) {
			cutDfs(nextIndex, faces, facesSize, p, line);
		}
	}
	for (int i = srcIndex+1; i < facesSize; i++) {
		if (faces[i].visible && !faces[i].visited && commonPoint(faces[dstIndex].edges, faces[i].edges)) {
			cutDfs(i, faces, facesSize, p, line);
		}
	}
}

int getTopFaceIncludingPointIndex(const Face *faces, int facesSize, cd p) {
	for (int i = facesSize - 1; i >= 0; i--) {
		if (faces[i].visible && insidePoint(faces[i].edges, p)) {
			return i;
		}
	}
	//maybe error
	DEBUG;
	return NULL_INDEX;
}

void resetVisitedFlags(Face *faces, int facesSize) {
	for (int i = 0; i < facesSize; i++) {
		faces[i].visited = false;
	}
}

Line makePerpendicularBisectorLine(cd p, cd q) {
	cd center = (p + q) / 2.0;
	cd v = p - center;
	v *= cd(0, -1);
	return make_pair(center, center + v);
}

cd holdCd(cd p, Line line) {
	cd vp = p - line.first;
	cd v = line.second - line.first;
	
	return conj(vp / v) * v + line.first;
}

Edge holdEdge(Edge e, int myselfIndex, int facesSize, Line line) {
	int holdedIndex = getHoldedFaceIndex(e.connectedFaceIndex, facesSize);
	if (holdedIndex == myselfIndex) {
		holdedIndex = e.connectedFaceIndex;
	}
	
	return makeEdge(holdCd(e.line.second, line), holdCd(e.line.first, line), holdedIndex);
}

void holdFace(Face &face, int myselfIndex, int facesSize, Line line) {
	for (Polygon::iterator it = face.edges.begin(); it != face.edges.end(); it++) {
		*it = holdEdge(*it, myselfIndex, facesSize, line);
	}
	reverse(face.edges.begin(), face.edges.end());
}

void holdFaces(Face *faces, int facesSize, Line line) {
	for (int i = facesSize; i < 2*facesSize; i++) {
		if (faces[i].visible) {
			holdFace(faces[i], i, facesSize, line);
		}
	}
}

int countPinHoles(Face *faces, int facesSize, cd p) {
	int count = 0;
	for (int i = 0; i < facesSize; i++) {
		if (faces[i].visible && insidePoint(faces[i].edges, p)) {
			count++;
		}
	}
	return count;
}

int main(void) {
	while (true) {
		int n;
		cin >> n;
		if (n == 0) {
			break;
		}
		
		Face faces[1<<n];
		for (int i = 0; i < (1<<n); i++) {
			faces[i].edges.clear();
			faces[i].visible = false;
		}
		setInitialFace(faces[0]);
		
		for (int i = 0; i < n; i++) {
			int px, py, qx, qy;
			cin >> px >> py >> qx >> qy;
			cd p = cd(px, py);
			cd q = cd(qx, qy);
			int facesSize = (1 << i);
			resetVisitedFlags(faces, facesSize);
			Line line = makePerpendicularBisectorLine(p, q);
			cutDfs(getTopFaceIncludingPointIndex(faces, facesSize, p), faces, facesSize, p, line);
			holdFaces(faces, facesSize, line);
		}
		int x, y;
		cin >> x >> y;
		cout << countPinHoles(faces, 1<<n, cd(x, y)) << endl;
	}
}
