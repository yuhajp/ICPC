#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

enum Type { ASSIGN, VALUE, EXPR };

struct Expr {
    Type   type;
    string ident;
    int    val;
    Expr*  e1;
    Expr*  e2;

    Expr(Type type,
         const string& ident = "", int val = 0, 
         Expr* e1 = NULL, Expr* e2 = NULL) :
        type(type), ident(ident), val(val), e1(e1), e2(e2)
    {
    }

    ~Expr() {
        delete e1;
        delete e2;
    }

    string to_string() const {
        string str;
        if (type == ASSIGN) {
            return ident + "[" + e1->to_string() + "] = " + e2->to_string();
        } else if (type == VALUE) {
            char buf[256];
            sprintf(buf, "%d", val);
            return buf;
        } else if (type == EXPR) {
            return ident + "[" + e1->to_string() + "]";
        } else {
            return "INVALID";
        }
    }
};

struct Array {
    int size;
    map<int, int> vals;

    Array(int size = 0) : size(size) {}
};

typedef map<string, Array> Environment;

// parse ident[expr]
Expr* parse_sub(const string& str)
{
    int pos = str.find_first_of('[');
    if (pos == string::npos) {
        return new Expr(VALUE, "", atoi(str.c_str()));
    }
    string ident = str.substr(0, pos);
    string expr  = str.substr(pos + 1, str.length() - (pos + 1) - 1);
    assert(str[str.length() - 1] == ']');
    
    Expr* e1 = parse_sub(expr);
    return new Expr(EXPR, ident, 0, e1);
}

Expr* parse(const string& str)
{
    int pos = str.find_first_of('=');
    if (pos == string::npos) {
        return parse_sub(str);
    } else {
        string left = str.substr(0, pos);
        string right = str.substr(pos + 1);
        Expr* e1 = parse_sub(left);
        Expr* e2 = parse_sub(right);
        e1->type = ASSIGN;
        e1->e2   = e2;
        return e1;
    }
}

int value(const map<string, Array>& env, Expr* exp)
{
    if (exp == NULL) { throw "bug"; }
    switch (exp->type) {
    case ASSIGN:
        throw "bug";
    case VALUE:
        return exp->val;
    case EXPR: {
        Environment::const_iterator it = env.find(exp->ident);
        if (it == env.end()) { throw "bug"; }
        const Array& arr = it->second;
        int v = value(env, exp->e1);
        map<int, int>::const_iterator that = arr.vals.find(v);
        if (that == arr.vals.end()) { throw "bug"; }
        return that->second;
    }
    default:
        throw "something wrong";
    }
}

int solve(const vector<Expr*>& exprs)
{
    map<string, Array> env;

    // top level is ASSIGN or EXPR
    for (int i = 0; i < exprs.size(); ++i) {
        try {
            const Expr* expr = exprs[i];
            switch (expr->type) {
            case ASSIGN: {
                Array& arr = env[expr->ident];
                int v1 = value(env, expr->e1);
                int v2 = value(env, expr->e2);
                if (v1 >= arr.size) { throw "bug"; }
                arr.vals[v1] = v2;
                break;
            }
            case EXPR: {
                env[expr->ident] = Array(value(env, expr->e1));                
                break;
            }
            default:
                throw "error";
            }
        } catch (const char* c) {
            return i + 1;
        } catch (...) {
        }
    }

    return 0;
}

int main(void)
{
    string line;
    vector<Expr*> exprs;
    while (getline(cin, line)) {
        if (line == ".") { 
            if (exprs.empty()) { return 0; }
            cout << solve(exprs) << endl; 
            for (int i = 0; i < exprs.size(); ++i) { delete exprs[i]; }
            exprs.clear();
            continue;
        }
        exprs.push_back(parse(line));
    }
}
