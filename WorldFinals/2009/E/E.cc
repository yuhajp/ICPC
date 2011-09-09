#include<iostream>
#include<vector>
#include<cstdio>
#include<algorithm>

using namespace std;

#define INF (1<<28)
#define N 50010

typedef struct edge {
	int index;
	int src;
	int cost;
} edge;

int maxCosts[N];
bool multiCosts[N];
int modifiedCosts[N];

bool modifyCosts(int index, int targetCost, vector<pair<int, int> > &edgeIndexAndToll, const vector<edge> *dstBasedEdges){
	if(index == 1)return false;
	if(modifiedCosts[index] == targetCost)return true;
	if(modifiedCosts[index] != 0)return false;
	
	modifiedCosts[index] = targetCost;
	for(vector<edge>::const_iterator it = dstBasedEdges[index].begin(); it != dstBasedEdges[index].end(); it++) {
		if(multiCosts[it->src]) {
			if(!modifyCosts(it->src, targetCost - it->cost, edgeIndexAndToll, dstBasedEdges)) {
				return false;
			}
		} else if(targetCost > it->cost + maxCosts[it->src]) {
			edgeIndexAndToll.push_back(make_pair(it->index, targetCost - it->cost - maxCosts[it->src]));
		}
	}
	return true;
}


void calculateCosts(int index, const vector<edge> *dstBasedEdges){
	if(maxCosts[index] >= 0)return;
	
	maxCosts[index] = 0;
	multiCosts[index] = false;
	for(vector<edge>::const_iterator it = dstBasedEdges[index].begin(); it != dstBasedEdges[index].end(); it++) {
		calculateCosts(it->src, dstBasedEdges);
		int newCost = maxCosts[it->src] + it->cost;
		if(maxCosts[index] != newCost && maxCosts[index] != 0) {
			multiCosts[index] = true;
		}
		maxCosts[index] = max(maxCosts[index], newCost);
		multiCosts[index] |= multiCosts[it->src];
	}
}

int main(void) {
	for(int caseNumber = 1; true; caseNumber++) {
		int n, r;
		cin >> n >> r;
		if(n == 0 && r == 0)break;
		
		vector<edge> dstBasedEdges[n+1];
		
		for(int i = 1; i <= r; i++) {
			int src, dst, cost;
			cin >> src >> dst >> cost;
			
			edge e;
			e.index = i;
			e.src = src;
			e.cost = cost;
			
			dstBasedEdges[dst].push_back(e);
		}
		
		for(int i = 1; i <= n; i++) {
			maxCosts[i] = -1;
			modifiedCosts[i] = 0;
		}
		
		calculateCosts(n, dstBasedEdges);
		
		vector<pair<int, int> > edgeIndexAndToll;
		if(modifyCosts(n, maxCosts[n], edgeIndexAndToll, dstBasedEdges)) {
			printf("Case %d: %d %d\n", caseNumber, edgeIndexAndToll.size(), maxCosts[n]);
			for(vector<pair<int, int> >::iterator it = edgeIndexAndToll.begin(); it != edgeIndexAndToll.end(); it++) {
				printf("%d %d\n", it->first, it->second);
			}
		} else {
			printf("Case %d: No solution\n", caseNumber);
		}
	}
}
