#include<iostream>

using namespace std;

int main(void) {
	srand((unsigned) time(NULL));
	cout << "10 9" << endl;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 9; j++) {
			if (i == 0 && j == 0) {
				cout << ".";
			} else if (i == 9 && j == 8) {
				cout << ".";
			} else if (rand() % 10 == 0) {
				cout << "#";
			} else {
				cout << ".";
			}
		}
		cout << endl;
	}
	cout << "0 0" << endl;
}
