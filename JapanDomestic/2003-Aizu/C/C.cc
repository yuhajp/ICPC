#include <iostream>
#include <vector>
#include <cctype>

using namespace std;

int main(void)
{
	int W, H;
	while (cin >> W >> H, (W || H)) {
		string str;
		getline(cin, str); // chop \n
		// read field.

		vector<vector<char> > field(H, vector<char>(W));
		for (int h = 0; h < H; ++h) {
			getline(cin, str);
			for (int w = 0; w < W; ++w) {
				field[h][w] = str[w];
			}
		}

		// DP.
		vector<vector<string> > work(H, vector<string>(W));

		if (isdigit(field[H - 1][W - 1])) { 
			work[H - 1][W - 1] = string(1, field[H - 1][W - 1]);
		}

		for (int w = W - 2; w >= 0; --w) {
			if (isdigit(field[H - 1][w])) {
				work[H - 1][w] = field[H - 1][w] + work[H - 1][w + 1];
			}
		}

		for (int h = H - 2; h >= 0; --h) {
			if (isdigit(field[h][W - 1])) {
				work[h][W - 1] = field[h][W - 1] + work[h + 1][W - 1];
			}
		}

		for (int h = H - 2; h >= 0; --h) {
			for (int w = W - 2; w >= 0; --w) {
				if (!isdigit(field[h][w])) { continue; }
				if ((work[h][w + 1].size() > work[h + 1][w].size()) ||
					((work[h][w + 1].size() == work[h + 1][w].size()) &&
					 work[h][w + 1] > work[h + 1][w])) {
					work[h][w] = field[h][w] + work[h][w + 1];
				} else {
					work[h][w] = field[h][w] + work[h + 1][w];
				}
			}
		}
#if 0
		// test
		for (int h = 0; h < H; ++h) {
			for (int w = 0; w < W; ++w) {
				cout << w << ", " << h << " : " << work[h][w] << endl;
			}
		}
#endif
		// cout << "hogehoge" << endl;

		// shorten and get max
		str = "";
		for (int h = 0; h < H; ++h) {
			for (int w = 0; w < W; ++w) {
				int i = 0;
				for (i = 0; i < work[h][w].size(); ++i) {
					if (work[h][w][i] != '0') { break; }
				}
				work[h][w] = work[h][w].substr(i);
				if (work[h][w].size() > str.size()) { str = work[h][w]; continue; }
				if (work[h][w].size() == str.size()) {
					if (work[h][w] > str) { str = work[h][w]; }
				}
			}
		}

		cout << str << endl;
	}
}
