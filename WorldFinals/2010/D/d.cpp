#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

pair<int, int> attackAndDie(int current, int previous, int n, const pair<int, int>* castles, const vector<vector<int> > &graph) {
	vector<pair<int, pair<int, int> > > v;
	for (vector<int>::const_iterator it = graph[current].begin(); it != graph[current].end(); it++) {
		if (*it == previous) {
			continue;
		}
		pair<int, int> p = attackAndDie(*it, current, n, castles, graph);
		v.push_back(make_pair(-p.first + p.second, p));
	}
	sort(v.begin(), v.end());
	
	int totalDieOrDefend = 0;
	int minAttackSoldiers = 0;
	for (vector<pair<int, pair<int, int> > >::const_iterator it = v.begin(); it != v.end(); it++) {
		minAttackSoldiers = max(minAttackSoldiers, totalDieOrDefend + it->second.first);
		totalDieOrDefend += it->second.second;
	}
	
	minAttackSoldiers = max(minAttackSoldiers, totalDieOrDefend);
	
	int finalMinAttackSoldiers = max(castles[current].first, castles[current].second + minAttackSoldiers);
	int finalTotalDieOrDefend = totalDieOrDefend + castles[current].second;
	
	return make_pair(max(finalTotalDieOrDefend, finalMinAttackSoldiers), finalTotalDieOrDefend);
}

int solve(int n, const pair<int, int>* castles, const vector<vector<int> > &graph) {
	int answer = 1 << 28;
	for (int i = 0; i < n; i++) {
		pair<int, int> p = attackAndDie(i, -1, n, castles, graph);
		answer = min(p.first, answer);
	}
	return answer;
}

int main(void) {
	for (int casenumber = 1; true; casenumber++) {
		int n;
		cin >> n;
		if (n == 0) {
			break;
		}
		pair<int, int> castles[n];
		
		for (int i= 0; i < n; i++) {
			int attack;
			int die;
			int defend;
			cin >> attack >> die >> defend;
			castles[i] = make_pair(attack, die+defend);
		}
		
		vector<vector<int> > graph(n);
		for (int i = 0; i < n-1; i++) {
			int src, dst;
			cin >> src >> dst;
			src--;
			dst--;
			graph[src].push_back(dst);
			graph[dst].push_back(src);
		}
		
		cout<<"Case "<<casenumber<<": "<<solve(n, castles, graph)<<endl;
	}
}
