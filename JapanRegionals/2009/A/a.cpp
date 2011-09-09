#include<iostream>
#include<algorithm>

using namespace std;

int main(void) {
	while(true) {
		int w, d;
		cin >> w >> d;
		if (w == 0 && d == 0) {
			break;
		}
		
		int count_w[30];
		int count_d[30];
		fill(count_w, count_w + 30, 0);
		fill(count_d, count_d + 30, 0);
		for (int i = 0; i < w; i++) {
			int height;
			cin >> height;
			count_w[height]++;
		}
		for (int i = 0; i < d; i++) {
			int height;
			cin >> height;
			count_d[height]++;
		}
		
		int answer = 0;
		for (int i = 1; i <= 20; i++) {
			answer += i*max(count_w[i], count_d[i]);
		}
		cout << answer << endl;
	}
}
