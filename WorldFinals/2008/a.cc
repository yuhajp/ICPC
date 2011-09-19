#include<iostream>

using namespace std;

int DRCT[6][3] = {
	{ 1,  0,  0},
	{-1,  0,  0},
	{ 0,  1,  0},
	{ 0, -1,  0},
	{ 0,  0,  1},
	{ 0,  0, -1},
};
string DRCT_STR[6] = {
	"+x", "-x", "+y", "-y", "+z", "-z"
};
int ELBOW_LEN_ARRAY1[2] = {2, 1};
int ELBOW_LEN_ARRAY2[2] = {2, 3};

int visit[20][20][20];

const int* getDrct() {
	string str;
	cin >> str;
	for(int i = 0; i < 6; i++) {
		if(str == DRCT_STR[i])return DRCT[i];
	}
	return NULL;
}

bool addVisitTable(const int* pos, const int* width, int addValue) {
	for(int i = 0; i < 3; i++) {
		if(pos[i] < 0 || width[i] <= pos[i])return false;
	}
	visit[pos[0]][pos[1]][pos[2]] += addValue;
	return visit[pos[0]][pos[1]][pos[2]] != 2;
}

bool parallel(const int* drct1, const int* drct2) {
	for(int i = 0; i < 3; i++) {
		if(drct1[i] != 0 && drct2[i] !=- 0)return true;
	}
	return false;
}

bool isGoal(const int* pos, const int* posDrct, const int* dst, const int* dstDrct) {
	for(int i = 0; i < 3; i++){
		if(pos[i] - posDrct[i] != dst[i])return false;
		if(posDrct[i] != dstDrct[i])return false;
	}
	return true;
}

int solveDfs(const int* width, int* pos, const int* posDrct, const int* dst, const int* dstDrct, int count, int answer) {
	if(count >= answer)return answer;
	for(int i = 0; i < 2; i++) {
		int elbowLen1 = ELBOW_LEN_ARRAY1[i];
		int elbowLen2 = ELBOW_LEN_ARRAY2[i];
		bool conflict1 = false;
		for(int j = 0; j < elbowLen1; j++) {
			conflict1 |= !addVisitTable(pos, width, +1);
			for(int k = 0; k < 3; k++) pos[k] += posDrct[k];
		}
		if(!conflict1) {
			for(int a = 0; a < 6; a++) {
				const int* nextDrct = DRCT[a];
				if(parallel(posDrct, nextDrct))continue;
				bool conflict2 = false;
				for(int j = 0; j < elbowLen2; j++) {
					conflict2 |= !addVisitTable(pos, width, +1);
					for(int k = 0; k < 3; k++) pos[k] += nextDrct[k];
				}
				if(!conflict2) {
					if(isGoal(pos, nextDrct, dst, dstDrct)) {
						answer = count;
					} else {
						answer = solveDfs(width, pos, nextDrct, dst, dstDrct, count+1, answer);
					}
				}
				for(int j = 0; j < elbowLen2; j++) {
					for(int k = 0; k < 3; k++) pos[k] -= nextDrct[k];
					addVisitTable(pos, width, -1);
				}
			}
		}
		for(int j = 0; j < elbowLen1; j++) {
			for(int k = 0; k < 3; k++) pos[k] -= posDrct[k];
			addVisitTable(pos, width, -1);
		}
	}
	return answer;
}

int solve(const int* width, const int* src, const int* srcDrct, const int* dst, const int* dstDrct) {
	for(int i = 0; i < width[0]; i++) {
		for(int j = 0; j < width[1]; j++) {
			for(int k = 0; k < width[2]; k++) {
				visit[i][j][k] = 0;
			}
		}
	}
	int pos[3] = {src[0], src[1], src[2]};
	return solveDfs(width, pos, srcDrct, dst, dstDrct, 1, 7);
}

int main(void) {
	for(int caseNumber = 1; true; caseNumber++) {
		int width[3];
		cin >> width[0];
		if(width[0] == 0)return 0;
		cin >> width[1] >> width[2];
		int src[3];
		for(int i = 0; i < 3; i++) {
			cin >> src[i];
			src[i]--;
		}
		const int* srcDrct = getDrct();
		int dst[3];
		for(int i = 0; i < 3; i++) {
			cin >> dst[i];
			dst[i]--;
		}
		const int* dstDrct = getDrct();
		int answer = solve(width, src, srcDrct, dst, dstDrct);
		if(answer == 7) {
			cout << "Case " << caseNumber << ": Impossible" << endl;
		} else {
			cout << "Case " << caseNumber << ": " << answer << endl;
		}
	}
}
