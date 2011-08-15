#include <iostream>
#include <string>

using namespace std;

int parse(const string& str)
{
    int sum = 0;
    int idx = 0;
    int key = 1;
    while (idx < str.size()) {
        switch (str[idx]) {
        case 'm':
            sum += key * 1000;
            key = 1;
            break;
        case 'c':
            sum += key * 100;
            key = 1;
            break;
        case 'x':
            sum += key * 10;
            key = 1;
            break;
        case 'i':
            sum += key * 1;
            key = 1;
            break;
        default:
            if (str[idx] == '\r') { continue; }
            key = str[idx] - '0';
        }
        ++idx;
    }

    return sum;
}

string unparse(int t)
{
    string str;

    int m = t / 1000;
    int c = (t % 1000) / 100;
    int x = (t % 100) / 10;
    int i = t % 10;

    if (m != 0) {
        if (m == 1) { str = str + "m"; }
        else { str = str + string(1, '0' + m) + "m"; }
    }
    if (c != 0) {
        if (c == 1) { str = str + "c"; }
        else { str = str + string(1, '0' + c) + "c"; }
    }
    if (x != 0) {
        if (x == 1) { str = str + "x"; }
        else { str = str + string(1, '0' + x) + "x"; }
    }
    if (i != 0) {
        if (i == 1) { str = str + "i"; }
        else { str = str + string(1, '0' + i) + "i"; }
    }

    return str;
}

int main(void)
{
    int n; cin >> n;
    while (n--) {
        string str1, str2;
        cin >> str1 >> str2;
        cout << unparse(parse(str1) + parse(str2)) << endl;
    }

    return 0;
}
