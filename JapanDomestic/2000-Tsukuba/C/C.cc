#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Rule {
    bool permit;
    string src;
    string dst;

    Rule(bool permit, string src, string dst) : 
        permit(permit), src(src), dst(dst) 
    {
    }
    

    bool match(string s, string d)
    {
        for (int i = 0; i < src.size(); ++i) {
            if (src[i] == '?') { continue; }
            if (src[i] == s[i]) { continue; }
            return false;
        }
        for (int i = 0; i < dst.size(); ++i) {
            if (dst[i] == '?') { continue; }
            if (dst[i] == d[i]) { continue; }
            return false;
        }
        return true;
    }
};

struct Message {
    string src;
    string dst;
    string msg;

    Message(string src, string dst, string msg) :
        src(src), dst(dst), msg(msg) {}
};

int main(void)
{
    for (int N, M; cin >> N >> M, (N || M); ) {
        vector<Rule> rules;
        rules.reserve(N);
        for (int i = 0; i < N; ++i) {
            string type, src, dst;
            cin >> type >> src >> dst;
            rules.push_back(Rule(type == "permit", src, dst));
        }


        vector<Message> messages;
        for (int i = 0; i < M; ++i) {
            string src, dst, msg;
            cin >> src >> dst >> msg;
            for (int j = N - 1; j >= 0; --j) {
                if (rules[j].match(src, dst)) {
                    if (rules[j].permit) {
                        messages.push_back(Message(src, dst, msg));
                    }
                    break;
                }
            }
        }
        
        cout << messages.size() << endl;
        for (int i = 0; i < messages.size(); ++i) {
            cout << messages[i].src << ' '
                 << messages[i].dst << ' '
                 << messages[i].msg << endl;
        }
    }
}
