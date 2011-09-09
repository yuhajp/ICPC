#include<iostream>
#include<vector>
#include<map>
#include<sstream>


using namespace std;

typedef vector<int> ARRAY_1D;
typedef vector<ARRAY_1D> ARRAY_2D;
typedef vector<ARRAY_2D> ARRAY_3D;

typedef struct element {
  int flag;
  ARRAY_1D array1d;
  ARRAY_2D array2d;
  ARRAY_3D array3d;
  string name;
} Element;

typedef map<string, Element> Environment;




#define DEBUG	cout<<"DEBUG:"<<__LINE__<<endl

#define SHOW_FAIL_REASON


#ifdef SHOW_FAIL_REASON
#define REPORT(s) cout <<"Fail("<<__LINE__<<"):"<<s<<endl
#else
#define REPORT
#endif



#define TYPE_PLUS      0
#define TYPE_MINUS     1
#define TYPE_MULTIPLY  2
#define TYPE_SLASH     3
#define TYPE_EQUAL     4
#define TYPE_IOTA      5
#define TYPE_RHO       6
#define TYPE_DROP      7
#define TYPE_OPEN      8
#define TYPE_CLOSE     9
//#define TYPE_NUMBER    10
#define TYPE_ARRAY_1D  11
#define TYPE_ARRAY_2D  12
#define TYPE_ARRAY_3D  13
#define TYPE_VARIABLE  14
#define TYPE_NULL      15

#define EXCEPTION_RE    1
#define EXCEPTION_WA    2


Element makeElementFlagOnly(int flag);
Element convertStringToElement(string token);


vector<Element> parse(string line);


Element solve(Environment &environment, string line);

string toString1D(const vector<int> &v);
string toString2D(const vector<vector<int> > &v);
string toString3D(const vector<vector<vector<int> > > &v);

void print(const Element &element);
