#include<iostream>
#include<vector>
#include<bitset>
#include<map>
using namespace std;

int m, n;
bitset<16> objects[130];
map<int, int> table;
#define DEBUG cout<<"DEBUG: "<<__LINE__<<endl

#define YES		1
#define NO		2

bool separateAtLeastOneObjects(int questionIndex, const vector<int> &restObjectIndexes) {
	bool existYes = false;
	bool existNo = false;
	for (vector<int>::const_iterator it = restObjectIndexes.begin(); it != restObjectIndexes.end(); it++) {
		if (objects[*it][questionIndex]) {
			existYes = true;
		} else {
			existNo = true;
		}
		if (existYes && existNo) {
			return true;
		}
	}
	return false;
}

pair<vector<int>, vector<int> > split(int questionIndex, const vector<int> &restObjectIndexes) {
	vector<int> yesIndexes;
	vector<int> noIndexes;
	for (vector<int>::const_iterator it = restObjectIndexes.begin(); it != restObjectIndexes.end(); it++) {
		if (objects[*it][questionIndex]) {
			yesIndexes.push_back(*it);
		} else {
			noIndexes.push_back(*it);
		}
	}
	return make_pair(yesIndexes, noIndexes);
}

int solve(int answers, vector<int> &restObjectIndexes) {
	map<int, int>::iterator it = table.find(answers);
	if (it != table.end()) {
		return it->second;
	}
	if (restObjectIndexes.size() <= 1) {
		table[answers] = 0;
		return 0;
	}
	
	int output = 100;
	for (int i = 0; i < m; i++) {
		int temp = answers;
		temp >>= 2*i;
		if ((temp & 3) == 0 && separateAtLeastOneObjects(i, restObjectIndexes)) {
			pair<vector<int>, vector<int> > twoIndexes = split(i, restObjectIndexes);
			int depth1 = solve(answers | (YES << (2*i)), twoIndexes.first);
			int depth2 = solve(answers | (NO << (2*i)), twoIndexes.second);
			output = min(output, max(depth1, depth2) + 1);
		}
	}
	table[answers] = output;
	return output;
}

int main(void) {
	while (true) {
		cin >> m >> n;
		if (m == 0 && n == 0) {
			break;
		}
		vector<int> initialIndexes;
		for (int i = 0; i < n; i++) {
			initialIndexes.push_back(i);
			string str;
			cin >> str;
			for (int j = 0; j < m; j++) {
				objects[i][j] = (str[j] == '1');
			}
		}
		table.clear();
		cout << solve(0, initialIndexes) << endl;
	}
}
