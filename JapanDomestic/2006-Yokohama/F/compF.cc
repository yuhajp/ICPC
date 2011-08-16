#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
int main(int argc, char* argv[])
{
    ifstream fin1(argv[1]);
    ifstream fin2(argv[2]);
    
    double d1, d2;
    while ((fin1 >> d1) && (fin2 >> d2)) {
        if (abs(d1 - d2) > 1E-10) { cout << "error!" << d1 << "--" << d2 << endl; }
    }
}
