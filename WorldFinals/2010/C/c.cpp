#include<iostream>
#include<algorithm>
#include<vector>
#include<set>

using namespace std;

#define DEBUG cout<<"DEBUG:"<<__LINE__<<endl
#define BLOCK	0
#define EMPTY	1

typedef long long ll;

typedef struct range {
	ll xMin;
	ll xMax;
	int type;
} Range;

typedef struct layer {
	ll width;
	vector<Range> ranges;
} Layer;

void printRange(const Range &range) {
	cout << "[" << range.xMax << ", " << range.xMin << ", ";
	if (range.type == BLOCK) {
		cout << "B";
	} else {
		cout << "E";
	}
	cout << "]";
}

void printRanges2(const vector<Range> &ranges) {
	for (vector<Range>::const_iterator it = ranges.begin(); it != ranges.end(); it++) {
		for (int x = it->xMin; x <= it->xMax; x++) {
			if (it->type == BLOCK) {
				cout << "B";
			} else {
				cout << ".";
			}
		}
	}
	cout << endl;
}

void printRanges(const vector<Range> &ranges) {
	for (vector<Range>::const_iterator it = ranges.begin(); it != ranges.end(); it++) {
		printRange(*it);
	}
}

void printLayers(const vector<Layer> &layers) {
	for (vector<Layer>::const_iterator it = layers.begin(); it != layers.end(); it++) {
		cout << it->width << ", ";
		printRanges(it->ranges);
		cout << endl;
	}
}

Range generateRange(ll xMin, ll xMax, int type) {
	Range r;
	r.xMin = xMin;
	r.xMax = xMax;
	r.type = type;
	return r;
}

Layer generateLayer(ll width, vector<pair<ll, ll> > blocks, ll n) {
	Layer output;
	output.width = width;
	if (blocks.size() > 0) {
		if (blocks[0].second < n-1) {
			output.ranges.push_back(generateRange(blocks[0].second + 1, n-1, EMPTY));
		}
		for (int i = 0; i < blocks.size()-1; i++) {
			output.ranges.push_back(generateRange(blocks[i].first, blocks[i].second, BLOCK));
			if (blocks[i].first > blocks[i+1].second + 1) {
				output.ranges.push_back(generateRange(blocks[i+1].second + 1, blocks[i].first-1, EMPTY));
			}
		}
		output.ranges.push_back(generateRange(blocks[blocks.size()-1].first, blocks[blocks.size()-1].second, BLOCK));
		if (blocks[blocks.size()-1].first > 0) {
			output.ranges.push_back(generateRange(0, blocks[blocks.size()-1].first-1, EMPTY));
		}
	} else {
		output.ranges.push_back(generateRange(0, n-1, EMPTY));
	}
	return output;
}

vector<Layer> generateLayers(ll m, ll n, int w, const ll* x1, const ll *x2, const ll *y) {
	vector<vector<pair<ll, ll> > > tempList(m);
	set<ll> blockExistYs;
	for (int i = 0; i < w; i++) {
		tempList[y[i]].push_back(make_pair(x1[i], x2[i]));
		blockExistYs.insert(y[i]);
	}
	for (int i = 0; i < m; i++) {
		sort(tempList[i].rbegin(), tempList[i].rend());
	}
	
	ll previousBlockExistY = m;
	vector<layer> layers;
	for (set<ll>::const_reverse_iterator it = blockExistYs.rbegin(); it != blockExistYs.rend(); it++) {
		ll currentBlockExistY = *it;
		if (currentBlockExistY < previousBlockExistY - 1) {
			layers.push_back(generateLayer(previousBlockExistY - 1 - currentBlockExistY, vector<pair<ll, ll> >(), n));
		}
		layers.push_back(generateLayer(1, tempList[currentBlockExistY], n));
		
		previousBlockExistY = currentBlockExistY;
	}
	set<ll>::const_iterator it = blockExistYs.begin();
	if (it == blockExistYs.end()) {
		layers.push_back(generateLayer(m, vector<pair<ll, ll> >(), n));
	} else if(*it > 0) {
		layers.push_back(generateLayer(*it, vector<pair<ll, ll> >(), n));
	}
	return layers;
}

vector<Range> simplifyRanges(const vector<Range> &ranges) {
	vector<Range> output;
	Range currentRange = ranges[0];
	for (int i = 1; i < ranges.size(); i++) {
		if (currentRange.type == ranges[i].type) {
			currentRange.xMin = ranges[i].xMin;
		} else {
			output.push_back(currentRange);
			currentRange = ranges[i];
		}
	}
	output.push_back(currentRange);
	return output;
}
pair<vector<Range>, ll> mergedLayerAndCountBlockedArea(const vector<Range> &previous, const vector<Range> &current, ll n) {
	int pIndex = 0;
	int cIndex = 0;
	
	vector<Range> outputRanges;
	ll outputCount = 0;
	
	ll previousX = n;
	int previousType = BLOCK;
	while (pIndex < previous.size() && cIndex < current.size()) {
		
		Range pRange = previous[pIndex];
		Range cRange = current[cIndex];
		ll maxXMin = max(pRange.xMin, cRange.xMin);
		int nextType;
		
		if (pRange.type == BLOCK && previousType == BLOCK) {
			nextType = BLOCK;
			if (cRange.type != BLOCK) {
				outputCount += previousX - maxXMin;
			}
		} else if(cRange.type == BLOCK) {
			nextType = BLOCK;
		} else {
			nextType = EMPTY;
		}
		outputRanges.push_back(generateRange(maxXMin, previousX - 1, nextType));
		previousX = maxXMin;
		previousType = nextType;
		
		if (pRange.xMin > cRange.xMin) {
			pIndex++;
		} else if(cRange.xMin > pRange.xMin) {
			cIndex++;
		} else {
			pIndex++;
			cIndex++;
		}
	}
	return make_pair(simplifyRanges(outputRanges), outputCount);
}

vector<Range> generateInitialPreviousRanges(ll n) {
	vector<Range> output;
	output.push_back(generateRange(n-1, n-1, EMPTY));
	if (n-2 >= 0) {
		output.push_back(generateRange(0, n-2, BLOCK));
	}
	return output;
}


ll solve(ll m, ll n, int w, const ll* x1, const ll* x2, const ll* y) {
	vector<Layer> layers = generateLayers(m, n, w, x1, x2, y);
	vector<Range> previousRanges = generateInitialPreviousRanges(n);
	ll output = 0;
	for (vector<Layer>::const_iterator it = layers.begin(); it != layers.end(); it++) {
		pair<vector<Range>, ll> result = mergedLayerAndCountBlockedArea(previousRanges, it->ranges, n);
		output += result.second * it->width;
		previousRanges = result.first;
	}
	return output;
}

int main(void) {
	for (int casenumber = 1; true; casenumber++) {
		ll m, n;
		int w;
		cin >> m >> n >> w;
		if (m == 0 && n == 0 && w == 0) {
			break;
		}
		
		ll x1[w], x2[w], y[w], temp;
		for (int i = 0; i < w; i++) {
			cin >> x1[i] >> y[i] >> x2[i] >> temp;
		}
		cout << "Case " << casenumber << ": " << solve(m, n, w, x1, x2, y) << endl;
	}
}
