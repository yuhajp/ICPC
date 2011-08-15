#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(void)
{
    int plen, cnum, width, cspace;
    while (true) {
        cin >> plen; if (!plen) { break; }
        cin >> cnum >> width >> cspace;
        string str;
        getline(cin, str); // chop
        
        vector<string> lines;
        while (getline(cin, str)) {
            if (str == "?") { break; }
            if (str == "") {
                lines.push_back(string(width, '.'));
                continue;
            }
            while (str.length() != 0) {
                if (str.length() < width) {
                    string k(width, '.');
                    for (int i = 0; i < str.length(); ++i) {
                        k[i] = str[i];
                    }
                    lines.push_back(k);
                    break;
                } else {
                    lines.push_back(str.substr(0, width));
                    str = str.substr(width);
                }
            }
        }
        
        int page = 0;
        int pageLineSize = cnum * plen;
        while (page * pageLineSize < lines.size()) {
            for (int p = 0; p < plen; ++p) {
                for (int c = 0; c < cnum; ++c) {
                    if (c) {
                        cout << string(cspace, '.');
                    }
                    int idx = page * pageLineSize + p + c * plen;
                    if (idx < lines.size()) {
                        cout << lines[page * pageLineSize + p + c * plen];
                    } else {
                        cout << string(width, '.');
                    }
                }
                cout << endl;
            }
            cout << '#' << endl;
            ++page;
        }
        cout << '?' << endl;
        
    }
    return 0;
}
