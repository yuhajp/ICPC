#include<iostream>
#include<string>
#include<algorithm>

using namespace std;

#define CURVE				0
#define STRAIGHT			1
#define LEFT_SWITCH			2
#define RIGHT_SWITCH		3

#define RIGHT	0
#define UP		1
#define LEFT	2
#define DOWN	3

#define ROTATION_UNDEFINED	-1

#define MAX_WH	6
#define MAX_SWITCH	6

#define DEBUG cout<<"DEBUG:"<<__LINE__<<endl

typedef struct nextSwitch {
	int cost;
	int index;
	bool isMerge;
} NextSwitch;

typedef struct _switch {
	NextSwitch mergedEdge;
	NextSwitch forkEdge[2];
} Switch;

typedef struct state {
	int switchIndex;
	bool isMergeIn;
	bool mergeStatus[6];
} State;

int TILE_CONNECTIONS_BY_ROTATION_SIZE[4]= {4, 2, 4, 4};

int DI[4] = {0, -1,  0,  1};
int DJ[4] = {1,  0, -1,  0};

#define CLOSE	0
#define FORK1	1
#define FORK2	2
#define MERGE	3
#define OPEN	4

int TILE_END_TYPES[4][4][4] = {
	{
		{OPEN,  OPEN,  CLOSE, CLOSE},
		{CLOSE, OPEN,  OPEN,  CLOSE},
		{CLOSE, CLOSE, OPEN,  OPEN},
		{OPEN,  CLOSE, CLOSE, OPEN},
	},
	
	{
		{OPEN,  CLOSE, OPEN,  CLOSE},
		{CLOSE, OPEN,  CLOSE, OPEN},
		{OPEN,  CLOSE, OPEN,  CLOSE},
		{CLOSE, OPEN,  CLOSE, OPEN},
	},
	
	{
		{FORK1, FORK2, MERGE, CLOSE},
		{CLOSE, FORK1, FORK2, MERGE},
		{MERGE, CLOSE, FORK1, FORK2},
		{FORK2, MERGE, CLOSE, FORK1},
	},
	
	{
		{MERGE, FORK1, FORK2, CLOSE},
		{CLOSE, MERGE, FORK1, FORK2},
		{FORK2, CLOSE, MERGE, FORK1},
		{FORK1, FORK2, CLOSE, MERGE},
	},
};

#define TABLE_SIZE	1024

int w, h;
int input[MAX_WH][MAX_WH];
int switchIndexes[MAX_WH][MAX_WH];

int switchSize;
int switchI[MAX_SWITCH];
int switchJ[MAX_SWITCH];



int rotation[MAX_WH][MAX_WH];
Switch switches[MAX_SWITCH];



int colorTable[TABLE_SIZE];
int stepTable[TABLE_SIZE];



int determineDirectionDfs();

bool isOpen(int flag) {
	if (flag != CLOSE) {
		return true;
	} else {
		return false;
	}
}

bool isClose(int flag) {
	return !isOpen(flag);
}



void appendTileInformation(int tileType, int tileRotation, string* lines) {
	string temp[3];
	temp[0] = "   ";
	temp[1] = " + ";
	temp[2] = "   ";
	
	if (isOpen(TILE_END_TYPES[tileType][tileRotation][RIGHT])) {
		temp[1][2] = '-';
	}
	if (isOpen(TILE_END_TYPES[tileType][tileRotation][UP])) {
		temp[0][1] = '|';
	}
	if (isOpen(TILE_END_TYPES[tileType][tileRotation][LEFT])) {
		temp[1][0] = '-';
	}
	if (isOpen(TILE_END_TYPES[tileType][tileRotation][DOWN])) {
		temp[2][1] = '|';
	}
	
	for (int i = 0; i < 3; i++) {
		lines[i] += temp[i];
	}
}

void printInput() {
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			cout << input[i][j];
		}
		cout << endl;
	}
	cout << endl;
}

void printConnectedDirection() {
	for (int k = 0; k < 3*w+2; k++) {
		cout << "-";
	}
	cout << endl;
	for (int i = 0; i < h; i++) {
		string lines[3];
		for (int j = 0; j < w; j++) {
			if (rotation[i][j] != ROTATION_UNDEFINED) {
				appendTileInformation(input[i][j], rotation[i][j], lines);
			} else {
				lines[0] += "   ";
				lines[1] += " + ";
				lines[2] += "   ";
			}
		}
		for (int k = 0; k < 3; k++) {
			cout << "|" << lines[k] << "|" << endl;
		}
	}
	
	for (int k = 0; k < 3*w+2; k++) {
		cout << "-";
	}
	cout << endl;
}

bool hasUncloseOutput(int i, int j) {
	int switchType = input[i][j];
	int switchRotation = rotation[i][j];
	
	for (int a = 0; a < 4; a++) {
		if (isOpen(TILE_END_TYPES[switchType][switchRotation][a]) && rotation[i + DI[a]][j + DJ[a]] == ROTATION_UNDEFINED) {
			return true;
		}
	}
	return false;
}

int getUncloseSwitchIndex() {
	for (int a = 0; a < switchSize; a++) {
		int i = switchI[a];
		int j = switchJ[a];
		if (rotation[i][j] != ROTATION_UNDEFINED && hasUncloseOutput(i, j)) {
			return a;
		}
	}
	return -1;
}

int getRotationUndefinedSwitchIndex() {
	for (int a = 0; a < switchSize; a++) {
		int i = switchI[a];
		int j = switchJ[a];
		if (rotation[i][j] == ROTATION_UNDEFINED) {
			return a;
		}
	}
	return -1;
}

bool adjacentCheck(int i, int j) {
	if (i < 0 || h <= i || j < 0 || w <= j) {
		return true;
	}
	
	int t = input[i][j];
	int r = rotation[i][j];
	if (r == ROTATION_UNDEFINED) {
		return true;
	}
	for (int a = 0; a < 4; a++) {
		int adjacentI = i + DI[a];
		int adjacentJ = j + DJ[a];
		if (isOpen(TILE_END_TYPES[t][r][a])) {
			if (adjacentI < 0 || h <= adjacentI || adjacentJ < 0 || w <= adjacentJ) {
				return false;
			}
			int adjacentT = input[adjacentI][adjacentJ];
			int adjacentR = rotation[adjacentI][adjacentJ];
			int b = (a + 2) % 4;
			if (adjacentR >= 0 && isClose(TILE_END_TYPES[adjacentT][adjacentR][b])) {
				return false;
			}
		} else {
			if (adjacentI < 0 || h <= adjacentI || adjacentJ < 0 || w <= adjacentJ) {
				continue;
			}
			int adjacentT = input[adjacentI][adjacentJ];
			int adjacentR = rotation[adjacentI][adjacentJ];
			int b = (a + 2) % 4;
			if (adjacentR >= 0 && isOpen(TILE_END_TYPES[adjacentT][adjacentR][b])) {
				return false;
			}
		}
	}
	
	return true;
}

int tripToSwitchAtCurveOrStright(int i, int j, int mustBeConnectedIndex) {
	int switchType = input[i][j];
	
	int output = 0;
	for (int rotationType = 0; rotationType < TILE_CONNECTIONS_BY_ROTATION_SIZE[switchType]; rotationType++) {
		rotation[i][j] = rotationType;
		if (isOpen(TILE_END_TYPES[switchType][rotationType][mustBeConnectedIndex]) && adjacentCheck(i, j)) {
			for (int a = 0; a < 4; a++) {
				if (a == mustBeConnectedIndex) {
					continue;
				}
				int adjacentI = i + DI[a];
				int adjacentJ = j + DJ[a];
				if (isOpen(TILE_END_TYPES[switchType][rotationType][a])) {
					if (rotation[adjacentI][adjacentJ] == ROTATION_UNDEFINED) {
						output = max(output, tripToSwitchAtCurveOrStright(adjacentI, adjacentJ, (a+2)%4));
					} else {
						output = max(output, determineDirectionDfs());
					}
					break;
				}
			}
		} else {
			rotation[i][j] = ROTATION_UNDEFINED;
		}
	}
	rotation[i][j] = ROTATION_UNDEFINED;
	return output;
}

int tripToSwitchAtSwitch(int i, int j) {
	int switchType = input[i][j];
	int switchRotation = rotation[i][j];
	
	for (int a = 0; a < 4; a++) {
		int adjacentI = i + DI[a];
		int adjacentJ = j + DJ[a];
		if (isOpen(TILE_END_TYPES[switchType][switchRotation][a]) && rotation[adjacentI][adjacentJ] == ROTATION_UNDEFINED) {
			return tripToSwitchAtCurveOrStright(adjacentI, adjacentJ, (a+2)%4);
		}
	}
	return determineDirectionDfs();
}

int defineRotation(int switchIndex) {
	int i = switchI[switchIndex];
	int j = switchJ[switchIndex];
	
	int output = 0;
	for (int a = 0; a < 4; a++) {
		rotation[i][j] = a;
		if (adjacentCheck(i, j)) {
			output = max(output, tripToSwitchAtSwitch(i, j));
		} else {
			rotation[i][j] = ROTATION_UNDEFINED;
		}
	}
	rotation[i][j] = ROTATION_UNDEFINED;
	return output;
}

void setNextSwitch(int i, int j, int sourceDirection, int count, NextSwitch &nextSwitch) {
	int counterPartDirection = (sourceDirection + 2) % 4;
	int t = input[i][j];
	int r = rotation[i][j];
//	cout << i << ", " << j << ", " << t << ", " << r << ", " << sourceDirection << ", " << counterPartDirection << endl;
	if (t == LEFT_SWITCH || t == RIGHT_SWITCH) {
		nextSwitch.cost = count;
		nextSwitch.index = switchIndexes[i][j];
		nextSwitch.isMerge = (TILE_END_TYPES[t][r][counterPartDirection] == MERGE);
	} else {
		for (int a = 0; a < 4; a++) {
			if (a != counterPartDirection && isOpen(TILE_END_TYPES[t][r][a])) {
				setNextSwitch(i + DI[a], j + DJ[a], a, count+1, nextSwitch);
				return;
			}
		}
		//error
		cout << "UNEXPECTED_CODE" << endl;
		DEBUG;
	}
}

void printNextSwitch(const NextSwitch &nextSwitch) {
	cout << "    NextSwitchID: " << nextSwitch.index << endl;
	cout << "    ConnectMerge: " << nextSwitch.isMerge << endl;
	cout << "    distance:     " << nextSwitch.cost << endl;
}

void printSwitches() {
	for (int s = 0; s < switchSize; s++) {
		cout << "SwitchID: " << s << endl;
		cout << "  Merge:" << endl;
		printNextSwitch(switches[s].mergedEdge);
		cout << "  Fork1:" << endl;
		printNextSwitch(switches[s].forkEdge[0]);
		cout << "  Fork2:" << endl;
		printNextSwitch(switches[s].forkEdge[1]);
	}
}

int encodeState(const State &state) {
	int output = state.switchIndex;
	output <<= 1;
	output |= (state.isMergeIn ? 1 : 0);
	for (int i = 0; i < 6; i++) {
		output <<= 1;
		output |= (state.mergeStatus[i] ? 1 : 0);
	}
	return output;
}

int updateStateReturnCost(State &state) {
	int sIndex = state.switchIndex;
	Switch currentSwitch = switches[sIndex];
	
	NextSwitch selectedEdge;
	int totalCost = 0;
	if (state.isMergeIn) {
		selectedEdge = currentSwitch.forkEdge[state.mergeStatus[sIndex]];
		state.mergeStatus[sIndex] = !state.mergeStatus[sIndex];
		totalCost++;
	} else {
		selectedEdge = currentSwitch.mergedEdge;
	}
	totalCost += selectedEdge.cost;
	state.switchIndex = selectedEdge.index;
	if (selectedEdge.isMerge) {
		state.isMergeIn = true;
	} else {
		state.isMergeIn = false;
		totalCost++;
	}
	return totalCost;
}



int runFunRoute(State &state, int color, int step) {
	int index = encodeState(state);
	if (colorTable[index] == color) {
		return step - stepTable[index];
	} else if (colorTable[index] != 0) {
		return 0;
	}
	
	colorTable[index] = color;
	stepTable[index] = step;
	
	int addedStep = updateStateReturnCost(state);
	return runFunRoute(state, color, step + addedStep);
}

int calculateMaxFunRoute() {
	for (int s = 0; s < switchSize; s++) {
		int i = switchI[s];
		int j = switchJ[s];
		int switchType = input[i][j];
		int switchRotation = rotation[i][j];
		for (int a = 0; a < 4; a++) {
			int adjacentI = i + DI[a];
			int adjacentJ = j + DJ[a];
			
//			DEBUG;
//			cout << i << ", " << j << ", " << a << endl;
			switch (TILE_END_TYPES[switchType][switchRotation][a]) {
			case FORK1:
				 setNextSwitch(adjacentI, adjacentJ, a, 0, switches[s].forkEdge[0]);
				 break;
			case FORK2:
				 setNextSwitch(adjacentI, adjacentJ, a, 0, switches[s].forkEdge[1]);
				 break;
			case MERGE:
				 setNextSwitch(adjacentI, adjacentJ, a, 0, switches[s].mergedEdge);
				 break;
			}
		}
	}
//	printSwitches();
	
//	map<int, int> colorTable;
//	map<int, int> stepTable;
	
	for (int i = 0; i < TABLE_SIZE; i++) {
		colorTable[i] = 0;
		stepTable[i] = 0;
	}
	
	int color = 1;
	int output = 0;
	for (int s = 0; s < switchSize; s++) {
		for (int bits = 0; bits < (1 << (switchSize + 1)); bits++) {
			State initialState;
			initialState.switchIndex = s;
			int tempBits = bits;
			initialState.isMergeIn = ((tempBits & 1) == 1);
			tempBits >>= 1;
			for (int i = 0; i < switchSize; i++) {
				initialState.mergeStatus[i] = ((tempBits & 1) == 1);
				tempBits >>= 1;
			}
			
			int encodedInitialState = encodeState(initialState);
			if (colorTable[encodedInitialState] == 0) {
				int result = runFunRoute(initialState, color, 0);
//				cout << result << endl;
				output = max(output, result);
				color++;
			}
		}
	}
	
	return output;
}

int determineDirectionDfs() {
	int uncloseSwitchIndex = getUncloseSwitchIndex();
	if (uncloseSwitchIndex >= 0) {
		return tripToSwitchAtSwitch(switchI[uncloseSwitchIndex], switchJ[uncloseSwitchIndex]);
	}
	
	int rotationUndefinedSwitchIndex = getRotationUndefinedSwitchIndex();
	if (rotationUndefinedSwitchIndex >= 0) {
		return defineRotation(rotationUndefinedSwitchIndex);
	}
	
//	printConnectedDirection();
	/*
	printConnectedDirection();
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			cout << input[i][j];
		}
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (rotation[i][j] == ROTATION_UNDEFINED) {
				cout << ".";
			} else {
				cout << rotation[i][j];
			}
		}
		cout << endl;
	}
	cout << endl;
	*/
	
	
	return calculateMaxFunRoute();
}


int solve() {
	return determineDirectionDfs();
}

int main(void) {
	while (true) {
		cin >> w >> h;
		if (w == 0 && h == 0) {
			break;
		}
		
		switchSize = 0;
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				char c;
				cin >> c;
				switch(c) {
				case 'C':
//					DEBUG;
					input[i][j] = CURVE;
					switchIndexes[i][j] = -1;
					break;
				case 'S':
//					DEBUG;
					input[i][j] = STRAIGHT;
					switchIndexes[i][j] = -1;
					break;
				case 'L':
//					DEBUG;
					input[i][j] = LEFT_SWITCH;
					switchI[switchSize] = i;
					switchJ[switchSize] = j;
					switchIndexes[i][j] = switchSize;
					switchSize++;
					break;
				case 'R':
//					DEBUG;
					input[i][j] = RIGHT_SWITCH;
					switchI[switchSize] = i;
					switchJ[switchSize] = j;
					switchIndexes[i][j] = switchSize;
					switchSize++;
					break;
				}
				rotation[i][j] = ROTATION_UNDEFINED;
//				cout << c;
			}
//			cout << endl;
		}
//			cout << endl;
		cout << solve() << endl;
	}
	return 0;
}
/*
bool TILE_CONNECTIONS_BY_ROTATION[4][4][4] = {
	{
		{true, true, false, false},
		{false, true, true, false},
		{false, false, true, true},
		{true, false, false, true},
	},
	
	{
		{true, false, true, false},
		{false, true, false, true},
		{true, false, true, false},
		{false, true, false, true},
	},
	
	{
		//R,   U,    L,    D
		{true, true, true, false},
		{false, true, true, true},
		{true, false, true, true},
		{true, true, false, true},
	},
	
	{
		{true, true, true, false},
		{false, true, true, true},
		{true, false, true, true},
		{true, true, false, true},
	},
};
*/

