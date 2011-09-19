/*
xhl_kogitsune

ROROROROROROROROR... にRを4つ入れる

OROROROR... + 4R = C(n+3,4)
RORORORO... + 4R = C(n+4,4)
C(n+3,4) + C(n+4,4) = (n+3)*(n+2)*(n+2)*(n+1)/12
*/
#include<iostream>
using namespace std;
int main( void )
{
	for(int C = 1; ; C ++ ){
		int N;
		cin >> N;
		if( N == 0 ) break;
		long long ret = 0;
		if( N >= 4 && (N-4) % 2 == 0 ){
			long long n = (N-4)/2;
			ret = (n+3)*(n+2)*(n+2)*(n+1)/12;
		}
		else
			ret = 0;
		cout << "Case " << C << ": " << ret << endl;
	}
	return 0;
}
