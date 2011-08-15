#include <iostream>
#include <string>
#include <vector>

using namespace std;

const char* alphas[] = {
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
    "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
    "ld", "mb", "mp", "nc", "nd", "ng", "nt", "nw", "ps", "qu", "cw", "ts",
};

const char* kans[] = {
    "ld", "mb", "mp", "nc", "nd", "ng", "nt", "nw", "ps", "qu", "cw", "ts"
};

const int SIZE = 26 + 12;

vector<int> tokenize(const string& str)
{
    vector<int> tokens;
    int pos = 0;
    while (pos < str.size()) {
        if (pos + 1 < str.size()) {
            for (int i = 0; i < 12; ++i) {
                if (str[pos] == kans[i][0] && str[pos + 1] == kans[i][1]) {
                    tokens.push_back(26 + i);
                    pos += 2;
                    goto NEXT;
                }
            }
        }

        if (str[pos] == ' ') { 
            tokens.push_back(-1);
        } else {
            tokens.push_back(str[pos] - 'a');
        }
        pos += 1;
    NEXT:
        ;
    }

    return tokens;
}

void count(int times[SIZE][SIZE], const string& str)
{
    vector<int> tokens = tokenize(str);
    if (tokens.size() < 2) { return; }

    for (int i = 0; i < tokens.size() - 1; ++i) {
        if (tokens[i] < 0 || tokens[i + 1] < 0) { continue; }
        times[tokens[i]][tokens[i + 1]] += 1;
    }
}

int main(void)
{
    string str;
    int N; cin >> N; getline(cin, str);
    
    int times[SIZE][SIZE];
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            times[i][j] = 0;
        }
    }
    while (N--) {
        getline(cin, str);
        count(times, str);
    }

    for (int i = 0; i < SIZE; ++i) {
        int idx = 0;
        for (int j = 0; j < SIZE; ++j) {
            if (times[i][j] > times[i][idx]) { idx = j; }
        }
        cout << alphas[i] << ' ' << alphas[it - times[i]] << ' ' << alphas[idx] << endl;
     }

    return 0;
}
