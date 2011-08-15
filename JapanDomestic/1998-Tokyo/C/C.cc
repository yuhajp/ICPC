#include <iostream>
#include <list>
#include <complex>
#include <cctype>

using namespace std;

typedef complex<int> cmpl;

enum Type {
    PLUS,
    MINUS,
    MULT,
    LPAREN,
    RPAREN,
    I,
    VAL
};

struct Token
{
    Type t;
    int v;

    Token(Type t, int v = 0) : t(t), v(v) {}
};

class OverflowException {};

list<Token> parse(const string& line)
{
    list<Token> rslt;
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] == '(') { rslt.push_back(Token(LPAREN)); }
        else if (line[i] == ')') { rslt.push_back(Token(RPAREN)); }
        else if (line[i] == 'i') { rslt.push_back(Token(I)); }
        else if (line[i] == '+') { rslt.push_back(Token(PLUS)); }
        else if (line[i] == '-') { rslt.push_back(Token(MINUS)); }
        else if (line[i] == '*') { rslt.push_back(Token(MULT)); }
        else if (isdigit(line[i])) {
            int v = line[i] - '0';
            while (isdigit(line[++i])) {
                v = v * 10 + line[i] - '0';
            }
            --i;
            rslt.push_back(Token(VAL, v));
            // cout << "VALUE: " << v << endl;
        }
    }

    return rslt;
}

inline
void checkOverflow(const cmpl& t)
{
    if (t.real() < -10000 || 10000 < t.real() ||
        t.imag() < -10000 || 10000 < t.imag()) {
        throw OverflowException();
    }
}

cmpl T(list<Token>&);
cmpl F(list<Token>&);

// - E -> T | E + T | E - T
// - T -> F | T * F
// - F -> i | val | ( E )
cmpl E(list<Token>& tokens) 
{
    cmpl t = T(tokens);

    while (!tokens.empty()) {
        switch (tokens.front().t) {
        case PLUS:
            tokens.pop_front();
            t += T(tokens);
            checkOverflow(t);
            break;
        case MINUS:
            tokens.pop_front();
            t -= T(tokens);
            checkOverflow(t);
            break;
        default:
            goto EXIT;
        }
    }
EXIT:
    return t;
}
    
cmpl T(list<Token>& tokens)
{
    cmpl t = F(tokens);
    while (!tokens.empty()) {
        if (tokens.front().t == MULT) {
            tokens.pop_front();
            t *= F(tokens);
            checkOverflow(t);
        } else {
            break;
        }
    }
    
    return t;
}
   
cmpl F(list<Token>& tokens) 
{
    if (tokens.empty()) {
        throw "parse error 3";
    }
    Token token = tokens.front();
    tokens.pop_front();
    switch (token.t) {
    case VAL: {
        cmpl x(token.v, 0);
        checkOverflow(x);
        return x;
    }
    case I: {
        return cmpl(0, 1);
    }
    case LPAREN: {
        cmpl x = E(tokens);
        tokens.pop_front(); // ')';
        return x;
    }
    default:
        throw "parse error 4";
    }
}         


string solve(const string& line)
{
    list<Token> tokens = parse(line);
    try {
        cmpl t = E(tokens);
        char buf[256];
        if (t.imag() == 0) { 
            sprintf(buf, "%d", t.real());
        } else if (t.real() == 0) {
            sprintf(buf, "%di", t.imag());
        } else {
            if (t.imag() < 0) {
                sprintf(buf, "%d-%di", t.real(), -t.imag());
            } else {
                sprintf(buf, "%d+%di", t.real(), t.imag());
            }

        }
        return string(buf);
    } catch (OverflowException& e) {
        return "overflow";
    } catch (const char* e) {
        cerr << e << endl;
        abort();
    }
}

int main(void)
{
    string line;
    while (getline(cin, line)) {
        cout << solve(line) << endl;
    }
}

