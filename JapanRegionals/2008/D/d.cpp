#include<iostream>
#include<complex>
#include<vector>

using namespace std;

typedef complex<double> cd;
typedef pair<cd, cd> line;

#define EPS 1e-8

bool online(line l, cd p) {
	cd v1 = l.second - l.first;
	cd v2 = p - l.first;
	cd v = v2 / v1;
	return abs(imag(v)) < EPS && 0.0-EPS < real(v) && real(v) < 1.0+EPS;
}

bool connect(line l1, line l2) {
	if(online(l1, l2.first))return true;
	if(online(l1, l2.second))return true;
	if(online(l2, l1.first))return true;
	if(online(l2, l1.second))return true;
	return false;
}

void extractConnectedSet(int startIndex, bool* visit, vector<line> &lines, vector<line> &output, int n) {
	visit[startIndex] = true;
	output.push_back(lines[startIndex]);
	for(int i = 0; i < n; i++) {
		if(visit[i])continue;
		if(connect(lines[startIndex], lines[i])){
			extractConnectedSet(i, visit, lines, output, n);
		}
	}
}

bool isEndPoint(cd p, line myself, vector<line> lines) {
	for(vector<line>::iterator it = lines.begin(); it != lines.end(); it++) {
		if(myself == *it)continue;
		if(online(*it, p))return false;
	}
	return true;
}

int countEndPoint(vector<line> lines) {
	int count = 0;
	for(vector<line>::iterator it = lines.begin(); it != lines.end(); it++) {
		if(isEndPoint(it->first, *it, lines))count++;
		if(isEndPoint(it->second, *it, lines))count++;
	}
	return count;
}
int detect25sub2(cd p1, cd p2, cd p3) {
	cd v1 = p2 - p1;
	cd v2 = p3 - p1;
	cd v = v2 / v1;
	if(imag(v) < 0)return 2;
	if(imag(v) > 0)return 5;
	cout << "ERROR @ detect25sub2" << endl;
	return -1;
}

int detect25sub1(cd endPoint, cd nextEndPoint, line endLine, vector<line> lines) {
	for(vector<line>::iterator it = lines.begin(); it != lines.end(); it++) {
		if(endLine == *it)continue;
		if(abs(it->first - nextEndPoint) < EPS)return detect25sub2(endPoint, nextEndPoint, it->second);
		if(abs(it->second - nextEndPoint) < EPS)return detect25sub2(endPoint, nextEndPoint, it->first);
	}
	cout << "ERROR @ detect25sub1" << endl;
	return -1;
}

int detect25(vector<line> lines) {
	for(vector<line>::iterator it = lines.begin(); it != lines.end(); it++) {
		if(isEndPoint(it->first, *it, lines)) {
			return detect25sub1(it->first, it->second, *it, lines);
		}
		if(isEndPoint(it->second, *it, lines)) {
			return detect25sub1(it->second, it->first, *it, lines);
		}
	}
	cout << "ERROR @ detect25" << endl;
	return -1;
}

int detectNumber(vector<line> lines) {
	int endPoint = countEndPoint(lines);
	if(endPoint == 0 && lines.size() == 4)return 0;
	if(endPoint == 2 && lines.size() == 1)return 1;
	if(endPoint == 3 && lines.size() == 4)return 3;
	if(endPoint == 3 && lines.size() == 3)return 4;
	if(endPoint == 1 && lines.size() == 5)return 6;
	if(endPoint == 2 && lines.size() == 3)return 7;
	if(endPoint == 0 && lines.size() == 5)return 8;
	if(endPoint == 1 && lines.size() == 4)return 9;
	return detect25(lines);
}

int main(void) {
	while(true) {
		int n;
		cin >> n;
		if(n == 0)break;
		
		int xa[n], ya[n], xb[n], yb[n];
		vector<line> lines;
		for(int i = 0; i < n; i++) {
			cin >> xa[i] >> ya[i] >> xb[i] >> yb[i];
			line newLine = make_pair(cd(xa[i], ya[i]), cd(xb[i], yb[i]));
			lines.push_back(newLine);
		}
		
		int count[10];
		fill(count, count+10, 0);
		bool visit[n];
		fill(visit, visit + n, false);
		for(int i = 0; i < n; i++) {
			if(visit[i])continue;
			vector<line> connectedSet;
			extractConnectedSet(i, visit, lines, connectedSet, n);
			int index = detectNumber(connectedSet);
			count[index]++;
		}
		cout << count[0];
		for(int i = 1; i < 10; i++) {
			cout << " " << count[i];
		}
		cout << endl;
	}
}
