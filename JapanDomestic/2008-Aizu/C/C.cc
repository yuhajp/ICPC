#include <iostream>
#include <vector>
#include <string>

using namespace std;

int eval(string s, int& pos)
{
  if (s[pos] == '0') { ++pos; return 0; }
  if (s[pos] == '1') { ++pos; return 1; }
  if (s[pos] == '2') { ++pos; return 2; }
  if (s[pos] == '-') { ++pos; return 2 - eval(s, pos); }
  if (s[pos] == '(') { 
    ++pos; 
    int a = eval(s, pos);
    char c = s[pos++];
    int b = eval(s, pos);
    ++pos;
    if (c == '*') { return (a * b + 1) / 2; }
    else { return std::max(a, b); }
  }
}

int solve(const string& str)
{
  int cnt = 0;
  for (int P = 0; P <= 2; ++P) {
    for (int Q = 0; Q <= 2; ++Q) {
      for (int R = 0; R <= 2; ++R) {
	string s = str;
	for (int i = 0; i < s.length(); ++i) {
	  if (s[i] == 'P') { s[i] = P + '0'; }
	  if (s[i] == 'Q') { s[i] = Q + '0'; }
	  if (s[i] == 'R') { s[i] = R + '0'; }	  
	}
	int t = 0;
	if (eval(s, t) == 2) { ++cnt;  }
      }
    }
  }

  return cnt;
} 

int main(void)
{
  string str;
  while (getline(cin, str)) {
    if (str == ".") { return 0; }
    cout << solve(str) << endl;
  }
}
