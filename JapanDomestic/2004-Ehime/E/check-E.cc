#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
  ifstream fin1(argv[1]);
  ifstream fin2(argv[2]);

  double d1, d2;
  while (fin1 >> d1, fin2 >> d2) {
    if (fabs(d1 - d2) > 0.001) { cout << "error!" << endl; }
  }
}