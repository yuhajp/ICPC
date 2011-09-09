#include<iostream>
#include<algorithm>
#include<queue>
#include<vector>

using namespace std;

typedef struct swimmer {
	int requireTime;
	int restLaps;
	int expectedTimeToReachEnd;
} Swimmer;

#define INF (1<<28)

bool operator < (const Swimmer &swimmer1, const Swimmer &swimmer2) {
	return swimmer1.requireTime < swimmer2.requireTime;
}

int getFirstSwimmerReachTime(const queue<Swimmer> &q) {
	if (q.empty()) {
		return INF;
	}
	return q.front().expectedTimeToReachEnd;
}

int getSmallerQueueIndex(const queue<Swimmer> *qs) {
	int reachTime0 = getFirstSwimmerReachTime(qs[0]);
	int reachTime1 = getFirstSwimmerReachTime(qs[1]);
	if (reachTime0 < reachTime1) {
		return 0;
	}
	return 1;
}

vector<Swimmer> getReachingSwimmers(queue<Swimmer> &q) {
	vector<Swimmer> swimmers;
	int time = q.front().expectedTimeToReachEnd;
	while(!q.empty() && time >= q.front().expectedTimeToReachEnd) {
		Swimmer swimmer = q.front();
		q.pop();
		swimmer.restLaps--;
		if (swimmer.restLaps > 0) {
			swimmer.expectedTimeToReachEnd = time + swimmer.requireTime;
			swimmers.push_back(swimmer);
		}
	}
	return swimmers;
}

void pushAll(vector<Swimmer> &src, queue<Swimmer> &dst) {
	for (vector<Swimmer>::iterator it = src.begin(); it != src.end(); it++) {
		dst.push(*it);
	}
}

int main(void) {
	while (true) {
		int n;
		cin >> n;
		if (n ==0) {
			break;
		}
		
		queue<Swimmer> q[2];
		vector<Swimmer> swimmers;
		for (int i = 0; i < n; i++) {
			Swimmer swimmer;
			cin>> swimmer.requireTime >> swimmer.restLaps;
			swimmer.restLaps *= 2;
			swimmer.expectedTimeToReachEnd = swimmer.requireTime;
			swimmers.push_back(swimmer);
		}
		sort(swimmers.begin(), swimmers.end());
		pushAll(swimmers, q[0]);
		int answer = 0;
		while (!q[0].empty() || !q[1].empty()) {
			int queueIndex = getSmallerQueueIndex(q);
			answer = q[queueIndex].front().expectedTimeToReachEnd;
			vector<Swimmer> reachingSwimmers = getReachingSwimmers(q[queueIndex]);
			sort(reachingSwimmers.begin(), reachingSwimmers.end());
			pushAll(reachingSwimmers, q[1 - queueIndex]);
		}
		cout << answer << endl;
	}
}
