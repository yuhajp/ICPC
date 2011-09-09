#include<iostream>
#include<map>
#include<vector>
#include<cstdio>

using namespace std;

#define DEBUG	cout<<"DEBUG:"<<__LINE__<<endl

typedef unsigned long long ull;


int adjacentTable[64][5];

int DI[4] = {1, 0, -1, 0};
int DJ[4] = {0, 1, 0, -1};

int NEGATIVE_COLOR_I[4] = { 0, 0, -1, -1};
int NEGATIVE_COLOR_J[4] = {-1, 0,  0, -1};

int POSITIVE_COLOR_I[4] = { 0, -1, -1,  0};
int POSITIVE_COLOR_J[4] = { 0,  0, -1, -1};

int makeIJToIndex(int i, int j) {
	return 8*i+j;
}

void printMask(int h, int w, ull mask, ull forbidden) {
	for (int i= 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			int bitmask = (1ULL << (makeIJToIndex(i, j)));
			bool isForbidden = ((forbidden & bitmask) != 0);
			bool isUsed = ((mask & bitmask) != 0);
			if (isForbidden && isUsed) {
				DEBUG;
				cout << "UNEXPECTED" << endl;
				cout << i << endl;
				cout << j << endl;
				return;
			} else if(isForbidden) {
				cout << "#";
			} else if(isUsed) {
				cout << "1";
			} else {
				cout << "0";
			}
		}
		cout << endl;
	}
	cout << endl;
}

void setupAdjacentTable(int h, int w) {
	for (int i = 0; i < 64; i++) {
		adjacentTable[i][0] = -1;
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			int index = makeIJToIndex(i, j);
			int count = 0;
			for (int k = 0; k < 4; k++) {
				int nexti = i + DI[k];
				int nextj = j + DJ[k];
				if (nexti < 0 || nextj < 0 || nexti >= h || nextj >= w) {
					continue;
				}
				int nextindex = makeIJToIndex(nexti, nextj);
				adjacentTable[index][count] = nextindex;
				count++;
				adjacentTable[index][count] = -1;
			}
		}
	}
}

int connected_map[10][10];

#define VISITED     1
#define UN_VISITED  2

int countUnVisitedDfs(int i, int j) {
  if (connected_map[i][j] == VISITED) {
    return 0;
  }
  connected_map[i][j] = VISITED;
  int sum = 1;
  for (int k = 0; k < 4; k++) {
    int nexti = i + DI[k];
    int nextj = j + DJ[k];
    sum += countUnVisitedDfs(nexti, nextj);
  }
  return sum;
}

bool isConnected(int h, int w, ull mask) {
  for (int i = 0; i < h+2; i++) {
    connected_map[i][0] = VISITED;
    connected_map[i][w+1] = VISITED;
  }
  for (int j = 0; j < w+2; j++) {
    connected_map[0][j] = VISITED;
    connected_map[h+1][j] = VISITED;
  }
  for (int i = 1; i <= h; i++) {
    for (int j = 1; j <= w; j++) {
      connected_map[i][j] = UN_VISITED;
    }
  }
  
  int expected = h*w;
  for (int k = 0; k < 64; k++) {
    if ((mask >> k) & 1) {
      connected_map[k / 8 + 1][k % 8 + 1] = VISITED;
      expected--;
    }
  }
  int actual = countUnVisitedDfs(1, 1);
  return expected == actual;
}


vector<map<ull, ull> > calculateVisitMaskAndCountHalf(int src, int length, ull forbidden) {
	/*
	DEBUG;
	cout << src << endl;
	cout << length << endl;
	printf("%016llX\n", forbidden);
	DEBUG;
	*/
	vector<map<ull, ull> > before(64);
	before[src][0] = 1;
	for (int i = 0; i < length; i++) {
//		DEBUG;
		vector<map<ull, ull> > after(64);
		for (int index = 0; index < 64; index++) {
//			DEBUG;
			for (int j = 0; true; j++) {
//				DEBUG;
				int nextIndex = adjacentTable[index][j];
//				cout << index << endl;
//				cout << j << endl;
//				cout << nextIndex << endl;
				if (nextIndex < 0) break;
				ull nextBit = (1ULL << nextIndex);
				if ((forbidden & nextBit) != 0) continue;
				
				for (map<ull, ull>::iterator it = before[index].begin(); it != before[index].end(); it++) {
//					DEBUG;
					if ((nextBit & it->first) != 0) {
						continue;
					}
					ull nextBitMask = it->first | nextBit;
					if (after[nextIndex].find(nextBitMask) == after[nextIndex].end()) {
						after[nextIndex][nextBitMask] = it->second;
					} else {
						after[nextIndex][nextBitMask] += it->second;
					}
				}
			}
		}
		
		before = after;
	}
	return before;
}

map<ull, ull> mergeVisitMaskAndCount(int h, int w, const vector<map<ull, ull> > &table1, const vector<map<ull, ull> > &table2) {
	map<ull, ull> output;
	for (int i = 0; i < 64; i++) {
		ull maskcommon = (1ULL << i);
		for (map<ull, ull>::const_iterator it1 = table1[i].begin(); it1 != table1[i].end(); it1++) {
			ull mask1 = it1->first;
			ull count1 = it1->second;
			for (map<ull, ull>::const_iterator it2 = table2[i].begin(); it2 != table2[i].end(); it2++) {
				ull mask2 = it2->first;
				ull count2 = it2->second;
				if ((mask1 & mask2) == maskcommon) {
					ull mask = mask1 | mask2;
					if (!isConnected(h, w, mask)) {
            continue;
          }
					
					if (output.find(mask) == output.end()) {
						output[mask] = count1 * count2;
					} else {
						output[mask] += count1 * count2;
					}
					/*
					cout << "--------------" << endl;
					DEBUG;
					printMask(3, 6, mask1, 0);
					DEBUG;
					printMask(3, 6, mask2, 0);
					DEBUG;
					printMask(3, 6, mask, 0);
					*/
				}
			}
		}
	}
	return output;
}

map<ull, ull> calculateVisitMaskAndCount(int h, int w, int src, int dst, int length, ull forbidden) {
	/*
	DEBUG;
	cout << src << endl;
	cout << dst << endl;
	cout << length << endl;
	printf("%016llX\n", forbidden);
	*/
	if (length % 2 == 1) {
//		DEBUG;
		vector<map<ull, ull> > srcTable = calculateVisitMaskAndCountHalf(src, length/2 + 1, forbidden);
//		DEBUG;
		vector<map<ull, ull> > dstTable = calculateVisitMaskAndCountHalf(dst, length/2 + 1, forbidden);
//		DEBUG;
		return mergeVisitMaskAndCount(h, w, srcTable, dstTable);
	} else {
//		DEBUG;
		vector<map<ull, ull> > srcTable = calculateVisitMaskAndCountHalf(src, length/2 + 1, forbidden);
//		DEBUG;
		vector<map<ull, ull> > dstTable = calculateVisitMaskAndCountHalf(dst, length/2, forbidden);
//		DEBUG;
		return mergeVisitMaskAndCount(h, w, srcTable, dstTable);
	}
}

/*
map<ull, ull> calculateVisitMaskAndCount2(int src, int mid, int dst, int len1, int len2, ull forbidden) {
//	DEBUG;
	map<ull, ull> srcmidTable = calculateVisitMaskAndCount(src, mid, len1, forbidden);
//	DEBUG;
	int len2a, len2b;
	if (len2  % 2 == 1) {
		len2a = len2/2 + 1;
		len2b = len2/2 + 1;
	} else {
		len2a = len2/2;
		len2b = len2/2 + 1;
	}
	
//	DEBUG;
	vector<map<ull, ull> > before(64);
	before[mid] = srcmidTable;
	for (int i = 0; i < len2a; i++) {
//		DEBUG;
		vector<map<ull, ull> > after(64);
		for (int index = 0; index < 64; index++) {
//			DEBUG;
			for (int j = 0; true; j++) {
//				DEBUG;
				int nextIndex = adjacentTable[index][j];
//				cout << index << endl;
//				cout << j << endl;
//				cout << nextIndex << endl;
				if (nextIndex < 0) break;
				ull nextBit = (1ULL << nextIndex);
				if ((forbidden & nextBit) != 0) continue;
				
				for (map<ull, ull>::iterator it = before[index].begin(); it != before[index].end(); it++) {
//					DEBUG;
					if ((nextBit & it->first) != 0) {
						continue;
					}
					ull nextBitMask = it->first | nextBit;
					if (after[nextIndex].find(nextBitMask) == after[nextIndex].end()) {
						after[nextIndex][nextBitMask] = it->second;
					} else {
						after[nextIndex][nextBitMask] += it->second;
					}
				}
			}
		}
		
		before = after;
	}
	
	
//	DEBUG;
	
	vector<map<ull, ull> > dstTable = calculateVisitMaskAndCountHalf(dst, len2b, forbidden);
//	DEBUG;
	
	return mergeVisitMaskAndCount(before, dstTable);
}
*/

int findTargetBit(const vector<pair<ull, ull> > &table, ull alreadyUsed) {
//	DEBUG;
//	cout << table.size() << endl;
	
	int counter[64];
	for (int i = 0; i < 64; i++) {
		counter[i] = 0;
	}
	for (vector<pair<ull, ull> >::const_iterator it = table.begin(); it != table.end(); it++) {
		for (int i = 0; i < 64; i++) {
			if(((it->first >> i) & 1) != 0) {
				counter[i]++;
			}
		}
	}
	int maxValue = 10;
	int maxIndex = -1;
	for (int i = 0; i < 64; i++) {
//		printf("[%2d]: %d %d\n", i, counter[i], (((1ULL << i) & alreadyUsed) != 0));
		if (((1ULL << i) & alreadyUsed) != 0) {
			continue;
		}
		if (maxValue < counter[i]) {
			maxValue = counter[i];
			maxIndex = i;
		}
	}
//	DEBUG;
//	printf("[%2d]: %d/%d, %f\n", maxIndex, maxValue, table.size(), (double)maxValue / table.size());
//	DEBUG;
	return maxIndex;
}
map<ull, ull> mergeNaive2(int h, int w, ull center, const vector<pair<ull, ull> > &table1, const vector<pair<ull, ull> > &table2) {
	map<ull, ull> next;
	for (vector<pair<ull, ull> >::const_iterator it1 = table1.begin(); it1 != table1.end(); it1++) {
		for (vector<pair<ull, ull> >::const_iterator it2 = table2.begin(); it2 != table2.end(); it2++) {
			if ((it1->first & it2->first) != 0) {
				continue;
			}
			ull mask = (it1->first | it2->first);
			if (!isConnected(h, w, mask | center)) {
        continue;
      }
			if (next.find(mask) == next.end()) {
				next[mask] = it1->second * it2->second;
			} else {
				next[mask] += it1->second * it2->second;
			}
		}
	}
	return next;
}


map<ull, ull> mergeNaive(const vector<pair<ull, ull> > &table1, const vector<pair<ull, ull> > &table2) {
	map<ull, ull> next;
	for (vector<pair<ull, ull> >::const_iterator it1 = table1.begin(); it1 != table1.end(); it1++) {
		for (vector<pair<ull, ull> >::const_iterator it2 = table2.begin(); it2 != table2.end(); it2++) {
			if ((it1->first & it2->first) != 0) {
				continue;
			}
			ull mask = (it1->first | it2->first);
			if (next.find(mask) == next.end()) {
				next[mask] = it1->second * it2->second;
			} else {
				next[mask] += it1->second * it2->second;
			}
		}
	}
	return next;
}

int bitCounter(ull n) {
	int output = 0;
	while (n) {
		output += (n & 1);
		n >>= 1;
	}
	return output;
}

vector<pair<ull, ull> > filter(const vector<pair<ull, ull> > &table, int index) {
	ull mask = (1ULL << index);
	vector<pair<ull, ull> > output;
	for (vector<pair<ull, ull> >::const_iterator it = table.begin(); it != table.end(); it++) {
		if ((it->first & mask) != 0) {
			continue;
		}
		output.push_back(*it);
	}
	return output;
}

map<ull, ull> mergeSmart(const vector<pair<ull, ull> > &table1, const vector<pair<ull, ull> > &table2, ull alreadyUsed) {
	if (bitCounter(alreadyUsed) >= 4) {
		return mergeNaive(table1, table2);
	}
//	DEBUG;
//	cout << table1.size() << endl;
//	cout << table2.size() << endl;
	if (table1.size() == 0 || table2.size() == 0) {
//		DEBUG;
//		cout << "NOOOOOOOOOOOOOOO" << endl;
		return map<ull, ull>();
	}
	if (table1.size() < 100 || table2.size() < 100) {
		return mergeNaive(table1, table2);
	}
//	DEBUG;
	
	int targetBit;
	if (table1.size() > table2.size()) {
		targetBit = findTargetBit(table1, alreadyUsed);
	} else {
		targetBit = findTargetBit(table2, alreadyUsed);
	}
	if (targetBit == -1) {
		return mergeNaive(table1, table2);
	}
	
	vector<pair<ull, ull> > filtered1 = filter(table1, targetBit);
	vector<pair<ull, ull> > filtered2 = filter(table2, targetBit);
//	printf("%d x %d -> %d x %d + %d x %d\n", table1.size(), table2.size(), table1.size(), filtered2.size(), filtered1.size(), table2.size());
	map<ull, ull> result1 = mergeSmart(table1, filtered2, alreadyUsed | (1ULL << targetBit));
	map<ull, ull> result2 = mergeSmart(filtered1, table2, alreadyUsed | (1ULL << targetBit));
	
	for (map<ull, ull>::iterator it = result2.begin(); it != result2.end(); it++) {
		if (result1.find(it->first) != result1.end()) {
			result1[it->first] += it->second;
		} else {
			result1[it->first] = it->second;
		}
	}
	
	return result1;
}
ull getAtLeastOneOnBits(const vector<pair<ull, ull> > &table) {
	ull result = 0;
	for (vector<pair<ull, ull> >::const_iterator it = table.begin(); it != table.end(); it++) {
		result |= it->first;
	}
	return result;
}
vector<pair<ull, ull> > filter2(const vector<pair<ull, ull> > &table, int index, bool flag) {
	ull mask = (1ULL << index);
	vector<pair<ull, ull> > output;
	for (vector<pair<ull, ull> >::const_iterator it = table.begin(); it != table.end(); it++) {
		if (((it->first & mask) != 0) == flag) {
			output.push_back(*it);
		}
	}
	return output;
}


map<ull, ull> mergeSmart2sub(const vector<pair<ull, ull> > &table1, const vector<pair<ull, ull> > &table2, ull mask) {
	if (table1.size() == 0 || table2.size() == 0) {
		return map<ull, ull>();
	}
	if (mask == 0) {
		return mergeNaive(table1, table2);
	}
	for (int i = 0; i < 64; i++) {
		if (((mask >> i) & 1)) {
			vector<pair<ull, ull> > subtable1t = filter2(table1, i, true);
			vector<pair<ull, ull> > subtable1f = filter2(table1, i, false);
			vector<pair<ull, ull> > subtable2t = filter2(table2, i, true);
			vector<pair<ull, ull> > subtable2f = filter2(table2, i, false);
			
//			printf("%d x %d -> %d x %d + %d x %d\n", table1.size(), table2.size(), subtable1t.size(), subtable2f.size(), subtable1f.size(), subtable2t.size());
			map<ull, ull> result1 = mergeSmart2sub(subtable1t, subtable2f, mask - (1ULL << i));
			map<ull, ull> result2 = mergeSmart2sub(subtable1f, subtable2t, mask - (1ULL << i));
	
			for (map<ull, ull>::iterator it = result2.begin(); it != result2.end(); it++) {
				if (result1.find(it->first) != result1.end()) {
					result1[it->first] += it->second;
				} else {
					result1[it->first] = it->second;
				}
			}
			return result1;
		}
	}
//	DEBUG;
	return map<ull, ull>();
}

map<ull, ull> mergeSmart2(const vector<pair<ull, ull> > &table1, const vector<pair<ull, ull> > &table2) {
	ull mask = getAtLeastOneOnBits(table1) | getAtLeastOneOnBits(table2);
//	DEBUG;
	return mergeSmart2sub(table1, table2, mask);
}

vector<pair<ull, ull> > mapToVector(const map<ull, ull> &table) {
	vector<pair<ull, ull> > output;
	for (map<ull, ull>::const_iterator it = table.begin(); it != table.end(); it++) {
		output.push_back(make_pair(it->first, it->second));
	}
	return output;
}

//first half is negative
//second half is positive

int cutDfsMap[8][8];
bool cutDfsNodeVisit[8][8];
int cutDfsColor[8][8];
bool shouldBeNegative[8][8];
bool shouldBePositive[8][8];
int cutDfsNextColor;

int cutDfsCountNegativeOrZero(int i, int j, int h, int w, int color) {
	if (i < 0 || h <= i || j < 0 || w <= j) {
		return 0;
	}
	if (cutDfsColor[i][j] == color) {
		return 0;
	}
	if (cutDfsMap[i][j] > 0) {
		return 0;
	}
	cutDfsColor[i][j] = color;
	int sum = 1;
	for (int k = 0; k < 4; k++) {
		int nexti = i + DI[k];
		int nextj = j + DJ[k];
		sum += cutDfsCountNegativeOrZero(nexti, nextj,h, w,  color);
	}
	return sum;
}

void cutDfs(int i, int j, int h, int w) {
	for (int k = 0; k < 4; k++) {
		int nexti = i + DI[k];
		int nextj = j + DJ[k];
		if (cutDfsNodeVisit[nexti][nextj]) {
			continue;
		}
		int negativei = i + NEGATIVE_COLOR_I[k];
		int negativej = j + NEGATIVE_COLOR_J[k];
		int positivei = i + POSITIVE_COLOR_I[k];
		int positivej = j + POSITIVE_COLOR_J[k];
		if (shouldBePositive[negativei][negativej] || shouldBeNegative[positivei][positivej]) {
			continue;
		}
		cutDfsMap[negativei][negativej]--;
		cutDfsMap[positivei][positivej]++;
		if (nexti <= 0 || h <= nexti || nextj <= 0 || w <= nextj) {
			//reach edge
			int area = cutDfsCountNegativeOrZero(0, 0, h, w, cutDfsNextColor);
			cutDfsNextColor++;
			/*
			if (area == h*w/2) {
			
				for (int a = 0; a < h; a++) {
					for (int b = 0; b < w; b++) {
						if (cutDfsMap[a][b] < 0) {
							cout << "-";
						} else if (cutDfsMap[a][b] > 0) {
							cout << "+";
						} else {
							cout << ".";
						}
					}
					cout << endl;
				}
				cout << endl;
			}
			*/
		} else {
			cutDfsNodeVisit[nexti][nextj] = true;
			cutDfs(nexti, nextj, h, w);
			cutDfsNodeVisit[nexti][nextj] = false;
		}
		
		
		cutDfsMap[negativei][negativej]++;
		cutDfsMap[positivei][positivej]--;
	}
}

void cut(int h, int w, const int* starti, const int* startj) {
//	DEBUG;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cutDfsMap[i][j] = 0;
			cutDfsNodeVisit[i][j] = false;
			cutDfsColor[i][j] = 0;
			shouldBeNegative[i][j] = false;
			shouldBePositive[i][j] = false;
		}
	}
	shouldBeNegative[starti[0]][startj[0]] = true;
	shouldBeNegative[starti[1]][startj[1]] = true;
	shouldBeNegative[starti[2]][startj[2]] = true;
	shouldBePositive[starti[3]][startj[3]] = true;
	shouldBePositive[starti[4]][startj[4]] = true;
	cutDfsMap[0][0]--;
	cutDfsMap[0][1]++;
	cutDfsNodeVisit[0][1] = true;
	cutDfsNodeVisit[1][1] = true;
	cutDfsNextColor = 1;
	
	cutDfs(1, 1, h, w);
	cout << cutDfsNextColor << endl;
}


typedef struct trie {
	struct trie *next[2];
	ull count;
} TRIE;

void initTrie(TRIE* trie) {
	trie->next[0] = NULL;
	trie->next[1] = NULL;
	trie->count = 0;
}

void addTrie(TRIE* trie, ull key, ull count) {
	for (int i = 0; i < 64; i++) {
		int bit = (key >> (63 - i)) & 1;
		if (trie->next[bit] == NULL) {
			trie->next[bit] = new TRIE();
			initTrie(trie->next[bit]);
		}
		trie = trie->next[bit];
	}
	trie->count = count;
}

void addTries(TRIE* trie, const vector<pair<ull, ull> > &tables) {
//	DEBUG;
	initTrie(trie);
//	DEBUG;
	for (vector<pair<ull, ull> >::const_iterator it = tables.begin(); it != tables.end(); it++) {
		addTrie(trie, it->first, it->second);
	}
}

void mergeTrie(TRIE* src1, TRIE* src2, TRIE* dst, int depth) {
	if (depth == 64) {
		dst->count += src1->count*src2->count;
		return;
	}
	if (src1->next[0] && src2->next[0]) {
		if (dst->next[0] == NULL) {
			dst->next[0] = new TRIE();
			initTrie(dst->next[0]);
		}
		mergeTrie(src1->next[0], src2->next[0], dst->next[0], depth+1);
	}
	if (src1->next[0] && src2->next[1]) {
		if (dst->next[1] == NULL) {
			dst->next[1] = new TRIE();
			initTrie(dst->next[1]);
		}
		mergeTrie(src1->next[0], src2->next[1], dst->next[1], depth+1);
	}
	if (src1->next[1] && src2->next[0]) {
		if (dst->next[1] == NULL) {
			dst->next[1] = new TRIE();
			initTrie(dst->next[1]);
		}
		mergeTrie(src1->next[1], src2->next[0], dst->next[1], depth+1);
	}
}

typedef struct trie4 {
	struct trie4 *next[16];
	ull count;
} TRIE4;

void initTrie4 (TRIE4* trie) {
	for (int i = 0; i < 16; i++) {
		trie->next[i] = NULL;
	}
	trie->count = 0;
}

void addTrie4(TRIE4* trie, ull key, ull count) {
	for (int i = 0; i < 16; i++) {
		int fourbits = (key >> (60-4*i)) & 0xF;
		if (trie->next[fourbits] == NULL) {
			trie->next[fourbits] = new TRIE4();
			initTrie4(trie->next[fourbits]);
		}
		trie = trie->next[fourbits];
	}
	trie->count = count;
}

void addTrie4s(TRIE4* trie, const vector<pair<ull, ull> > &tables) {
	initTrie4(trie);
//	DEBUG;
	for (vector<pair<ull, ull> >::const_iterator it = tables.begin(); it != tables.end(); it++) {
		addTrie4(trie, it->first, it->second);
	}
}

void mergeTrie4(TRIE4* src1, TRIE4* src2, TRIE4* dst, int depth) {
	if (depth == 16) {
		dst->count += src1->count*src2->count;
		return;
	}
	for (int i= 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			if ((i&j) || src1->next[i] == NULL || src2->next[j] == NULL) {
				continue;
			}
			if (dst->next[i|j] == NULL) {
				dst->next[i|j] = new TRIE4();
				initTrie4(dst->next[i|j]);
			}
			mergeTrie4(src1->next[i], src2->next[j], dst->next[i|j], depth+1);
		}
	}
}




vector<pair<ull, ull> > filterOutSeparated(int h, int w, const vector<pair<ull, ull> > &input) {
  vector<pair<ull, ull> > output;
  for (vector<pair<ull, ull> >::const_iterator it = input.begin(); it != input.end(); it++) {
    if (isConnected(h, w, it->first)) {
      output.push_back(*it);
    }
  }
  return output;
}

ull binarySearchFind(const vector<pair<ull, ull> > &array, ull target) {
  int minIndex = 0;
  int maxIndex = array.size() - 1;
  
  while (maxIndex - minIndex > 1) {
    int midIndex = (maxIndex + minIndex) / 2;
    if (array[midIndex].first < target) {
      
    } else if (array[midIndex].first > target) {
      
    } else {
      return array[midIndex].second;
    }
  }
  
  if (array[minIndex].first == target) {
    return array[minIndex].second;
  }
  if (array[maxIndex].first == target) {
    return array[maxIndex].second;
  }
  return 0;
}

ull solve(int h, int w, const int* starti, const int* startj) {
	setupAdjacentTable(h, w);
	
	int index[5];
	int checkpoint[5];
	ull forbidden = 0;
	
	for (int i = 0; i <= 4; i++) {
		index[i] = i*h*w / 4;
		checkpoint[i] = makeIJToIndex(starti[i], startj[i]);
		forbidden |= (1ULL << checkpoint[i]);
	}
	index[0] = 1;
	vector<pair<ull, ull> > tables[4];
	TRIE4 tries[4];
	vector<pair<ull, ull> > total;
	total.push_back(make_pair(0, 1));
	/*
	for (int i = 1; i <= 4; i++) {
		tables[i-1] = mapToVector(calculateVisitMaskAndCount(h, w, checkpoint[i-1], checkpoint[i], index[i] - index[i-1] - 1, forbidden));
	}
	
	map<ull, ull> merge1 = mergeNaive(tables[0], tables[1]);
	map<ull, ull> merge2 = mergeNaive(tables[2], tables[3]);
	*/
	
	vector<pair<ull, ull> > tables0 = mapToVector(calculateVisitMaskAndCount(h, w, checkpoint[1-1], checkpoint[1], index[1] - index[1-1] - 1, forbidden));
	vector<pair<ull, ull> > tables1 = mapToVector(calculateVisitMaskAndCount(h, w, checkpoint[2-1], checkpoint[2], index[2] - index[2-1] - 1, forbidden));
	map<ull, ull> merge1 = mergeNaive2(h, w, 1ULL << checkpoint[1], tables0, tables1);
//	DEBUG;
//	cout << tables0.size() << endl;
//	cout << tables1.size() << endl;
//	cout << merge1.size() << endl;
	tables0 = mapToVector(calculateVisitMaskAndCount(h, w, checkpoint[3-1], checkpoint[3], index[3] - index[3-1] - 1, forbidden));
	tables1 = mapToVector(calculateVisitMaskAndCount(h, w, checkpoint[4-1], checkpoint[4], index[4] - index[4-1] - 1, forbidden));
	map<ull, ull> merge2 = mergeNaive2(h, w, 1ULL << checkpoint[3], tables0, tables1);
//	DEBUG;
//	cout << tables0.size() << endl;
//	cout << tables1.size() << endl;
//	cout << merge2.size() << endl;
	
	ull mask = 0ULL;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			mask |= (1ULL << makeIJToIndex(i, j));
		}
	}
	mask -= forbidden;
	
	ull answer =0;
	for (map<ull, ull>::iterator it1 = merge1.begin(); it1 != merge1.end(); it1++) {
		map<ull, ull>::iterator it2 = merge2.find(mask - it1->first);
		if (it2 != merge2.end()) {
			answer += it1->second * it2->second;
		}
	}
	return answer;
}

int visit[10][10];
ull solveDfsSub(int i, int j, int index, const int* starti, const int* startj, const int* target) {
	for (int k = 4; k > 0; k--) {
		if (target[k] == index) {
			if (starti[k] != i || startj[k] != j) {
				return 0;
			}
			if (k == 4) {
        /*
				for (int a = 9; a >= 0; a--) {
					for (int b = 0; b < 10; b++) {
						if (visit[a][b] == 100) {
							printf(" xx");
						} else {
							printf("%3d", visit[a][b]);
						}
					}
					printf("\n");
				}
				printf("\n");
				*/
				
				return 1;
			} else {
				break;
			}
		} else {
			if (starti[k] == i && startj[k] == j) {
				return 0;
			}
		}
		if (target[k] < index) {
      break;
    }
	}
	ull sum = 0;
	for (int k = 0; k < 4; k++) {
		int nexti = i + DI[k];
		int nextj = j + DJ[k];
		if (visit[nexti][nextj] != 0) continue;
		visit[nexti][nextj] = index+1;
		sum += solveDfsSub(nexti, nextj, index+1, starti, startj, target);
		visit[nexti][nextj] = 0;
	}
	return sum;
}

ull solveDfs(int h, int w, const int* starti, const int* startj) {
	int target[5];
	int substarti[5];
	int substartj[5];
	target[0] = 1;
	for (int i = 1; i <= 4; i++) {
		substarti[i] = starti[i]+1;
		substartj[i] = startj[i]+1;
		target[i] = i*h*w / 4;
//		cout << target[i] << endl;
	}
	for (int i = 1; i <= 4; i++) {
//		cout << target[i] - target[i-1] << endl;
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			visit[i][j] = 100;
		}
	}
	for (int i = 1; i <= h; i++) {
		for (int j = 1; j <= w; j++) {
			visit[i][j] = 0;
		}
	}
	visit[1][1] = 1;
	return solveDfsSub(1, 1, 1, substarti, substartj, target);
}


int main(void) {
	for (int casenumber = 1; true; casenumber++) {
		int h, w;
		cin >> h >> w;
		if (h == 0 && w == 0) {
			break;
		}
		int starti[5];
		int startj[5];
		starti[0] = 0;
		startj[0] = 0;
		for (int i = 1; i <= 3; i++) {
			cin >> starti[i] >> startj[i];
		}
		starti[4] = 0;
		startj[4] = 1;
		
		int answer;
		if (h*w >= 18) {
      answer = solve(h, w, starti, startj);
    } else {
      answer = solveDfs(h, w, starti, startj);
    }
		
		/*
		int answer = solve(h, w, starti, startj);
		int dfsAnswer = solveDfs(h, w, starti, startj);
		if (answer != dfsAnswer) {
      for (int i = 1; i <= 3; i++) {
        cout << starti[i] << " " << startj[i] << " ";
      }
      cout << endl;
    }
    */
		cout << "Case " << casenumber << ": " << answer << endl;
//		DEBUG;
//		cout << dfsAnswer << endl;
//		DEBUG;
	}
}
