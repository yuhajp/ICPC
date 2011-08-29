#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool solve(const string& str)
{
    vector<char> parens;

    for (int i = 0; i < str.size(); ++i) {
        switch (str[i]) {
        case '(':
            parens.push_back('('); break;
        case ')':
            if (!parens.empty() && parens.back() == '(')
                parens.pop_back();
            else
                return false;
            break;
        case '[':
            parens.push_back('['); break;
        case ']':
            if (!parens.empty() && parens.back() == '[')
                parens.pop_back();
            else
                return false;
            break;
        }
    }

    return parens.empty();
}

int main(void)
{
    while (true) {
        string whole, str;
        while (getline(cin, str)) {
            if (str == ".") return 0;
            whole += str;
            if (str[str.length() - 1] == '.')
                break;
        }
        cout << (solve(whole) ? "yes" : "no") << endl;
    }

    return 0;
}
