#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdio>
#include<string>
#include<map>

using namespace std;

#define INF (1LL<<60)
#define DEBUG cout<<"DEBUG: "<<__LINE__<<endl

#define N 21
#define M 210

long long distanceMatrixes[N][M][M];
string keywordMatrix[N][M];
int keywordArraySize[N];
map<string, int> keywordToIndex[N];

int getIndex(string str) {
	int len = str.length();
	map<string, int>::iterator it = keywordToIndex[len].find(str);
	return it->second;
}

void addKeyword(string k) {
	int length = k.length();
	
	if(keywordToIndex[length].find(k) == keywordToIndex[length].end()) {
		keywordToIndex[length][k] = keywordArraySize[length];
		keywordMatrix[length][keywordArraySize[length]++] = k;
	}
}

void addAllKeyword(string keyword) {
	for(int i = 0; i < keyword.length(); i++) {
		addKeyword(keyword.substr(i));
	}
}

void initKeywordToIndex(string src, string dst, vector<pair<string, string> > &rules) {
	for(int i = 0; i < N; i++) {
		keywordToIndex[i].clear();
		keywordArraySize[i] = 0;
	}
	
	addAllKeyword(src);
	addAllKeyword(dst);
	for(vector<pair<string, string> >::iterator it = rules.begin(); it != rules.end(); it++) {
		addAllKeyword(it->first);
		addAllKeyword(it->second);
	}
}

void initDistanceMatrix(vector<pair<string, string> > &rules) {
	for(int len = 0; len < N; len++) {
		int size = keywordArraySize[len];
		for(int i = 0; i < size; i++) {
			for(int j = 0; j < size; j++) {
				distanceMatrixes[len][i][j] = INF;
			}
			distanceMatrixes[len][i][i] = 0;
		}
	}
	
	for(vector<pair<string, string> >::iterator it = rules.begin(); it != rules.end(); it++) {
		string src = it->first;
		string dst = it->second;
		int len = src.length();
		
		int srcIndex = getIndex(src);
		int dstIndex = getIndex(dst);
		
		distanceMatrixes[len][srcIndex][dstIndex] = min(1LL, distanceMatrixes[len][srcIndex][dstIndex]);
	}
}

long long calculateDistance(string src, string dst) {
	if(src == dst)return 0;
	
	long long output = INF;
	for(int i = 0; i < src.length()-1 && src[i] == dst[i]; i++) {
		string subsrc = src.substr(i+1);
		string subdst = dst.substr(i+1);
		int sublen = subsrc.length();
		
		output = min(output, distanceMatrixes[sublen][getIndex(subsrc)][getIndex(subdst)]);
	}
	return output;
}

void calculateDistanceMatrix() {
	for(int len = 1; len < N; len++) {
		int size = keywordArraySize[len];
		
		for(int i = 0; i < size; i++) {
			for(int j = 0; j < size; j++) {
				distanceMatrixes[len][i][j] = min(distanceMatrixes[len][i][j], calculateDistance(keywordMatrix[len][i], keywordMatrix[len][j]));
			}
		}
		
		for(int k = 0; k < size; k++) {
			for(int i = 0; i < size; i++) {
				for(int j = 0; j < size; j++) {
					distanceMatrixes[len][i][j] = min(distanceMatrixes[len][i][j], distanceMatrixes[len][i][k] + distanceMatrixes[len][k][j]);
				}
			}
		}
	}
}


long long solve(string src, string dst, vector<pair<string, string> > &rules) {
	initKeywordToIndex(src, dst, rules);
	initDistanceMatrix(rules);
	calculateDistanceMatrix();
	return distanceMatrixes[src.length()][getIndex(src)][getIndex(dst)];
}

int main(void) {
	for(int caseNumber = 1; true; caseNumber++) {
		string src;
		cin >> src;
		if(src == ".")break;
		
		string dst;
		int n;
		cin >> dst >> n;
		
		
		vector<pair<string, string> > rules;
		for(int i = 0; i < n; i++) {
			string a, b;
			cin >> a >> b;
			rules.push_back(make_pair(a, b));
		}
		
		long long answer = solve(src, dst, rules);
		
		cout << "Case " << caseNumber << ": ";
		if(answer == INF) {
			cout << "No solution";
		} else {
			cout << answer;
		}
		cout << endl;
	}
	return 0;
}
