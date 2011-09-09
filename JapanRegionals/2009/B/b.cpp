#include<iostream>
#include<string>
#include<vector>
using namespace std;

string replaceAll(string src, string dst, string text) {
	string buf("");
	while (true) {
		string::size_type index = text.find(src);
		if (index != string::npos) {
			buf += text.substr(0, index) + dst;
			text = text.substr(index + src.length());
		} else if (!buf.empty()) {
			return buf + text;
		} else {
			return "---------------------------------------";
		}
	}
}

int solve(int n, const string* srcs, const string* dsts, string begin, string end) {
	int count = 1;
	vector<string> before;
	vector<string> after;
	before.push_back(begin);
	while (before.size() > 0) {
		after = vector<string>();
		for (vector<string>::iterator it = before.begin(); it != before.end(); it++) {
			for (int i = 0; i < n; i++) {
				string replaced = replaceAll(srcs[i], dsts[i], *it);
				if (replaced == end) {
					return count;
				} else if (replaced.length() < end.length()) {
					after.push_back(replaced);
				}
			}
		}
		count++;
		before = after;
	}
	return -1;
}

int main(void) {
	while (true) {
		int n;
		cin >> n;
		if (n == 0) {
			break;
		}
		
		string srcs[n];
		string dsts[n];
		for (int i = 0; i < n; i++) {
			cin >> srcs[i] >> dsts[i];
		}
		string begin, end;
		cin >> begin >> end;
		cout << solve(n, srcs, dsts, begin, end) << endl;
	}
}
