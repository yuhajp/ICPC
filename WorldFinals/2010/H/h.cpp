#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<set>
#include<complex>
#include<algorithm>
#include<queue>

using namespace std;

typedef struct Point {
	int x;
	int y;
	int h;
} Point;

typedef struct Side {
	int pointIndexes[2];
	int faceIndexes[2];
} Side;

typedef struct face {
	int pointIndexes[3];
	int sideIndexes[3];
	int color;
} Face;

#define DEBUG cout<<"DEBUG: "<<__LINE__<<endl

#define UNVISITED_COLOR -1

#define NULL_INDEX -1

void printPoint(int pointIndex, const Point* points, bool linebreak) {
	cout << "(" << points[pointIndex].x << ", " << points[pointIndex].y << ", " << points[pointIndex].h << ")";
	if (linebreak) {
		cout << endl;
	}
}

void printFace(int faceIndex, const Face* faces, const Point* points) {
	printPoint(faces[faceIndex].pointIndexes[0], points, false);
	cout << " - ";
	printPoint(faces[faceIndex].pointIndexes[1], points, false);
	cout << " - ";
	printPoint(faces[faceIndex].pointIndexes[2], points, false);
	cout << ", " << faces[faceIndex].color << endl;
}

void printFaces(const Face* faces, int facesNumber, const Point* points) {
	for (int i = 0; i < facesNumber; i++) {
		printFace(i, faces, points);
	}
}

void readPoints(int pointsNumber, Point* points, map<string, int> &pointNameToIndexMap) {
	for (int i = 0; i < pointsNumber; i++) {
		string name;
		cin >> name >> points[i].x >> points[i].y >> points[i].h;
		pointNameToIndexMap[name] = i;
	}
}

void readSides(int sidesNumber, map<string, int> &pointNameToIndexMap, Side *sides, map<pair<int, int>, int> &twoPointsToSideIndexMap, vector<vector<int> > &sidesGraph) {
	for (int i = 0; i < sidesNumber; i++) {
		string name1, name2;
		cin >> name1 >> name2;
		int index1 = pointNameToIndexMap[name1];
		int index2 = pointNameToIndexMap[name2];
		
		pair<int, int> p = make_pair(min(index1, index2), max(index1, index2));
		
		sides[i].pointIndexes[0] = index1;
		sides[i].pointIndexes[1] = index2;
		sides[i].faceIndexes[0] = NULL_INDEX;
		sides[i].faceIndexes[1] = NULL_INDEX;
		
		twoPointsToSideIndexMap[p] = i;
		
		sidesGraph[index1].push_back(index2);
		sidesGraph[index2].push_back(index1);
	}
}

int findSideIndexFromTwoPointsIndex(int pointIndex1, int pointIndex2, const map<pair<int, int>, int> &twoPointsToSideIndexMap) {
	map<pair<int, int>, int>::const_iterator it = twoPointsToSideIndexMap.find(make_pair(min(pointIndex1, pointIndex2), max(pointIndex1, pointIndex2)));
	if (it != twoPointsToSideIndexMap.end()) {
		return it->second;
	}
	return NULL_INDEX;
}

void addFaceIndexToSide(Side *sides, int sideIndex, int faceIndex) {
	if (sides[sideIndex].faceIndexes[0] == NULL_INDEX) {
		sides[sideIndex].faceIndexes[0] = faceIndex;
	} else {
		sides[sideIndex].faceIndexes[1] = faceIndex;
	}
}

int generateFaces(Face *faces, int pointsNumber, const Point* points, int sidesNumber, Side *sides, map<pair<int, int>, int> &twoPointsToSideIndexMap, vector<vector<int> > &sidesGraph) {
	
	pair<pair<int, int>, int> arrayXYPointIndex[pointsNumber];
	for (int i = 0; i < pointsNumber; i++) {
		arrayXYPointIndex[i].first.first  = points[i].x;
		arrayXYPointIndex[i].first.second = points[i].y;
		arrayXYPointIndex[i].second = i;
	}
	sort(arrayXYPointIndex, arrayXYPointIndex + pointsNumber);
	
	int facesNumber = 0;
	for (int i = 0; i < pointsNumber; i++) {
//		DEBUG;
		int indexSrc = arrayXYPointIndex[i].second;
		int xSrc = points[indexSrc].x;
		int ySrc = points[indexSrc].y;
		
		
		vector<pair<double, int> > pairsArgAndDstIndex;
//		DEBUG;
		for (vector<int>::iterator it = sidesGraph[indexSrc].begin(); it != sidesGraph[indexSrc].end(); it++) {
//		DEBUG;
			int indexDst = *it;
			int xDst = points[indexDst].x;
			int yDst = points[indexDst].y;
			if (xSrc < xDst || (xSrc == xDst && ySrc < yDst)) {
				double arg = atan2(yDst - ySrc, xDst - xSrc);
				pairsArgAndDstIndex.push_back(make_pair(arg, indexDst));
			}
		}
		sort(pairsArgAndDstIndex.begin(), pairsArgAndDstIndex.end());
		
		for (int i = 1; i < pairsArgAndDstIndex.size(); i++) {
//		DEBUG;
			int pointIndex1 = pairsArgAndDstIndex[i-1].second;
			int pointIndex2 = pairsArgAndDstIndex[i].second;
			
			int sideIndex0 = findSideIndexFromTwoPointsIndex(indexSrc, pointIndex1, twoPointsToSideIndexMap);
			int sideIndex1 = findSideIndexFromTwoPointsIndex(indexSrc, pointIndex2, twoPointsToSideIndexMap);
			int sideIndex2 = findSideIndexFromTwoPointsIndex(pointIndex1, pointIndex2, twoPointsToSideIndexMap);
			
			if (sideIndex2 != NULL_INDEX) {
//		DEBUG;
				faces[facesNumber].sideIndexes[0] = sideIndex0;
				faces[facesNumber].sideIndexes[1] = sideIndex1;
				faces[facesNumber].sideIndexes[2] = sideIndex2;
				faces[facesNumber].pointIndexes[0] = indexSrc;
				faces[facesNumber].pointIndexes[1] = sideIndex1;
				faces[facesNumber].pointIndexes[2] = sideIndex2;
				faces[facesNumber].color = UNVISITED_COLOR;
				
				addFaceIndexToSide(sides, sideIndex0, facesNumber);
				addFaceIndexToSide(sides, sideIndex1, facesNumber);
				addFaceIndexToSide(sides, sideIndex2, facesNumber);
				facesNumber++;
			}
		}
	}
	return facesNumber;
}

int getMinHeight(const Side &side, const Point* points) {
	return min(points[side.pointIndexes[0]].h, points[side.pointIndexes[1]].h);
}

void addSideToPriorityQueue(int sideIndex, int faceIndex, const Side* sides, const Point* points, priority_queue<pair<int, pair<int, int> > > &queue) {
	queue.push(make_pair(-getMinHeight(sides[sideIndex], points), make_pair(sideIndex, faceIndex)));
}

void addOutsideSidesToPriorityQueue(int sidesNumber, const Side* sides, const Point* points, priority_queue<pair<int, pair<int, int> > > &queue) {
	for (int i = 0; i < sidesNumber; i++) {
		if (sides[i].faceIndexes[1] == NULL_INDEX) {
			addSideToPriorityQueue(i, sides[i].faceIndexes[0], sides, points, queue);
		}
	}
}

int getAnotherFace(int sideIndex, int faceIndex, const Side* sides) {
	if (sides[sideIndex].faceIndexes[0] == faceIndex) {
		return sides[sideIndex].faceIndexes[1];
	} else {
		return sides[sideIndex].faceIndexes[0];
	}
}

#define LAKE_NOT_FOUND	0
#define LAKE_FOUND		1
#define NEVER_LAKE		2

int flow(int color, int height, int sideIndex, int faceIndex, const Point* points, const Side* sides, Face* faces, priority_queue<pair<int, pair<int, int> > > &queue) {
	if (faces[faceIndex].color != UNVISITED_COLOR) {
		if (faces[faceIndex].color == color) {
			return LAKE_NOT_FOUND;
		} else {
			return NEVER_LAKE;
		}
	}
	faces[faceIndex].color = color;
	int output = LAKE_NOT_FOUND;
	for (int i = 0; i < 3; i++) {
		int nextSideIndex = faces[faceIndex].sideIndexes[i];
		if (nextSideIndex == sideIndex) {
			continue;
		}
		int nextSideMinHeight = getMinHeight(sides[nextSideIndex], points);
		if (nextSideMinHeight >= height) {
			int nextFaceIndex = getAnotherFace(nextSideIndex, faceIndex, sides);
			if (nextFaceIndex != NULL_INDEX && faces[nextFaceIndex].color == UNVISITED_COLOR) {
				addSideToPriorityQueue(nextSideIndex, nextFaceIndex, sides, points, queue);
			}
			continue;
		}
		if (output == LAKE_NOT_FOUND) {
			output = LAKE_FOUND;
		}
		int nextFaceIndex = getAnotherFace(nextSideIndex, faceIndex, sides);
		if (nextFaceIndex == NULL_INDEX) {
			output = NEVER_LAKE;
			continue;
		}
		int nextOutput = flow(color, height, nextSideIndex, nextFaceIndex, points, sides, faces, queue);
		if (nextOutput == NEVER_LAKE) {
			output = NEVER_LAKE;
		}
	}
	
	return output;
}

bool findLake(const Point* points, const Side* sides, Face* faces, priority_queue<pair<int, pair<int, int> > > &queue, int facesNumber) {
	bool findAtLeastOneLake = false;
	
	int color = 0;
//	cout << queue.size() << endl;
	while (!queue.empty()) {
//		DEBUG;
//		printFaces(faces, facesNumber, points);
//		DEBUG;
		
		
		pair<int, pair<int, int> > p = queue.top();
		queue.pop();
		int height = -p.first;
		int sideIndex = p.second.first;
		int faceIndex = p.second.second;
		
		if (faces[faceIndex].color != UNVISITED_COLOR) {
			continue;
		}
		
		if (flow(color, height, sideIndex, faceIndex, points, sides, faces, queue) == LAKE_FOUND) {
			cout << "  " << height << endl;
			findAtLeastOneLake = true;
		}
		color++;
	}
	return findAtLeastOneLake;
}

void solve(int pointsNumber, int sidesNumber) {
	Point points[pointsNumber];
	map<string, int> pointNameToIndexMap;
	
	readPoints(pointsNumber, points, pointNameToIndexMap);
	
	Side sides[sidesNumber];
	map<pair<int, int>, int> twoPointsToSideIndexMap;
	vector<vector<int> > sidesGraph(pointsNumber);
	
	readSides(sidesNumber, pointNameToIndexMap, sides, twoPointsToSideIndexMap, sidesGraph);
	
	Face faces[sidesNumber*2];
	int facesNumber = generateFaces(faces, pointsNumber, points, sidesNumber, sides, twoPointsToSideIndexMap, sidesGraph);
	
	priority_queue<pair<int, pair<int, int> > > queue;
	addOutsideSidesToPriorityQueue(sidesNumber, sides, points, queue);
	if (!findLake(points, sides, faces, queue, facesNumber)) {
		cout << "  0" << endl;
	}
}

int main(void) {
	for (int casenumber = 1; true; casenumber++) {
		int pointsNumber, sidesNumber;
		cin >> pointsNumber >> sidesNumber;
		
		if (pointsNumber == 0) {
			return 0;
		}
		
		cout << "Case " << casenumber << ":" << endl;
		solve(pointsNumber, sidesNumber);
	}
}
