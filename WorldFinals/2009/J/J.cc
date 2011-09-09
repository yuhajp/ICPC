#include<iostream>
#include<vector>
#include<algorithm>
#include<cstdio>

using namespace std;

typedef pair<int, int> edge;
typedef vector<edge> edges;
typedef vector<edges> graph;

//max - first, min - second
typedef pair<int, int> error;

#define INF (1<<28)

//#define DEBUG cout<<"DEBUG: "<<__LINE__<<endl
#define DEBUG

#define N 100
#define M 10000

void calculateArrays(int index, int bound, graph &g, int* outputPN, int* outputNP);

void setPNAndNP(int p, int n, int bound, int* outputPN, int* outputNP) {
	if(p > bound) return;
	if(n > bound) return;
	if(p < 0 && n < 0) {
		//maybe error
		setPNAndNP(0, 0, bound, outputPN, outputNP);
	} else if(p < 0) {
		setPNAndNP(0, n, bound, outputPN, outputNP);
	} else if(n < 0) {
		setPNAndNP(p, 0, bound, outputPN, outputNP);
	} else {
		for(int i = p; i <= bound && outputPN[i] > n; i++) {
			outputPN[i] = n;
		}
		for(int i = n; i <= bound && outputNP[i] > p; i++) {
			outputNP[i] = p;
		}
	}
}

void mergeSingleSource(graph &g, int dst, int cost, int bound, int* outputPN, int* outputNP) {
	int testbedPN[bound+1];
	int testbedNP[bound+1];
	
	calculateArrays(dst, bound, g, testbedPN, testbedNP);
	
	for(int i = 0; i <= bound; i++) {
		outputPN[i] = INF;
		outputNP[i] = INF;
	}
	
	int pd = cost;
	int nd = abs(cost - 60);
	
	for(int i = 0; i <= bound; i++) {
		setPNAndNP(i+pd, testbedPN[i] - pd, bound, outputPN, outputNP);
		setPNAndNP(testbedNP[i] - nd, i+nd, bound, outputPN, outputNP);
		setPNAndNP(i-nd, testbedPN[i] + nd, bound, outputPN, outputNP);
		setPNAndNP(testbedNP[i] + pd, i-pd, bound, outputPN, outputNP);
	}
}

int selectTop2(const int* array, int size, int bound) {
	int max1 = max(array[0], array[1]);
	int max2 = min(array[0], array[1]);
	
	for(int i = 2; i < size; i++) {
		if(max1 < array[i]) {
			max2 = max1;
			max1 = array[i];
		} else if(max2 < array[i]) {
			max2 = array[i];
		}
	}
	if(max1 + max2 <= bound) {
		return max1;
	} else {
		return INF;
	}
}


void mergeMultipleSources(graph &g, int index, int bound, int* outputPN, int* outputNP) {
	int n = g[index].size();
	
	int testbedPNMatrix[n][bound+1];
	int testbedNPMatrix[n][bound+1];
	
	for(int i = 0; i <= bound; i++) {
		outputPN[i] = INF;
		outputNP[i] = INF;
	}
	
	for(int i = 0; i < n; i++) {
		mergeSingleSource(g, g[index][i].first, g[index][i].second, bound, testbedPNMatrix[i], testbedNPMatrix[i]);
	}
	
	int tempPN[n];
	int tempNP[n];
	for(int i = 0; i <= bound/2; i++) {
		for(int j = 0; j < n; j++) {
			tempPN[j] = testbedPNMatrix[j][i];
			tempNP[j] = testbedNPMatrix[j][i];
		}
		setPNAndNP(i, selectTop2(tempPN, n, bound), bound, outputPN, outputNP);
		setPNAndNP(selectTop2(tempNP, n, bound), i, bound, outputPN, outputNP);
	}
	
	for(int i = bound/2 + 1; i <= bound; i++) {
		for(int j = 0; j < n; j++) {
			tempPN[j] = testbedPNMatrix[j][bound - i];
			tempNP[j] = testbedNPMatrix[j][bound - i];
		}
		for(int j = 0; j < n; j++) {
			tempPN[j] = testbedPNMatrix[j][i];
			setPNAndNP(i, selectTop2(tempPN, n, bound), bound, outputPN, outputNP);
			tempPN[j] = testbedPNMatrix[j][bound - i];
			
			tempNP[j] = testbedNPMatrix[j][i];
			setPNAndNP(selectTop2(tempNP, n, bound), i, bound, outputPN, outputNP);
			tempNP[j] = testbedNPMatrix[j][bound - i];
		}
	}
}


void calculateArrays(int index, int bound, graph &g, int* outputPN, int* outputNP) {
	if(g[index].size() == 0) {
		for(int i = 0; i <= bound; i++) {
			outputPN[i] = 0;
			outputNP[i] = 0;
		}
	} else if(g[index].size() == 1) {
		mergeSingleSource(g, g[index][0].first, g[index][0].second, bound, outputPN, outputNP);
	} else {
		mergeMultipleSources(g, index, bound, outputPN, outputNP);
	}
}

bool acceptable(int bound, graph &g) {
	int finalPN[bound+1];
	int finalNP[bound+1];
	
	calculateArrays(0, bound, g, finalPN, finalNP);
	
	for(int i = 0; i <= bound; i++) {
		if(finalPN[i] != INF){
			return true;
		}
		if(finalNP[i] != INF){
			return true;
		}
	}
	return false;
}

void makeOrientedGraphDfs(graph &gInput, graph &gOutput, int index, int from) {
	for(edges::iterator it = gInput[index].begin(); it != gInput[index].end(); it++) {
		if(it->first != from) {
			gOutput[index].push_back(*it);
			makeOrientedGraphDfs(gInput, gOutput, it->first, index);
		}
	}
}

graph makeOrientedGraph(graph &gInput) {
	int n = gInput.size();
	
	graph gOutput;
	for(int i = 0; i < n; i++) {
		gOutput.push_back(vector<pair<int, int> >());
	}
	makeOrientedGraphDfs(gInput, gOutput, 0, -1);
	return gOutput;
}

int solve(graph &gInput) {
	graph g = makeOrientedGraph(gInput);
	
	if(acceptable(0, g)) return 0;
	
	int min = 0;
	int max = 1;
	while(!acceptable(max, g)) {
		min = max;
		max *= 2;
	}
	
	while(max - min >= 2) {
		int mid = (max + min) / 2;
		if(acceptable(mid, g)) {
			max = mid;
		} else {
			min = mid;
		}
	}
	return max;
}

int main(void) {
	for(int caseNumber = 1; true; caseNumber++) {
		int n;
		cin >> n;
		if(n == 0)break;
		
		graph g;
		for(int i = 0; i < n; i++) {
			g.push_back(vector<pair<int, int> >());
		}
		
		for(int i = 0; i < n-1; i++) {
			int a, b, c;
			cin >> a >> b >> c;
			a--;
			b--;
			c %= 60;
			g[a].push_back(make_pair(b, c));
			g[b].push_back(make_pair(a, c));
		}
		printf("Case %d: %d\n", caseNumber, solve(g));
	}
}
