// author: mayah

#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
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
#include <cmath>
#include <cassert>
#include <sstream>

using namespace std;
typedef complex<double> P;

struct Poly {
    vector<long long> coff;
    long long q;

    // f(x)/q の、left に f(x) を表す文字列が、right に q を表す文字列が入っている。
    Poly(const string& left, const string& right) : coff(1) {
        // q は普通に atoi で int にすれば OK
        q = atoi(right.c_str());
        // f(x) は parse する。
        parse(left);
    }
    
    // 次元
    int dim() {
        return coff.size() - 1;
    }

    // f(x) を parse する関数
    void parse(const string& s) 
    {
        int pos = 0;

        // ループ一回で、+ a x^k 一つ分を parse している。
        while (pos < s.length()) {
            bool positive = true;
            if (s[pos] == '+') { ++pos; }
            else if (s[pos] == '-') { positive = false; ++pos; }
            
            int c = 0;
            while (pos < s.length() && '0' <= s[pos] && s[pos] <= '9') {
                c = c * 10 + (s[pos] - '0');
                ++pos;
            }
            if (c == 0) { c = 1; }
            
            if (pos == s.length()) {
                coff[0] = positive ? c : -c;
                break;
            }

            assert(s[pos] == 'n');
            ++pos;

            int p = 0;
            if (pos < s.length() && s[pos] == '^') {
                ++pos;
                while (pos < s.length() && '0' <= s[pos] && s[pos] <= '9') {
                    p = p * 10 + (s[pos] - '0');
                    ++pos;
                }
            } else {
                p = 1;
            }
            
            if (coff.size() <= p) { coff.resize(p + 1); }
            coff[p] = positive ? c : -c; 
        }
    } 

    // for debugging
    void print() {
        for (int i = coff.size() - 1; i >= 0; --i) {
            cout << coff[i] << 'n' << '^' << i << ' ';
        }
        cout << endl;
    }

    // f(x)/q が int かどうか check。ここでは f(x) mod q == 0 => f(x)/q が int なので
    // それを check。途中で overflow する可能性があるので long long で計算している。
    bool testInt(long long n) {
        // cout << "f(" << n << ") = ?" << endl;
        long long sum = 0;
        long long p = 1;        
        for (int i = 0; i < coff.size(); ++i) {
            sum = (sum + p * (coff[i] % q)) % q;
            p = (p * n) % q;
        }

        return sum == 0;
    }
};

// パーズの準備
Poly parse(const string& str)
{
    // str は f(x)/q の形なので、f(x) と q の２つにわけているだけ。

    int idx = str.find('/');
    if (idx == -1) { abort(); } // shouldn't happen
    string left = str.substr(1, idx - 2);
    string right = str.substr(idx + 1);

    return Poly(left, right);
}

// always integer かどうか判断する
bool solve(const string& str)
{
    // 式を parse して
    Poly p = parse(str);
    // p.print();

    // 0, ..., n の全てで式の値が int であれば always integer である。
    // testInt が int かどうかを check する method
    for (int i = 0; i <= p.dim(); ++i) {
        if (!p.testInt(i)) { return false; }
    }

    return true;
}

int main(void)
{
    string str;
    int i = 0;
    
    // 読み込み
    while (cin >> str) {
        if (str == "" || str[0] == '.') { break; }

        // integer かどうか判断
        if (solve(str)) {
            cout << "Case " << (++i) << ": Always an integer" << endl;
        } else {
            cout << "Case " << (++i) << ": Not always an integer" << endl;
        }
    }
    return 0;
}

