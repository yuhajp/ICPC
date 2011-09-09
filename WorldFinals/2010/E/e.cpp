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
typedef ull LAYER_STATE_WITH_TOTAL_LENGTH_AND_DUPLICATION;

typedef map<LAYER_STATE, LAYER_STATE_WITH_TOTAL_LENGTH_AND_DUPLICATION> PREV_STATE_AND_TOTAL_LENGTH_TABLE;

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
	*/
	
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
	
	for (int i = 1; i < WIDTH; i++) {
		if (isFilledBefore[i] && openIdBefore[i] == 0 && !isFilledBefore[i-1] && isFilledAfter[i-1]) {
//			DEBUG;
			return NULL_LAYER_STATE;
		}
		if (isFilledBefore[i-1] && openIdBefore[i-1] == 0 && !isFilledBefore[i] && isFilledAfter[i]) {
			return NULL_LAYER_STATE;
		}
	}
	
	for (int i = 2; i < WIDTH; i++) {
		if (isFilledBefore[i-1] && openIdBefore[i-1] != 0 && isFilledAfter[i-2] && isFilledAfter[i]) {
//			DEBUG;
			return NULL_LAYER_STATE;
		}
	}
	vector<pair<int, int> > beginEndList = getConnectedChannelBeginAndEnd2(isFilledAfter);
	int nextNewOpenId = 10;
	vector<pair<int, int> > mergedIdPair;
	
	for (vector<pair<int, int> >::iterator it = beginEndList.begin(); it != beginEndList.end(); it++) {
		int begin = it->first;
		int end = it->second;
		
		if (begin == end) {
			if (openIdBefore[begin] != 0) {
				openIdAfter[begin] = openIdBefore[begin];
			} else {
//			DEBUG;
				return NULL_LAYER_STATE;
			}
		} else {
			bool beginBeforeIsOpen = (openIdBefore[begin] != 0);
			bool endBeforeIsOpen = (openIdBefore[end] != 0);
			
			if (beginBeforeIsOpen && endBeforeIsOpen) {
				if (openIdBefore[begin] == openIdBefore[end]) {
//			DEBUG;
					return NULL_LAYER_STATE;
				}
//				mergedIdPair.push_back(make_pair(openIdBefore[begin], openIdBefore[end]));
				int minId = min(openIdBefore[begin], openIdBefore[end]);
				int maxId = max(openIdBefore[begin], openIdBefore[end]);
				mergedIdPair.push_back(make_pair(minId, maxId));
				if (mergedIdPair.size() >= 2) {
					if (mergedIdPair[0].first == mergedIdPair[1].first && mergedIdPair[0].second == mergedIdPair[1].second) {
						return NULL_LAYER_STATE;
					}
				}
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
//			DEBUG;
					return NULL_LAYER_STATE;
				}
			}
		}
	}
	
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
	
	
	int mapping[20];
	for (int i = 0; i < 20; i++) {
		mapping[i] = 0;
	}
	
	int nextOpenId = 1;
	for (int i = 0; i < WIDTH; i++) {
		if (openIdAfter[i] != 0) {
			if (mapping[openIdAfter[i]] == 0) {
				mapping[openIdAfter[i]] = nextOpenId;
				nextOpenId++;
			}
			openIdAfter[i] = mapping[openIdAfter[i]];
		}
	}
	
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
	*/
	return output;
}

const vector<LAYER_STATE_WITH_LENGTH>& getNextLayerStates(LAYER_STATE state) {
	vector<LAYER_STATE_WITH_LENGTH> output;
	
	NEXT_LAYER_STATE_TABLE::iterator it = nextLayerStateTable.find(state);
	if (it != nextLayerStateTable.end()) {
		return it->second;
	}
	
	vector<LAYER_STATE_WITH_LENGTH> v;
	for (int i = 1; i < (1<<WIDTH); i++) {
		LAYER_STATE_WITH_LENGTH nextState = getNextLayerStateWithLength(state, i);
		if (nextState != NULL_LAYER_STATE) {
			v.push_back(nextState);
		}
	}
	nextLayerStateTable[state] = v;
	return nextLayerStateTable[state];
}

void insertLayer2(PREV_STATE_AND_TOTAL_LENGTH_TABLE &table, LAYER_STATE current, LAYER_STATE previous, int totalLength) {
	LAYER_STATE_WITH_TOTAL_LENGTH_AND_DUPLICATION previousAndTotalLength = ((ull)totalLength << 28) | previous;
	PREV_STATE_AND_TOTAL_LENGTH_TABLE::iterator it = table.find(current);
	if (it == table.end()) {
		table[current] = previousAndTotalLength;
	} else {
		int alreadyTotalLength = it->second >> 28;
		if (totalLength > alreadyTotalLength) {
			table[current] = previousAndTotalLength;
		} else if (totalLength == alreadyTotalLength) {
			table[current] |= (1 << 27);
		}
	}
	/*
	if (it == table.end() || it->second < previousAndTotalLength) {
		table[current] = previousAndTotalLength;
	}
	*/
}


void printAllHistory(const PREV_STATE_AND_TOTAL_LENGTH_TABLE* prevStatesTables, int depth) {
	for (PREV_STATE_AND_TOTAL_LENGTH_TABLE::const_iterator it = prevStatesTables[depth+1].begin(); it != prevStatesTables[depth+1].end(); it++) {
		LAYER_STATE history[depth+2];
		history[depth+1] = it->first;
		for (int i = depth; i >= 0; i--) {
			LAYER_STATE_WITH_TOTAL_LENGTH_AND_DUPLICATION layerStateWithTotalLength = prevStatesTables[i+1].find(history[i+1])->second;
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
	
	PREV_STATE_AND_TOTAL_LENGTH_TABLE prevStatesTables[h+2];
	insertLayer2(prevStatesTables[0], 3, NULL_LAYER_STATE, 1);
	
	for (int i = 1; i < h+2; i++) {
		for (PREV_STATE_AND_TOTAL_LENGTH_TABLE::iterator it1 = prevStatesTables[i-1].begin(); it1 != prevStatesTables[i-1].end(); it1++) {
			LAYER_STATE currentState = it1->first;
			bool duplication = ((it1->second & (1 << 27)) != 0);
			if (duplication) {
				continue;
			}
			int totalLength = (it1->second >> 28);
			vector<LAYER_STATE_WITH_LENGTH> v = getNextLayerStates(currentState);
			
			for (vector<LAYER_STATE_WITH_LENGTH>::iterator it2 = v.begin(); it2 != v.end(); it2++) {
				LAYER_STATE nextState = *it2 & 0x7FFFFFF;
				int length = *it2 >> 27;
				if ((nextState & mask[i]) == 0) {
					
					insertLayer2(prevStatesTables[i], nextState, currentState, totalLength + length);
				}
			}
		}
	}
	
	
	PREV_STATE_AND_TOTAL_LENGTH_TABLE::iterator it = prevStatesTables[h+1].begin();
	LAYER_STATE history[h+2];
	history[h+1] = it->first;
	for (int i = h; i >= 0; i--) {
		LAYER_STATE_WITH_TOTAL_LENGTH_AND_DUPLICATION layerStateWithTotalLength = prevStatesTables[i+1][history[i+1]];
		history[i] = layerStateWithTotalLength & 0x7FFFFFF;
	}
	
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
//	cout << step << endl;
}

int main(void) {
	for (int casenumber = 1; true; casenumber++) {
		cin >> h >> w;
		if (h == 0 && w == 0) {
			break;
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
		cout << endl;
	}
	/*
	DEBUG;
	cout << nextLayerStateTable.size() << endl;
	DEBUG;
	int sum = 0;
	for (NEXT_LAYER_STATE_TABLE::iterator it1 = nextLayerStateTable.begin(); it1 != nextLayerStateTable.end(); it1++) {
		cout << "============================================" << endl;
		cout << it1->second.size() << endl;
		LAYER_STATE prevState = it1->first;
		for (vector<LAYER_STATE_WITH_LENGTH>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
			LAYER_STATE nextState = *it2 & 0x7FFFFFF;
			DEBUG;
			printLayerState(prevState);
			printLayerState(nextState);
		}
		sum += it1->second.size();
	}
	DEBUG;
	cout << sum << endl;
	*/
}
