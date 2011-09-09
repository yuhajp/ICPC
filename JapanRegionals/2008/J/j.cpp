#include<iostream>
#include<vector>
#include<complex>
#include<set>

using namespace std;

typedef complex<double> cd;

typedef int State;
//10 - 4 - 4
//mask, src, dst

#define MAX_DISTANCE 1e10
#define EPS 1e-8
#define TABLE_SIZE 262144
#define INVALI_TURN_POINT (cd(MAX_DISTANCE, MAX_DISTANCE))

#define DEBUG cout<<"DEBUG:"<<__LINE__<<endl;

//‘å‚«‚¢‚©‚çGlobal ‚É‚Æ‚é‚—
int countTurnTable[TABLE_SIZE];
double distanceTable[TABLE_SIZE];

int getMask(State s) {
	return s >> 8;
}

int getSrc(State s) {
	return (s & 0xF0) >> 4;
}

int getDst(State s) {
	return s & 0xF;
}

State makeState(int mask, int src, int dst) {
	State output = 0;
	output |= mask;
	output <<= 4;
	output |= src;
	output <<= 4;
	output |= dst;
	return output;
}

bool online(cd p1, cd p2, cd target) {
	p2 -= p1;
	target -= p1;
	target /= p2;
	return abs(imag(target)) < EPS && -EPS < real(target) && real(target) < 1.0+EPS;
}

int getMask(int n, const cd* p, int src, int dst) {
	int mask = 0;
	for(int k = 0; k < n; k++) {
		if(online(p[src], p[dst], p[k])) {
			mask |= 1<<k;
		}
	}
	return mask;
}

//p1 + a1 * v1 = p2 + a2 * v2
//a1 * v1 = p2 - p1 + a2 * v2
//a1 * v1/v2 = (p2 - p1)/v2 + a2
//a1 * imag(v1/v2) = imag((p2-p1)/v2)
//a1 = imag((p2-p1)/v2) / imag(v1/v2)
cd validTurnPoint(cd pSrc, cd pDst, cd pNextSrc, cd pNextDst) {
	cd p1 = pSrc;
	cd v1 = pDst - pSrc;
	cd p2 = pNextDst;
	cd v2 = pNextSrc - pNextDst;
	if(abs(imag(v1/v2)) < EPS)return INVALI_TURN_POINT;
	double a1 = imag((p2-p1)/v2) / imag(v1/v2);
	double a2 = imag((p1-p2)/v1) / imag(v2/v1);
	if(1.0-EPS < a1 && 1.0-EPS < a2) {
		return p1+a1*v1;
	}
	return INVALI_TURN_POINT;
}

void solve(int n, cd* p) {
	for(int i = 0; i < TABLE_SIZE; i++) {
		countTurnTable[i] = 100;
		distanceTable[i] = MAX_DISTANCE;
	}
	
	set<State> queue;
	for(int src = 0; src < n; src++) {
		for(int dst = 0; dst < n; dst++) {
			if(src == dst)continue;
			int mask = getMask(n, p, src, dst);
			State state = makeState(mask, src, dst);
			countTurnTable[state] = 0;
			distanceTable[state] = abs(p[dst] - p[src]);
			queue.insert(state);
		}
	}
	int countTurn = 0;
	int finishState = (1 << n) - 1;
	while(true) {
		double answer = MAX_DISTANCE;
		set<State> nextQueue;
		int nextCountTurn = countTurn+1;
		for(set<State>::iterator it = queue.begin(); it != queue.end(); it++) {
			int mask = getMask(*it);
			int src = getSrc(*it);
			int dst = getDst(*it);
			if(mask == finishState) {
				answer = min(answer, distanceTable[*it]);
			}
			for(int nextSrc = 0; nextSrc < n; nextSrc++) {
				if((mask & (1<<nextSrc)) != 0 && nextSrc != dst)continue;
				for(int nextDst = 0; nextDst < n; nextDst++) {
					if(nextDst == dst)continue;
					if(nextDst == nextSrc)continue;
					if((mask & (1<<nextDst)) != 0)continue;
					cd turnPoint = validTurnPoint(p[src], p[dst], p[nextSrc], p[nextDst]);
					if(turnPoint == INVALI_TURN_POINT)continue;
					int nextMask = mask | getMask(n, p, nextSrc, nextDst);
					State nextState = makeState(nextMask, nextSrc, nextDst);
					if(countTurnTable[nextState] < nextCountTurn)continue;
					double nextDistance = distanceTable[*it] + abs(p[nextDst] - turnPoint) + abs(turnPoint - p[dst]);
					if(distanceTable[nextState] < nextDistance)continue;
					
					countTurnTable[nextState] = nextCountTurn;
					distanceTable[nextState] = nextDistance;
					nextQueue.insert(nextState);
				}
			}
		}
		if(answer < MAX_DISTANCE) {
			printf("%d %.5f\n", countTurn, answer);
			return;
		}
		queue = nextQueue;
		countTurn = nextCountTurn;
	}
}

int main(void) {
	while(true) {
		int n;
		cin >> n;
		if(n == 0)return 0;
		
		cd p[n];
		for(int i = 0; i < n; i++) {
			int x, y;
			cin >> x >> y;
			p[i] = cd(x, y);
		}
		solve(n, p);
	}
}
