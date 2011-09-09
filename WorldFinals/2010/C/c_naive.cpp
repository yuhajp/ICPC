#include<iostream>
using namespace std;


int solve(int m, int n, int w, const int* x1, const int* x2, const int* y) {
	bool blockExist[m+1][n+1];
	for (int i = 0; i <= m; i++) {
		for (int j = 0; j <= n; j++) {
			blockExist[i][j] = true;
		}
	}
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			blockExist[i][j] = false;
		}
	}
	for (int i = 0; i < w; i++) {
		for (int x = x1[i]; x <= x2[i]; x++) {
			blockExist[y[i]][x] = true;
		}
	}
	
	int count = 0;
	for (int i = m-1; i >= 0; i--) {
		for (int j = n-1; j >= 0; j--) {
			if (i ==m-1 && j == n-1) {
//				cout << ".";
				continue;
			}
			if (blockExist[i][j]) {
//				cout << "B";
				continue;
			}
			blockExist[i][j] = blockExist[i+1][j] && blockExist[i][j+1];
			if (blockExist[i][j]) {
//				cout << "S";
				count++;
			} else {
//				cout << ".";
			}
		}
//		cout << endl;
	}
	return count++;
}


int main(void) {
	for (int casenumber = 1; true; casenumber++) {
		int m, n;
		int w;
		cin >> m >> n >> w;
		if (m == 0 && n == 0 && w == 0) {
			break;
		}
		
		int x1[w], x2[w], y[w], temp;
		for (int i = 0; i < w; i++) {
			cin >> x1[i] >> y[i] >> x2[i] >> temp;
		}
		cout << "Case " << casenumber << ": " << solve(m, n, w, x1, x2, y) << endl;
	}
}
