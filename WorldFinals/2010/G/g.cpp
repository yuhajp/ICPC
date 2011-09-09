#include<iostream>
#include<cstdio>
#include<cmath>
#include<vector>

using namespace std;

#define INF 1e+10
#define N 105

int x[N], y[N];
double lengthTable[N][N];
int src1Table[N][N];
int src2Table[N][N];

double dis(int index1, int index2) {
	int diffX = x[index1] - x[index2];
	int diffY = y[index1] - y[index2];
	return sqrt(diffX*diffX + diffY*diffY);
}

void trackBack(int index1, int index2, vector<int> &pass1, vector<int> &pass2) {
	if (index1 == 0 && index2 == 0) {
		return;
	}
	int nextIndex1 = src1Table[index1][index2];
	int nextIndex2 = src2Table[index1][index2];

	if (index1 != nextIndex1) {
		pass1.push_back(nextIndex1);
	}
	if (index2 != nextIndex2) {
		pass2.push_back(nextIndex2);
	}
	trackBack(nextIndex1, nextIndex2, pass1, pass2);
}

int main(void) {
	for (int casenumber = 1; true; casenumber++) {
		int n, b1, b2;
		cin >> n >> b1 >> b2;

		if (n == 0) {
			break;
		}

		for (int i = 0; i < n; i++) {
			cin >> x[i] >> y[i];
		}


		lengthTable[0][0] = 0.0;
		for (int i = 1; i <= n-2; i++) {
			lengthTable[i][i] = INF;
			for (int j = 0; j < i; j++) {
				lengthTable[i][j] = INF;
				lengthTable[j][i] = INF;
			}
			
			for (int j = 0; j < i; j++) {
				if (i != b2) {
					if (lengthTable[i][i-1] > lengthTable[j][i-1] + dis(j, i)) {
						lengthTable[i][i-1] = lengthTable[j][i-1] + dis(j, i);
						src1Table[i][i-1] = j;
						src2Table[i][i-1] = i-1;
					}
					if (lengthTable[i][j] > lengthTable[i-1][j] + dis(i-1, i)) {
						lengthTable[i][j] = lengthTable[i-1][j] + dis(i-1, i);
						src1Table[i][j] = i-1;
						src2Table[i][j] = j;
					}
				}
				if (i != b1) {
					if (lengthTable[i-1][i] > lengthTable[i-1][j] + dis(i, j)) {
						lengthTable[i-1][i] = lengthTable[i-1][j] + dis(i, j);
						src1Table[i-1][i] = i-1;
						src2Table[i-1][i] = j;
					}
					if (lengthTable[j][i] > lengthTable[j][i-1] + dis(i, i-1)) {
						lengthTable[j][i] = lengthTable[j][i-1] + dis(i, i-1);
						src1Table[j][i] = j;
						src2Table[j][i] = i-1;
					}
				}
			}
		}
		
		double minLength = INF;
		int index1 = -1;
		int index2 = -1;

		for (int i = 0; i <= n-3; i++) {
			double tempLength = lengthTable[i][n-2] + dis(n-2, n-1) + dis(i, n-1);
			if (tempLength < minLength) {
				index1 = i;
				index2 = n-2;
				minLength = tempLength;
			}
			tempLength = lengthTable[n-2][i] + dis(n-2, n-1) + dis(i, n-1);
			if (tempLength < minLength) {
				index1 = n-2;
				index2 = i;
				minLength = tempLength;
			}
		}

		vector<int> pass1;
		vector<int> pass2;

		pass1.push_back(n-1);
		pass1.push_back(index1);
		pass2.push_back(index2);
		trackBack(index1, index2, pass1, pass2);

		vector<int> pass;
		for (int i = pass1.size() - 1; i >= 0; i--) {
			pass.push_back(pass1[i]);
		}
		for (int i = 0; i < pass2.size(); i++) {
			pass.push_back(pass2[i]);
		}

		printf("Case %d: %.2f\n", casenumber, minLength);
		printf("0");
		if (pass[1] == 1) {
			for (int i = 1; i < pass.size(); i++) {
				printf(" %d", pass[i]);
			}
		} else {
			for (int i = pass.size() - 2; i >= 0; i--) {
				printf(" %d", pass[i]);
			}
		}
		printf("\n");
	}
}
