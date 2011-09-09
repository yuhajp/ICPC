#include<iostream>
using namespace std;
void printInstance(int i, int j, int k) {
  cout << "6 6" << endl;
  cout << (i/6) << " " << (i % 6) << " ";
  cout << (j/6) << " " << (j % 6) << " ";
  cout << (k/6) << " " << (k % 6) << endl;
}

int main(void) {
  for (int i = 2; i < 36; i++) {
    for (int j = 2; j < 36; j++) {
      if (i == j) continue;
      for (int k = 2; k < 36; k++) {
        if (k == i) continue;
        if (k == j) continue;
        printInstance(i, j, k);
      }
    }
  }
}
