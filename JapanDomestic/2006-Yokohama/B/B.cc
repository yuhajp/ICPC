#include <iostream>
#include <set>
#include <string>

using namespace std;

int solve(const string& str)
{
    set<string> ss;
    
    for (int i = 1; i < str.size(); ++i) {
        string left = str.substr(0, i);
        string right = str.substr(i);
        string rleft = string(left.rbegin(), left.rend());
        string rright = string(right.rbegin(), right.rend());
        ss.insert(left + right);
        ss.insert(left + rright);
        ss.insert(rleft + right);
        ss.insert(rleft + rright);
        ss.insert(right + left);
        ss.insert(right + rleft);
        ss.insert(rright + left);
        ss.insert(rright + rleft);
    }

    return ss.size();
}

int main(void)
{
    int N; cin >> N;
    while (N--) {
        string str; cin >> str;
        cout << solve(str) << endl;
    }
    return 0;
}

