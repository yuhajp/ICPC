#include<iostream>
#include<map>
#include<vector>
#include<bitset>

using namespace std;

#define DEBUG	cout<<"DEBUG:"<<__LINE__<<endl

#define HEIGHT	22
#define WIDTH	9

#define UNDEFINE_ID		100

int h, w;
bitset<WIDTH> availableFramLayers[HEIGHT];

typedef long long ll;

typedef unsigned long long ull;
typedef unsigned int ui;

//Use 0th - 26th bits
typedef ui LAYER_STATE;

//Use 0th - 26th bits for LAYER_STATE
//Use 27th - 30th bits for LENGTH
typedef ui LAYER_STATE_WITH_LENGTH;

#define NULL_LAYER_STATE		0

typedef map<LAYER_STATE, vector<LAYER_STATE_WITH_LENGTH> > NEXT_LAYER_STATE_TABLE;

//Use 0th - 26th bits for LAYER_STATE
//USe 27th - 34th bits for TOTAL_LENGTH
typedef ull LAYER_STATE_WITH_TOTAL_LENGTH;

typedef map<LAYER_STATE, LAYER_STATE_WITH_TOTAL_LENGTH> PREV_STATE_AND_TOTAL_LENGTH_TABLE;

NEXT_LAYER_STATE_TABLE nextLayerStateTable;

char input[30][30];

void printLayerState(LAYER_STATE state) {
	LAYER_STATE temp = state;
	for (int i = 0; i < WIDTH; i++) {
		int cell = temp & 0x7;
		switch (cell) {
		case 0:
			cout << ".";
			break;
		case 1:
			cout << "C";
			break;
		default:
			cout << (cell >> 1);
		}
		temp >>= 3;
	}
	cout << endl;
}

//001,001,001,001,001,001,001,001,001  
//001,0010,0100,1001,0010,0100,1001
//  1,   2,   4,   9,   2,   4,   9

#define MASK	0x1249249

bool isFilled(LAYER_STATE state, int i) {
	return ((state >> (3*i)) & 1) == 1;
}

int getOpenId(LAYER_STATE state, int i) {
	return ((state >> (3*i+1)) & 3);
}

LAYER_STATE_WITH_LENGTH generateLayerState(const bool* isFilled, const int* openIds) {
	int length = 0;
	
	LAYER_STATE state = 0;
	for (int i = WIDTH-1; i >= 0; i--) {
		if (isFilled[i]) {
			state <<= 2;
			state += openIds[i];
			state <<= 1;
			state++;
			length++;
		} else {
			state <<= 3;
		}
	}
	return state + (length << 27);
}
/*
	 U
	 C
	
	 Ux
	  C
	
	xU
	C
	
	O
	x
	
	 O
	CCC
	
	1  1
	CCCC
	
	
*/

vector<pair<int, int> > getConnectedChannelBeginAndEnd2(const bool *isFilled) {
	vector<pair<int, int> > output;
	for (int begin = 0; begin < WIDTH;){
		if (isFilled[begin]) {
			pair<int, int> beginAndEnd = make_pair(begin, begin);
			for (int end = begin; end < WIDTH && isFilled[end]; end++) {
				beginAndEnd = make_pair(begin, end);
			}
			output.push_back(beginAndEnd);
			begin = beginAndEnd.second + 1;
		} else {
			begin++;
		}
	}
	return output;
}


LAYER_STATE_WITH_LENGTH getNextLayerStateWithLength(LAYER_STATE state, int bitsets) {
//	DEBUG;
	bool isFilledBefore[WIDTH];
	int openIdBefore[WIDTH];
	
	
	
	bool isFilledAfter[WIDTH];
	int openIdAfter[WIDTH];
	
	for (int i = 0; i < WIDTH; i++) {
		isFilledBefore[i] = isFilled(state, i);
		openIdBefore[i] = getOpenId(state, i);
		isFilledAfter[i] = ((bitsets & (1 << i)) != 0);
		openIdAfter[i] = 0;
	}
	
	
	/*
	DEBUG;
	for (int i = 0; i < WIDTH; i++) {
		if (isFilledBefore[i]) {
			cout << "C";
		} else {
			cout << ".";
		}
	}
	cout << endl;
	for (int i = 0; i < WIDTH; i++) {
		if (openIdBefore[i] > 0) {
			cout << openIdBefore[i];
		} else if(isFilledBefore[i]) {
			cout << "C";
		} else {
			cout << ".";
		}
	}
	cout << endl;
	for (int i = 0; i < WIDTH; i++) {
		if (isFilledAfter[i]) {
			cout << "C";
		} else {
			cout << ".";
		}
	}
	cout << endl;
	DEBUG;
	*/
	
	
	
	
//	DEBUG;
	for (int i = 0; i < WIDTH; i++) {
		if (isFilledBefore[i] && openIdBefore[i] == 0 && isFilledAfter[i]) {
//			DEBUG;
			return NULL_LAYER_STATE;
		}
		if (openIdBefore[i] != 0 && !isFilledAfter[i]) {
//			DEBUG;
			return NULL_LAYER_STATE;
		}
	}
	
//	DEBUG;
	for (int i = 1; i < WIDTH; i++) {
		if (isFilledBefore[i] && openIdBefore[i] == 0 && !isFilledBefore[i-1] && isFilledAfter[i-1]) {
//			DEBUG;
			return NULL_LAYER_STATE;
		}
		if (isFilledBefore[i-1] && openIdBefore[i-1] == 0 && !isFilledBefore[i] && isFilledAfter[i]) {
//			DEBUG;
			return NULL_LAYER_STATE;
		}
	}
	
//	DEBUG;
	for (int i = 2; i < WIDTH; i++) {
		if (isFilledBefore[i-1] && openIdBefore[i-1] != 0 && isFilledAfter[i-2] && isFilledAfter[i]) {
//			DEBUG;
			return NULL_LAYER_STATE;
		}
	}
	vector<pair<int, int> > beginEndList = getConnectedChannelBeginAndEnd2(isFilledAfter);
	int nextNewOpenId = 10;
	vector<pair<int, int> > mergedIdPair;
	
//	DEBUG;
	for (vector<pair<int, int> >::iterator it = beginEndList.begin(); it != beginEndList.end(); it++) {
		int begin = it->first;
		int end = it->second;
		
		if (begin == end) {
			if (openIdBefore[begin] != 0) {
				openIdAfter[begin] = openIdBefore[begin];
			} else {
//				DEBUG;
				return NULL_LAYER_STATE;
			}
		} else {
			bool beginBeforeIsOpen = (openIdBefore[begin] != 0);
			bool endBeforeIsOpen = (openIdBefore[end] != 0);
			
			if (beginBeforeIsOpen && endBeforeIsOpen) {
				if (openIdBefore[begin] == openIdBefore[end]) {
//					DEBUG;
					return NULL_LAYER_STATE;
				}
				mergedIdPair.push_back(make_pair(openIdBefore[begin], openIdBefore[end]));
			} else if (beginBeforeIsOpen) {
				openIdAfter[end] = openIdBefore[begin];
			} else if (endBeforeIsOpen) {
				openIdAfter[begin] = openIdBefore[end];
			} else {
				if (begin+2 <= end) {
					openIdAfter[end] = nextNewOpenId;
					openIdAfter[begin] = nextNewOpenId;
					nextNewOpenId++;
				} else {
//					DEBUG;
					return NULL_LAYER_STATE;
				}
			}
		}
	}
	
//	DEBUG;
	for (int i = 0; i < WIDTH; i++) {
		if (openIdAfter[i] != 0) {
			for (vector<pair<int, int> >::iterator it = mergedIdPair.begin(); it != mergedIdPair.end(); it++) {
				if (openIdAfter[i] == it->first || openIdAfter[i] == it->second) {
					openIdAfter[i] = min(it->first, it->second);
					break;
				}
			}
		}
	}
	
	
//	DEBUG;
	int mapping[20];
	for (int i = 0; i < 20; i++) {
		mapping[i] = 0;
	}
	
//	DEBUG;
	int nextOpenId = 1;
	for (int i = 0; i < WIDTH; i++) {
		if (openIdAfter[i] != 0) {
//			DEBUG;
//			cout << openIdAfter[i] << endl;
			if (mapping[openIdAfter[i]] == 0) {
				mapping[openIdAfter[i]] = nextOpenId;
				nextOpenId++;
			}
			openIdAfter[i] = mapping[openIdAfter[i]];
		}
	}
	
	
	
//	DEBUG;
	LAYER_STATE_WITH_LENGTH output = generateLayerState(isFilledAfter, openIdAfter);
	
	
	/*
	DEBUG;
	for (int i = 0; i < WIDTH; i++) {
		if (openIdBefore[i] > 0) {
			cout << openIdBefore[i];
		} else if(isFilledBefore[i]) {
			cout << "C";
		} else {
			cout << ".";
		}
	}
	cout << endl;
	for (int i = 0; i < WIDTH; i++) {
		if (openIdAfter[i] > 0) {
			cout << openIdAfter[i];
		} else if(isFilledAfter[i]) {
			cout << "C";
		} else {
			cout << ".";
		}
	}
	cout << endl;
	DEBUG;
	*/
	
	return output;
}

const vector<LAYER_STATE_WITH_LENGTH>& getNextLayerStates(LAYER_STATE state) {
//	DEBUG;
	vector<LAYER_STATE_WITH_LENGTH> output;
	
	NEXT_LAYER_STATE_TABLE::iterator it = nextLayerStateTable.find(state);
//	DEBUG;
	if (it != nextLayerStateTable.end()) {
		return it->second;
	}
	
//	DEBUG;
	vector<LAYER_STATE_WITH_LENGTH> v;
//	cout << "========================" << endl;
//	printLayerState(state);
//	cout << "========================" << endl;
	for (int i = 1; i < (1<<WIDTH); i++) {
//		cout << "--------------------------------" << endl;
//		cout << i << endl;
//		cout << "--------------------------------" << endl;
//		DEBUG;
		LAYER_STATE_WITH_LENGTH nextState = getNextLayerStateWithLength(state, i);
//		DEBUG;
		if (nextState != NULL_LAYER_STATE) {
//			DEBUG;
			v.push_back(nextState);
		}
	}
//	cout << "========================" << endl;
	
	nextLayerStateTable[state] = v;
	return nextLayerStateTable[state];
}

void insertLayer2(PREV_STATE_AND_TOTAL_LENGTH_TABLE &table, LAYER_STATE current, LAYER_STATE previous, int totalLength) {
	LAYER_STATE_WITH_TOTAL_LENGTH previousAndTotalLength = ((ull)totalLength << 27) | previous;
	PREV_STATE_AND_TOTAL_LENGTH_TABLE::iterator it = table.find(current);
	if (it == table.end() || it->second < previousAndTotalLength) {
		table[current] = previousAndTotalLength;
	}
}

void printAllHistory(const PREV_STATE_AND_TOTAL_LENGTH_TABLE* prevStatesTables, int depth) {
	for (PREV_STATE_AND_TOTAL_LENGTH_TABLE::const_iterator it = prevStatesTables[depth+1].begin(); it != prevStatesTables[depth+1].end(); it++) {
		LAYER_STATE history[depth+2];
		history[depth+1] = it->first;
		for (int i = depth; i >= 0; i--) {
			LAYER_STATE_WITH_TOTAL_LENGTH layerStateWithTotalLength = prevStatesTables[i+1].find(history[i+1])->second;
			history[i] = layerStateWithTotalLength & 0x7FFFFFF;
		}
		
		DEBUG;
		for (int i = 0; i <= depth+1; i++) {
			printLayerState(history[i]);
		}
	}
}

void solve() {
	ui mask[h+2];
	for (int i = 0; i < h+2; i++) {
		mask[i] = 0;
		for (int j = WIDTH-1; j >= 0; j--) {
			mask[i] <<= 3;
			if (input[i][j] == '#') {
				mask[i]++;
			}
		}
	}
	
	/*
	for (int i = 0; i < h+2; i++) {
		ui temp = mask[i];
		for (int j = 0; j < WIDTH; j++) {
			if ((temp & 1) != 0) {
				cout << "#";
			} else {
				cout << ".";
			}
			temp >>= 3;
		}
		cout << endl;
	}
	*/
	
	PREV_STATE_AND_TOTAL_LENGTH_TABLE prevStatesTables[h+2];
	insertLayer2(prevStatesTables[0], 3, NULL_LAYER_STATE, 1);
	
//	DEBUG;
	for (int i = 1; i < h+2; i++) {
//		DEBUG;
//		cout << "======================================================" << endl;
//		cout << i << endl;
//		cout << "======================================================" << endl;
		for (PREV_STATE_AND_TOTAL_LENGTH_TABLE::iterator it1 = prevStatesTables[i-1].begin(); it1 != prevStatesTables[i-1].end(); it1++) {
//			DEBUG;
			LAYER_STATE currentState = it1->first;
//			DEBUG;
			vector<LAYER_STATE_WITH_LENGTH> v = getNextLayerStates(currentState);
//			DEBUG;
			int totalLength = (it1->second >> 27);
			
//			DEBUG;
//			printLayerState(currentState);
//			return;
			
//			DEBUG;
			for (vector<LAYER_STATE_WITH_LENGTH>::iterator it2 = v.begin(); it2 != v.end(); it2++) {
				LAYER_STATE nextState = *it2 & 0x7FFFFFF;
				int length = *it2 >> 27;
				if ((nextState & mask[i]) == 0) {
					
					insertLayer2(prevStatesTables[i], nextState, currentState, totalLength + length);
					
//					DEBUG;
//					printf("%08X\n", nextState);
//					printf("%08X\n", mask[i]);
//					printLayerState(currentState);
//					printLayerState(nextState);
					/*
					ui temp = mask[i];
					for (int k = 0; k < WIDTH; k++) {
						if ((temp & 1) != 0) {
							cout << "#";
						} else {
							cout << ".";
						}
						temp >>= 3;
					}
					cout << endl;
					*/
				}
			}
		}
//		cout << "***********************************************************************" << endl;
//		printAllHistory(prevStatesTables, i-1);
		
	}
	
	
	PREV_STATE_AND_TOTAL_LENGTH_TABLE::iterator it = prevStatesTables[h+1].begin();
	LAYER_STATE history[h+2];
	history[h+1] = it->first;
	for (int i = h; i >= 0; i--) {
		LAYER_STATE_WITH_TOTAL_LENGTH layerStateWithTotalLength = prevStatesTables[i+1][history[i+1]];
		history[i] = layerStateWithTotalLength & 0x7FFFFFF;
	}
	
//	DEBUG;
	int step = 0;
	for (int i = 1; i <= h; i++) {
		ui temp = history[i];
		for (int j = 0; j < w; j++) {
			if (input[i][j] == '#') {
				cout << "#";
			} else if(temp & 1) {
				cout << "C";
				step++;
			} else {
				cout << ".";
			}
			temp >>= 3;
		}
		cout << endl;
	}
	cout << step << endl;
//	cout << endl;
}

int main(void) {
	for (int casenumber = 1; true; casenumber++) {
		cin >> h >> w;
		if (h == 0 && w == 0) {
			break;
		} else if(casenumber != 1) {
			cout << endl;
		}
		
		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 30; j++) {
				input[i][j] = '#';
			}
		}
		
		input[0][0] = '.';
		for (int i = 1; i <= h; i++) {
			string line;
			cin >> line;
			for (int j = 0; j < w; j++) {
				input[i][j] = line[j];
			}
		}
		input[h+1][w-1] = '.';
		
		cout << "Case " << casenumber << ":" << endl;
		solve();
	}
}
