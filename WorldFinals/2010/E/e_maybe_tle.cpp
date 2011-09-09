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
//char farm[HEIGHT][WIDTH];
bitset<WIDTH> availableFramLayers[HEIGHT];

typedef long long ll;

typedef struct channel {
	bool occupied;
	bool open;
	int id;
} Channel;

typedef struct layer {
	Channel channels[WIDTH];
	int maxLength;
	ll previousMaxLayerHashCode;
//	struct layer* previousMaxLayer;
} Layer;


typedef map<ll, Layer> LayerTable;

void printChannel(const Channel& c) {
	if (!c.occupied) {
		cout << "### ";
		return;
	}
	if (!c.open) {
		cout << "CCC ";
		return;
	}
	cout << "[" << c.id << "] ";
}

Channel getVacantChannel() {
	Channel channel;
	channel.occupied = false;
	channel.open = false;
	channel.id = 0;
	return channel;
}

Channel getNonOpenChannel() {
	Channel channel;
	channel.occupied = true;
	channel.open = false;
	channel.id = 0;
	return channel;
}

Channel getOpenChannel(int id) {
	Channel channel;
	channel.occupied = true;
	channel.open = true;
	channel.id = id;
	return channel;
}

bool isChannelOccupied(const Channel& c) {
	return c.occupied;
}

bool isChannelOccupiedButUnOpen(const Channel& c) {
	return c.occupied && !c.open;
}

bool isChannelOpen(const Channel& c) {
	return c.occupied && c.open;
}

ll getChannelHashCode(const Channel& channel) {
	if(channel.occupied && channel.open) {
		return channel.id;
	} else if (channel.occupied && !channel.open) {
		return 9;
	} else if (!channel.occupied && !channel.open) {
		return 10;
	} else {	//!channel.occupid && channel.open maybe error
		cout << "unexpected channel input" << endl;
		return 11;
	}
}

void printLayer(const Layer& layer) {
	for (int i = 0; i < WIDTH; i++) {
		printChannel(layer.channels[i]);
	}
	cout << ", " << layer.maxLength << ", " << layer.previousMaxLayerHashCode << endl;
}

Layer getInvalidLayer() {
	Layer layer;
	layer.maxLength = 0;
	return layer;
}

bool isInvalidLayer(const Layer &layer) {
	return layer.maxLength == 0;
}

Layer makeInitialLayer() {
	Layer layer;
	layer.channels[0] = getOpenChannel(0);
	for (int i = 1; i < WIDTH; i++) {
		layer.channels[i] = getVacantChannel();
	}
	layer.maxLength = 1;
	layer.previousMaxLayerHashCode = -1;
	return layer;
}

#define NORMALIZED_WIDTH	30

Layer getNormalizedLayer(const Layer &layer) {
	Layer normalizedLayer;
	int nextLayerId = 0;
	int assignment[NORMALIZED_WIDTH];
	
	for (int i= 0; i < NORMALIZED_WIDTH; i++) {
		assignment[i] = -1;
	}
	for (int i = 0; i < WIDTH; i++) {
		if (!layer.channels[i].open) {
			normalizedLayer.channels[i] = layer.channels[i];
		} else if(assignment[layer.channels[i].id] != -1) {
			normalizedLayer.channels[i] = getOpenChannel(assignment[layer.channels[i].id]);
		} else {
			assignment[layer.channels[i].id] = nextLayerId;
			normalizedLayer.channels[i] = getOpenChannel(nextLayerId);
			nextLayerId++;
		}
	}
	normalizedLayer.maxLength = layer.maxLength;
	normalizedLayer.previousMaxLayerHashCode = layer.previousMaxLayerHashCode;
	return normalizedLayer;
}

ll getLayerHashCode(const Layer& layer) {
	ll hashCode = 0L;
	for (int i = 0; i < WIDTH; i++) {
		hashCode *= 11;
		hashCode += getChannelHashCode(layer.channels[i]);
	}
	return hashCode;
}

void insertLayer(LayerTable &table, const Layer &insertedLayer) {
	if (isInvalidLayer(insertedLayer)) {
		return;
	}
	Layer normalizedLayer = getNormalizedLayer(insertedLayer);
	ll hashCode = getLayerHashCode(normalizedLayer);
	
	LayerTable::iterator it = table.find(hashCode);
	if (it != table.end()) {
		Layer sameHashCodeLayer = it->second;
		if (normalizedLayer.maxLength > sameHashCodeLayer.maxLength) {
//			DEBUG;
			table[hashCode] = normalizedLayer;
		} else {
//			DEBUG;
		}
	} else {
//		DEBUG;
		table[hashCode] = normalizedLayer;
	}
}

vector<bitset<WIDTH> > getChannelPositionCandidates (const bitset<WIDTH> &availableFramLayer) {
	vector<bitset<WIDTH> > list;
	for (int i = 1; i < ((1<<WIDTH) - 1); i++) {
		bitset<WIDTH> bs(i);
		if ((availableFramLayer & bs) == bs) {
			list.push_back(bs);
		}
	}
	return list;
}

/*
	U
	C
	
	O
	x
	
	Ux
	 C
	
	xU
	C
	
	 O
	CCC
	
	1  1
	CCCC
	
	
*/

vector<pair<int, int> > getConnectedChannelBeginAndEnd(const Layer &layer) {
	vector<pair<int, int> > output;
	for (int begin = 0; begin < WIDTH;){
		if (isChannelOccupied(layer.channels[begin])) {
			pair<int, int> beginAndEnd = make_pair(begin, begin);
			for (int end = begin; end < WIDTH && isChannelOccupied(layer.channels[end]); end++) {
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

Layer makeNextLayer(ll layerId, const Layer &layer, const bitset<WIDTH> &cannelPosition) {
	Layer nextLayer;
	
	for (int i = 0; i < WIDTH; i++) {
		if (cannelPosition[i]) {
			nextLayer.channels[i] = getNonOpenChannel();
		} else {
			nextLayer.channels[i] = getVacantChannel();
		}
	}
	nextLayer.maxLength = layer.maxLength + cannelPosition.count();
	nextLayer.previousMaxLayerHashCode = layerId;
	/*
	DEBUG;
	printLayer(layer);
	printLayer(nextLayer);
	DEBUG;
	*/
	for (int i = 0; i < WIDTH; i++) {
		if (isChannelOccupiedButUnOpen(layer.channels[i]) && isChannelOccupied(nextLayer.channels[i])) {
//			DEBUG;
			return getInvalidLayer();
		}
		if (isChannelOpen(layer.channels[i]) && !isChannelOccupied(nextLayer.channels[i])) {
//			DEBUG;
			return getInvalidLayer();
		}
	}
	
	for (int i = 1; i < WIDTH; i++) {
		if (isChannelOccupiedButUnOpen(layer.channels[i-1]) &&
				!isChannelOccupied(layer.channels[i]) &&
				isChannelOccupied(nextLayer.channels[i])) {
//			DEBUG;
			return getInvalidLayer();
		}
		if (isChannelOccupiedButUnOpen(layer.channels[i]) &&
				!isChannelOccupied(layer.channels[i-1]) &&
				isChannelOccupied(nextLayer.channels[i-1])) {
//			DEBUG;
			return getInvalidLayer();
		}
	}
	
	for (int i = 2; i < WIDTH; i++) {
		if (isChannelOpen(layer.channels[i-1]) &&
				isChannelOccupied(nextLayer.channels[i-2]) &&
				isChannelOccupied(nextLayer.channels[i-1]) &&
				isChannelOccupied(nextLayer.channels[i])) {
//			DEBUG;
			return getInvalidLayer();
		}
	}
	
	//TODO
	vector<pair<int, int> > beginEndList = getConnectedChannelBeginAndEnd(nextLayer);
	int nextNewOpenId = 10;
	vector<pair<int, int> > mergedIdPair;
	for (vector<pair<int, int> >::iterator it = beginEndList.begin(); it != beginEndList.end(); it++) {
		int begin = it->first;
		int end = it->second;
		
		if (begin == end) {
			if (isChannelOpen(layer.channels[begin])) {
				nextLayer.channels[begin] = layer.channels[begin];
			} else {
//				DEBUG;
				return getInvalidLayer();
			}
		} else {
			bool beginOldIsOpen = isChannelOpen(layer.channels[begin]);
			bool endOldIsOpen = isChannelOpen(layer.channels[end]);
			
			if (beginOldIsOpen && endOldIsOpen) {
				int beginOldId = layer.channels[begin].id;
				int endOldId = layer.channels[end].id;
				// generate loop
				if (beginOldId == endOldId) {
//					DEBUG;
					return getInvalidLayer();
				}
				mergedIdPair.push_back(make_pair(beginOldId, endOldId));
			} else if (beginOldIsOpen) {
				nextLayer.channels[end] = layer.channels[begin];
			} else if (endOldIsOpen) {
				nextLayer.channels[begin] = layer.channels[end];
			} else {
				if (begin+2 <= end) {
					nextLayer.channels[begin] = getOpenChannel(nextNewOpenId);
					nextLayer.channels[end] = getOpenChannel(nextNewOpenId);
					nextNewOpenId++;
				} else {
//					DEBUG;
					return getInvalidLayer();
				}
			}
		}
	}
	
	for (int i = 0; i < WIDTH; i++) {
		if (isChannelOpen(nextLayer.channels[i]) && nextLayer.channels[i].id >= 0) {
			for (vector<pair<int, int> >::iterator it = mergedIdPair.begin(); it != mergedIdPair.end(); it++) {
				if (nextLayer.channels[i].id == it->first || nextLayer.channels[i].id == it->second) {
					nextLayer.channels[i].id = min(it->first, it->second);
					break;
				}
			}
		}
	}
	/*
	printLayer(layer);
	printLayer(nextLayer);
	DEBUG;
	*/
	return nextLayer;
}

void printHistory(const LayerTable *tables, int depth, ll startid, const Layer &layer) {
	ll previousMaxLayerHashCodes[depth+1];
	/*
	LayerTable::iterator it = tables[h+1].begin();
	Layer maxLayer = it->second;
	*/
	Layer layertemp = layer;
	previousMaxLayerHashCodes[depth] = startid;
	for (int i = depth-1; i > 0; i--) {
		previousMaxLayerHashCodes[i] = layertemp.previousMaxLayerHashCode;
		layertemp = tables[i].find(previousMaxLayerHashCodes[i])->second;
	}
	
	for (int i = 1; i <= depth; i++) {
		printLayer(tables[i].find(previousMaxLayerHashCodes[i])->second);
	}
}

void solve(const string* input) {
	for (int i = 0; i < h+2; i++) {
		availableFramLayers[i].reset();
	}
	availableFramLayers[0][0] = true;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (input[i][j] == '.') {
				availableFramLayers[i+1][j] = true;
			}
		}
	}
	availableFramLayers[h+1][w-1] = true;
	
	/*
	for (int i = 0; i < h+2; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (availableFramLayers[i][j]) {
				cout << ".";
			} else {
				cout << "#";
			}
		}
		cout << endl;
	}
	cout << endl;
	*/
	
	LayerTable tables[h+2];
	
	insertLayer(tables[0], makeInitialLayer());
	
	for (int i = 1; i < h+2; i++) {
		/*
		cout << "************************************************************************" << endl;
		cout << "************************************************************************" << endl;
		cout << i << endl;
		cout << "************************************************************************" << endl;
		cout << "************************************************************************" << endl;
		*/
		vector<bitset<WIDTH> > cannelPositionCandidates = getChannelPositionCandidates(availableFramLayers[i]);
		for (LayerTable::iterator it1 = tables[i-1].begin(); it1 != tables[i-1].end(); it1++) {
//			cout << "------------------------------------------------------------------------" << endl;
//			printLayer(it1->second);
//			cout << "------------------------------------------------------------------------" << endl;
			for (vector<bitset<WIDTH> >::iterator it2 = cannelPositionCandidates.begin(); it2 != cannelPositionCandidates.end(); it2++) {
				insertLayer(tables[i], makeNextLayer(it1->first, it1->second, *it2));
			}
		}
		
		/*
		cout << "////////////////////////////////////////////" << endl;
		cout << "////////////////////////////////////////////" << endl;
		for (LayerTable::iterator it1 = tables[i].begin(); it1 != tables[i].end(); it1++) {
			printHistory(tables, i, it1->first, it1->second);
			cout << endl;
		}
		*/
		
		
		
		/*
		DEBUG;
		for (int j = 0; j < WIDTH; j++) {
			if (availableFramLayers[i][j]) {
				cout << ".";
			} else {
				cout << "#";
			}
		}
		cout << endl;
		DEBUG;
		vector<bitset<WIDTH> > cannelPositionCandidates = getChannelPositionCandidates(availableFramLayers[i]);
		for (int k = 0; k < cannelPositionCandidates.size(); k++) {
			for (int j = 0; j < WIDTH; j++) {
				if (cannelPositionCandidates[k][j]) {
					cout << ".";
				} else {
					cout << "#";
				}
			}
			cout << endl;
		}
		*/
	}
	/*
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
		for (LayerTable::iterator it1 = tables[h+1].begin(); it1 != tables[h+1].end(); it1++) {
			printHistory(tables, h+1, it1->first, it1->second);
			cout << endl;
		}
	cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
	*/
	ll previousMaxLayerHashCodes[h+1];
//	cout << tables[h+1].size() << endl;
	LayerTable::iterator it = tables[h+1].begin();
	Layer maxLayer = it->second;
	for (int i = h; i > 0; i--) {
		previousMaxLayerHashCodes[i] = maxLayer.previousMaxLayerHashCode;
		maxLayer = tables[i][previousMaxLayerHashCodes[i]];
	}
	
	for (int i = 1; i <= h; i++) {
		for (int j = 0; j < w; j++) {
			if (!availableFramLayers[i][j]) {
				cout << "#";
			} else if (tables[i][previousMaxLayerHashCodes[i]].channels[j].occupied) {
				cout << "C";
			} else {
				cout << ".";
			}
		}
		cout << endl;
//		printLayer(tables[i][previousMaxLayerHashCodes[i]]);
	}
	cout << endl;
}

int main(void) {
	for (int casenumber = 1; true; casenumber++) {
		cin >> h >> w;
		if (h == 0 && w == 0) {
			break;
		}
		
		string input[h];
		for (int i = 0; i < h; i++) {
			cin >> input[i];
		}
		cout << "Case " << casenumber << ":" << endl;
		solve(input);
	}
}
