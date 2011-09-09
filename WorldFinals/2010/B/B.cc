// -*- encoding: utf-8-unix -*-
// USED ALGORITHM: 
#define _USE_MATH_DEFINES
#include <cstdio>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <utility>
#include <map>
#include <set>
#include <queue>
#include <complex>

using namespace std;
typedef complex<double> P;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<vvi> vvvi;
typedef vector<double> vd;
typedef vector<string> vs;
#define ALL(c) (c).begin(), (c).end()
//#define FOR(c, it) for (typeof((c).begin()) it = (c).begin(); it != (c).end(); ++it)
#define REP(i, n) for (int i = 0; i < n; ++i)

// copied from hmuta :)
const int UNDEF = -1;
const int HYPHEN = 10;
const int SS = 11;
int DECODE_TABLE[] = {
	UNDEF,		//00000
	0,			//00001
	UNDEF,		//00010
	7,			//00011
	HYPHEN,		//00100
	4,			//00101
	SS,			//00110
	UNDEF,		//00111
	UNDEF,		//01000
	2,			//01001
	UNDEF,		//01010
	UNDEF,		//01011
	6,			//01100
	UNDEF,		//01101
	UNDEF,		//01110
	UNDEF,		//01111
	9,			//10000
	1,			//10001
	8,			//10010
	UNDEF,		//10011
	5,			//10100
	UNDEF,		//10101
	UNDEF,		//10110
	UNDEF,		//10111
	3,			//11000
	UNDEF,		//11001
	UNDEF,		//11010
	UNDEF,		//11011
	UNDEF,		//11100
	UNDEF,		//11101
	UNDEF,		//11110
	UNDEF,		//11111
};

template<typename T>
int convert(T& it) {
    int x = 0;
    x += *it++ << 4;
    x += *it++ << 3;
    x += *it++ << 2;
    x += *it++ << 1;
    x += *it++ << 0;
//    cout << x << ':' << DECODE_TABLE[x] << ' ';
    return DECODE_TABLE[x];
}

template<typename T>
bool decode(T begin, T end, vi& result)
{
    result.clear();
    
    T it = begin;
    // remove start
    if (convert(it) != SS) { return false; }
    if (*it != 0) { return false; }
    ++it;

    while (it + 5 < end) {
        int x = convert(it);
        if (x == UNDEF) { return false; }
        result.push_back(x);
        if (*it != 0) { return false; }
        ++it;
    }

    // remove stop
    if (convert(it) != SS) { return false; }
    assert(it == end);

    return true;
}

string solve(const vi& v)
{
    if (v.size() < 29 || v.size() % 6 != 5) { return "bad code"; }
    
    double mn = *std::min_element(v.begin(), v.end());
    double mx = *std::max_element(v.begin(), v.end());

    // range check
    {
        vi check;
        REP (i, v.size()) {
            double rn = v[i] / mn;
            double rx = v[i] / mx;
            if (1 <= rn && rn <= 1.05 / 0.95) { check.push_back(v[i]*2); }
            else if (0.95 / 1.05 <= rx && rx <= 1) { check.push_back(v[i]); }
            else { return "bad code"; } // ambiguous!
        }
        
        double mn1 = *std::min_element(check.begin(), check.end());
        double mx1 = *std::max_element(check.begin(), check.end());
        double rate = mx1 / mn1;
        if (1.05 / 0.95 < rate) { return "bad code"; }
    }
    
    
//    cout << "rate = " << rate << endl;
    
    // --- convert to wide(1) or narrow(0)
    vi bits;
    REP (i, v.size()) {
        double rn = v[i] / mn;
        double rx = v[i] / mx;
        if (1 <= rn && rn <= 1.05 / 0.95) { bits.push_back(0); }
        else if (0.95 / 1.05 <= rx && rx <= 1) { bits.push_back(1); }
        else { return "bad code"; } // ambiguous! .. but should not happen.
    }

    

//    cout << bits.size() << endl;

    // --- decode. both 'start' and 'stop' are removed here.
    vi result;
    if (decode(bits.begin(), bits.end(), result)) {
        // OK
    } else if (decode(bits.rbegin(), bits.rend(), result)) {
        // OK
    } else {
        return "bad code";
    }

//    cout << "decoded" << endl;

    // check C and K
    int N = result.size() - 2;
    int C = 0;
    for (int i = 0; i < N; ++i) {
        C += ((N - (i + 1)) % 10 + 1) * result[i];
    }
    C %= 11;
    if (C != result[N]) { return "bad C"; }

    int K = 0;
    for (int i = 0; i < N + 1; ++i) {
        K += ((N - (i + 1) + 1) % 9 + 1) * result[i];
    }
    K %= 11;
    if (K != result[N + 1]) { return "bad K"; }

    string answer;
    for (int i = 0; i < N; ++i) {
        if (0 <= result[i] && result[i] < 10) { answer += '0' + result[i]; }
        else { answer += '-'; }
    }

    return answer;
}

int main(void)
{
    int caseNo = 0;
    for (int N; cin >> N, N; ) {
        vi v(N); REP(i, N) { cin >> v[i]; }
        cout << "Case " << ++caseNo << ": " << solve(v) << endl;
    }

    return 0;
}

