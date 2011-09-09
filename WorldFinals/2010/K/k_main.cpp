#include "k.h"

int main(void) {
	for (int casenumber = 1; true; casenumber++) {
		int input[18];
		for (int i = 0; i < 18; i++) {
			if (!(cin >> input[i])) {
				return 0;
			}
		}
		pair<double, double> answer = solve(input);
		printf("Case %d: %.5f %.5f\n", casenumber, answer.first, answer.second);
	}
}
