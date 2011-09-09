#include<string>
#include<iostream>
#include<sstream>
#include<vector>
#include<cstdio>

using namespace std;

#define AND		0
#define OR		1
#define XOR		2
#define NOT		3
#define IAND	4
#define IOR		5
#define IXOR	6
#define INOT	7
#define ONE		8
#define ZERO	9

#define UNDEF	2

#define DIFFERENCE_ORIGINAL_INVERTED 4

#define DEBUG cout<<"DEBUG:"<<__LINE__<<endl

//usage
// GATE_IMP[gateType][(reg[src1] << 1 ) | reg[src2]]
int GATE_IMP[10][4] = {
//		00	01	10	11	(src1, src2)
	{	0,	0,	0,	1},		//AND
	{	0,	1,	1,	1},		//OR
	{	0,	1,	1,	0},		//XOR
	{	1,	0,	0,	0},		//NOT(NAND)
	{	1,	1,	1,	0},		//inverted AND
	{	1,	0,	0,	0},		//inverted OR
	{	1,	0,	0,	1},		//inverted XOR
	{	0,	1,	1,	1},		//inverted NOT
	{	1,	1,	1,	1},		//always 1
	{	0,	0,	0,	0},		//always 0
};


string ERROR_TEXTS[3] = {
	"inverted",		"stuck at 1",		"stuck at 0",
};


typedef struct Gate {
	int src1;
	int src2;
	int type;
	int dst;
} gate;

int getRegisterIndex(int n, string str) {
	istringstream is(str);
	
	int num;
	char c;
	is >> c >> num;
	
	if(c == 'g') {
		return n + num;
	} else {
		return num;
	}
	return 0;
}

gate readGate(int n, int index) {
	gate output;
	
	char type;
	cin >> type;
	
	switch(type) {
	case 'a':
		output.type = AND;
		break;
	case 'o':
		output.type = OR;
		break;
	case 'x':
		output.type = XOR;
		break;
	case 'n':
		output.type = NOT;
		break;
	default:
		cout << "parse error" << endl;
		break;
	}
	
	if(output.type == NOT) {
		string src;
		cin >> src;
		output.src1 = getRegisterIndex(n, src);
		output.src2 = output.src1;
	} else {
		string src1, src2;
		cin >> src1 >> src2;
		output.src1 = getRegisterIndex(n, src1);
		output.src2 = getRegisterIndex(n, src2);
	}
	output.dst = n + index + 1;
	
	return output;
}


void sortGates(gate* gates, int n, int g) {
	bool calculatedRegister[n + g + 1];
	for(int i = 0; i <= n; i++) {
		calculatedRegister[i] = true;
	}
	for(int i = n+1; i <= n+g; i++) {
		calculatedRegister[i] = false;
	}
	
	//similar bubble sort
	for(int i = 0; i < g; i++) {
		for(int j = i; j < g; j++) {
			if(calculatedRegister[gates[j].src1] && calculatedRegister[gates[j].src2]) {
				calculatedRegister[gates[j].dst] = true;
				swap(gates[i], gates[j]);
				break;
			}
			if(j == g-1) {
				cout << "sort error" << endl;
			}
		}
	}
}


bool isNoConflict(const gate* gates, vector<vector<int> > observations, int n, int g) {
	int registers[n+g+1];
	for(vector<vector<int> >::iterator it = observations.begin(); it != observations.end(); it++) {
		for(int i = 0; i <= n+g; i++) {
			registers[i] = (*it)[i];
		}
		for(int i = 0; i < g; i++) {
			registers[gates[i].dst] = GATE_IMP[gates[i].type][(registers[gates[i].src1] << 1) | registers[gates[i].src2]];
		}
		for(int i = n+1; i <= n+g; i++) {
			if((*it)[i] != UNDEF && registers[i] != (*it)[i]) {
				return false;
			}
		}
	}
	return true;
}


string detectError(gate* gates, vector<vector<int> > observations, int n, int g) {
	string output = "";
	
	for(int i = 0; i < g; i++) {
		int originalGateType = gates[i].type;
		
		int testTypes[3] = {originalGateType+DIFFERENCE_ORIGINAL_INVERTED, ONE, ZERO};
		for(int j = 0; j < 3; j++) {
			gates[i].type = testTypes[j];
			if(isNoConflict(gates, observations, n, g)) {
				if(output != "") {
					return "Unable to totally classify the failure";
				}
				ostringstream oss;
				oss << "Gate " << (gates[i].dst - n) << " is failing; output " << ERROR_TEXTS[j];
				output = oss.str();
			}
		}
		gates[i].type = originalGateType;
	}
	
	if(output == "") {
		return "Unable to totally classify the failure";
	} else {
		return output;
	}
}

string solve(gate* gates, vector<vector<int> > observations, int n, int g) {
	if(isNoConflict(gates, observations, n, g)) {
		return "No faults detected";
	}
	return detectError(gates, observations, n, g);
}

int main(void) {
	for(int caseNumber = 1; true; caseNumber++) {
		int n, g, u;
		cin >> n >> g >> u;
		if(n == 0 && g == 0 && u == 0)break;
		
		// usage of register
		//0 -- ground
		//1 -- input 1
		//2 -- input 2
		// ...
		//n -- input n
		//1 -- gate 1
		//2 -- gate 2
		// ...
		//g -- gate g
		
		gate gates[g];
		for(int i = 0; i < g; i++) {
			gates[i] = readGate(n, i);
		}
		
		sortGates(gates, n, g);
		
		int output[u];
		for(int i = 0; i < u; i++) {
			int index;
			cin >> index;
			output[i] = n + index;
		}
		
		int b;
		cin >> b;
		vector<vector<int> > observations;
		for(int i = 0; i < b; i++) {
			vector<int> obs;
			for(int j = 0; j < n+g+1; j++) {
				obs.push_back(UNDEF);
			}
			//0 register
			obs[0] = 0;
			for(int j = 1; j <= n; j++) {
				cin >> obs[j];
			}
			for(int k = 0; k < u; k++) {
				cin >> obs[output[k]];
			}
			observations.push_back(obs);
		}
		
		cout << "Case " << caseNumber << ": " << solve(gates, observations, n, g) << endl;
	}
}
