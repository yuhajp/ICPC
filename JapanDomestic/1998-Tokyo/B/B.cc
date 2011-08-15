#include <iostream>
#include <vector>
#include <string>


using namespace std;

class Editor {
private:
    string line;
    int cursor;
public:
    Editor(string initial) : line(initial), cursor(0) {}

    void forwardChar() {
        if (line.size() != cursor) { ++cursor; }
    }
    void forwardWord() {
        while (cursor < line.size() && line[cursor] == ' ') { ++cursor; }
        while (cursor < line.size() && line[cursor] != ' ') { ++cursor; }
    }
    void backwardChar() {
        if (cursor != 0) { --cursor; }
    }
    void backwardWord() {
        while (cursor > 0 && line[cursor] == ' ') { --cursor; }
        while (cursor > 0 && line[cursor] != ' ') { --cursor; }
    }
    void insert(string str) {
        // cout << "INSERTING : " << str << endl;
        line.insert(cursor, str);
        cursor += str.length();
    }
    void deleteChar() {
        if (cursor != line.size()) {
            line.erase(cursor, 1);
        }
    }
    void deleteWord() {
        int idx = cursor;
        bool found = false;
        while (idx < line.size() && line[idx] == ' ') { ++idx; }
        while (idx < line.size() && line[idx] != ' ') { ++idx; found = true; }
        if (found) {
            line.erase(cursor, idx - cursor);
        }

    }
    void printAnswer() {
        for (int i = 0; i < line.size(); ++i) {
            if (cursor == i) { cout << '^'; }
            cout << line[i];
        }
        if (cursor == line.size()) {
            cout << '^';
        }
        cout << endl;
    }
};


int main(void)
{
    string str;
    int N; cin >> N; getline(cin, str);
    for (int i = 0; i < N; ++i) {
        string initial;
        int n;
        getline(cin, initial);
        cin >> n; getline(cin, str);
        Editor editor(initial);
        for (int j = 0; j < n; ++j) {
            getline(cin, str);
            if      (str == "forward char")  { editor.forwardChar(); }
            else if (str == "forward word")  { editor.forwardWord(); }
            else if (str == "backward char") { editor.backwardChar(); }
            else if (str == "backward word") { editor.backwardWord(); }
            else if (str == "delete char")   { editor.deleteChar(); }
            else if (str == "delete word")   { editor.deleteWord(); }
            else {
                // must be insert.
                editor.insert(str.substr(8, str.length() - 9));
            }
            //editor.printAnswer();
        }
        editor.printAnswer();
    }
    
}
