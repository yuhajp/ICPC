#include <iostream>
#include <vector>

using namespace std;

char radix_sub(int x)
{
    if (x < 0) { throw "shouldn't happen"; }
    if (x < 26) { return 'A' + x; }
    if (x < 52) { return ('a' + (x - 26)); }
    if (x < 62) { return ('0' + (x - 52)); }
    if (x == 62) { return '+'; }
    if (x == 63) { return '/'; }
    throw "shouldn't happen";
}

string radix(int x)
{
    string rev;
    while (x >= 64) {
        rev += radix_sub(x % 64);
        x /= 64;
    }
    rev += radix_sub(x);
    
    return string(rev.rbegin(), rev.rend());
}

bool scan(string str, int q, int& p, int& r)
{
    p = r = 0;
    for (int pp = 0; pp < q; ++pp) {
        int rr = 0;
        while (pp + rr < str.length() && str[pp + rr] == str[q + rr]) {
            ++rr;
        }
        if (rr > r) {
            r = rr;
            p = pp;
        }
    }

    return (r >= 7);
}

string conv(int x, int y)
{
    return "%" + radix(x) + "%" + radix(y);
}

void solve(string input)
{
    int q = 0;
    int p, r;
    while (q < input.length()) {
        if (scan(input, q, p, r)) {
            cout << conv(p, r) << endl;
            q = q + r;
        } else if (input[q] == '%') {
            cout << "%%";
            ++q;
        } else {
            cout << input[q];
            ++q;
        }
    }
}

int main(void)
{
    while (true) {
        string input;
        string line;
        while (getline(cin, line)) {
            if (line == "") {
                if (input.empty()) { return 0; }
                break;
            }
            input.append(line);
            input.append("\n");
        }
        
        solve(input);
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
    }
}
