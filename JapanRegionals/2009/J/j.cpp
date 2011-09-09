#include<iostream>
#include<vector>
#include<set>
#include<string>

using namespace std;

typedef unsigned int ui;

int bitCounterMask[512];
ui adjacentMask[6][25];
ui nextVehicleIndexes[6][25][9];

#define FINISH_MARKER		31
#define DEBUG cout<<"DEBUG:"<<__LINE__<<endl

int countBitsNaive(int n){
	int output = 0;
	while (n > 0) {
		output += (n & 1);
		n >>= 1;
	}
	return output;
}

void generateBitCounterMask() {
	for (int i = 0; i < 512; i++) {
		bitCounterMask[i] = countBitsNaive(i);
	}
}

int countBits(ui n) {
	return bitCounterMask[n & 0x1FF] + bitCounterMask[(n >> 9) & 0x1FF] + bitCounterMask[n >> 18];
}

int di[8] = {1, 1,  1, 0,  0, -1, -1, -1};
int dj[8] = {1, 0, -1, 1, -1,  1,  0, -1};

int getIndex(int i, int j, int n) {
	return n*i+j;
}

ui getMask(int i, int j, int n) {
	return 1 << getIndex(i, j, n);
}

void generateAdjacentMask() {
	for (int n = 1; n <= 5; n++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				int index = getIndex(i, j, n);
				adjacentMask[n][index] = 0;
				for (int k = 0; k < 8; k++) {
					int adjacentI = i + di[k];
					int adjacentJ = j + dj[k];
					if (0 <= adjacentI && adjacentI < n && 0 <= adjacentJ && adjacentJ < n) {
						adjacentMask[n][index] |= getMask(adjacentI, adjacentJ, n);
					}
				}
			}
		}
	}
}

void generateNextVehicleIndexes() {
	for (int n = 1; n <= 5; n++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				int index = getIndex(i, j, n);
				int size = 0;
				for (int k = 0; k < 8; k++) {
					int adjacentI = i + di[k];
					int adjacentJ = j + dj[k];
					if (0 <= adjacentI && adjacentI < n && 0 <= adjacentJ && adjacentJ < n) {
						nextVehicleIndexes[n][index][size++] = getIndex(adjacentI, adjacentJ, n);
					}
				}
				nextVehicleIndexes[n][index][size++] = FINISH_MARKER;
			}
		}
	}
}


ui readState(int n) {
	ui output = 0;
	int vehicleI = -1;
	int vehicleJ = -1;
	for (int i= 0; i < n; i++) {
		string line;
		cin >> line;
		for (int j = 0; j < n; j++) {
			switch (line[j]) {
			case '@':
				vehicleI = i;
				vehicleJ = j;
			case '#':
				output |= getMask(i, j, n);
				break;
			}
		}
	}
	output |= (getIndex(vehicleI, vehicleJ, n) << 25);
	return output;
}

ui getNextState(ui nextStateOnlyMoveVehicle, ui nextVehiclePosition, int n) {
	ui nextState = 0;
	int index = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			ui adjacentInfectedAreas = (nextStateOnlyMoveVehicle & adjacentMask[n][index]);
			int countAdjacentInfectedAreas = countBits(adjacentInfectedAreas);
			
			if (countAdjacentInfectedAreas == 3 || (countAdjacentInfectedAreas == 2 && nextStateOnlyMoveVehicle & (1 << index)) != 0) {
				nextState |= (1 << index);
			}
			index++;
		}
	}
	nextState |= (1 << nextVehiclePosition);
	nextState |= (nextVehiclePosition << 25);
	return nextState;
}

bool isFinished(ui state) {
	return countBits(state & 0x1FFFFFF) <= 1;
}

int solve(int n, ui initialState) {
	if (isFinished(initialState)) {
		return 0;
	}
	
	vector<ui> queue1;
	vector<ui> queue2;
	
	vector<ui>* beforeQueue = &queue1;
	vector<ui>* afterQueue = &queue2;
	
	set<ui> visited;
	beforeQueue->push_back(initialState);
	visited.insert(initialState);
	
	int step = 1;
	while (beforeQueue->size() > 0) {
		afterQueue->clear();
		for (vector<ui>::iterator it = beforeQueue->begin(); it != beforeQueue->end(); it++) {
			ui currentState = *it;
			ui currentVehiclePosition = currentState >> 25;
			ui currentStateRemoveVehicle = currentState- (1 << currentVehiclePosition);
			for (int i = 0; nextVehicleIndexes[n][currentVehiclePosition][i] != FINISH_MARKER; i++) {
				ui nextVehiclePosition = nextVehicleIndexes[n][currentVehiclePosition][i];
				ui nextStateOnlyMoveVehicle = currentStateRemoveVehicle | (1 << nextVehiclePosition);
				if (nextStateOnlyMoveVehicle == currentStateRemoveVehicle) {
					continue;
				}
				ui nextState = getNextState(nextStateOnlyMoveVehicle, nextVehiclePosition, n);
				if (visited.find(nextState) != visited.end()) {
					continue;
				}
				if (isFinished(nextState)) {
					return step;
				}
				afterQueue->push_back(nextState);
				visited.insert(nextState);
			}
		}
		
		step++;
		vector<ui>* swap = beforeQueue;
		beforeQueue = afterQueue;
		afterQueue = swap;
	}
	return -1;
}



int main(void) {
	generateBitCounterMask();
	generateAdjacentMask();
	generateNextVehicleIndexes();
	
	while(true) {
		int n;
		cin >> n;
		if (n == 0) {
			break;
		}
		ui initialState = readState(n);
		cout << solve(n, initialState) << endl;
	}
}
