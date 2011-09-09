#include<iostream>
#include<map>
#include<vector>
#include<cstdlib>
#include<string>

using namespace std;

typedef string ChemicalElement;
typedef int Number;
typedef map<ChemicalElement, Number> Molecule;
typedef vector<Molecule> MoleculeSequence;
typedef pair<MoleculeSequence, MoleculeSequence> ChemicalEquation;

typedef long long ll;

#define UNEXPECTED_OUTPUT(index) cout<<"UnexpectedCharacter["<<index<<"] on line:"<<__LINE__

void printMolecule(const Molecule &m);
void printMoleculeSequence(const MoleculeSequence &ms);
void printChemicalEquation(const ChemicalEquation &ce);

pair<ChemicalEquation, int> parseChemicalEquation(const string &input, int index);
pair<MoleculeSequence, int> parseMoleculeSequence(const string &input, int index);
pair<Molecule, int> parseMolecule(const string &input, int index);
pair<Molecule, int> parseGroup(const string &input, int index);
pair<Molecule, int> parseUnitGroup(const string &input, int index);
pair<ChemicalElement, int> parseChemicalElement(const string &input, int index);
pair<Number, int> parseNumber(const string &input, int index);

Molecule multiplyMolecule(const Molecule &molecule, int multiplier);
Molecule addMolecules(const Molecule &molecule1, const Molecule &molecule2);

void solve(const string &line);


void printMolecule(const Molecule &m) {
	cout << "[";
	for (Molecule::const_iterator it = m.begin(); it != m.end(); it++) {
		cout << "(" << it->first << ", " << it->second << ") ";
	}
	cout << "]";
}

void printMoleculeSequence(const MoleculeSequence &ms) {
	bool isFirst = true;
	for (MoleculeSequence::const_iterator it = ms.begin(); it != ms.end(); it++) {
		if (!isFirst) {
			cout << " + ";
		}
		printMolecule(*it);
		isFirst = false;
	}
}

void printChemicalEquation(const ChemicalEquation &ce) {
	printMoleculeSequence(ce.first);
	cout << " -> ";
	printMoleculeSequence(ce.second);
}



pair<ChemicalEquation, int> parseChemicalEquation(const string &input, int index) {
	pair<MoleculeSequence, int> f = parseMoleculeSequence(input, index);
	index = f.second;
	if (input[index] != '-' || input[index+1] != '>') {
		UNEXPECTED_OUTPUT(index);
		return make_pair(make_pair(MoleculeSequence(), MoleculeSequence()), input.size());
	}
	index += 2;
	pair<MoleculeSequence, int> s = parseMoleculeSequence(input, index);
	return make_pair(make_pair(f.first, s.first), s.second);
}

pair<MoleculeSequence, int> parseMoleculeSequence(const string &input, int index) {
	MoleculeSequence output;
	while (index < input.size()) {
		pair<Molecule, int> p = parseMolecule(input, index);
		output.push_back(p.first);
		index = p.second;
		if (input[index] != '+') {
			break;
		}
		index++;
	}
	return make_pair(output, index);
}

pair<Molecule, int> parseMolecule(const string &input, int index) {
	Molecule output;
	while (index < input.size() && (isupper(input[index]) ||input[index] == '(')) {
		pair<Molecule, int> p = parseGroup(input, index);
		output = addMolecules(output, p.first);
		index = p.second;
	}
	return make_pair(output, index);
}

//pair<

pair<Molecule, int> parseGroup(const string &input, int index) {
	pair<Molecule, int> p = parseUnitGroup(input, index);
	if (isdigit(input[p.second])) {
		pair<Number, int> number = parseNumber(input, p.second);
		return make_pair(multiplyMolecule(p.first, number.first), number.second);
	} else {
		return p;
	}
}

pair<Molecule, int> parseUnitGroup(const string &input, int index) {
	if (isupper(input[index])) {
		pair<ChemicalElement, int> p = parseChemicalElement(input, index);
		Molecule output;
		output[p.first] = 1;
		return make_pair(output, p.second);
	} else if(input[index] == '(') {
		pair<Molecule, int> p = parseMolecule(input, index+1);
		if (input[p.second] == ')') {
			return make_pair(p.first, p.second+1);
		} else {
			UNEXPECTED_OUTPUT(p.second);
			return make_pair(Molecule(), input.size());
		}
	}
}

pair<ChemicalElement, int> parseChemicalElement(const string &input, int index) {
	if (!isupper(input[index])) {
		UNEXPECTED_OUTPUT(index);
		return make_pair(string(""), input.size());
	}
	if (islower(input[index+1])) {
		return make_pair(input.substr(index, 2), index+2);
	} else {
		return make_pair(input.substr(index, 1), index+1);
	}
}

pair<Number, int> parseNumber(const string &input, int index) {
	if (!isdigit(input[index])) {
		UNEXPECTED_OUTPUT(index);
		return make_pair(-1, input.size());
	}
	if (isdigit(input[index+1])) {
		return make_pair(10*(input[index] - '0') + input[index+1] - '0', index+2);
	} else {
		return make_pair(input[index] - '0', index+1);
	}
}

Molecule multiplyMolecule(const Molecule &molecule, int multiplier) {
	Molecule output;
	for (Molecule::const_iterator it = molecule.begin(); it != molecule.end(); it++) {
		output[it->first] = it->second * multiplier;
	}
	return output;
}

Molecule addMolecules(const Molecule &molecule1, const Molecule &molecule2) {
	Molecule output = molecule1;
	for (Molecule::const_iterator it2 = molecule2.begin(); it2 != molecule2.end(); it2++) {
		Molecule::iterator ito = output.find(it2->first);
		if (ito != output.end()) {
			ito->second += it2->second;
		} else {
			output[it2->first] = it2->second;
		}
	}
	return output;
}


map<ChemicalElement, int> makeIndexToMoleculeTable(const MoleculeSequence &ms) {
	int index = 0;
	map<ChemicalElement, int> table;
	for (MoleculeSequence::const_iterator it1 = ms.begin(); it1 != ms.end(); it1++) {
		for (Molecule::const_iterator it2 = it1->begin(); it2 != it1->end(); it2++) {
			map<ChemicalElement, int>::const_iterator it3 = table.find(it2->first);
			if (it3 == table.end()) {
				table[it2->first] = index;
				index++;
			}
		}
	}
	return table;
}

vector<vector<ll> > makeMatrix(const ChemicalEquation &ce) {
	vector<vector<ll> > output;
	map<ChemicalElement, int> table = makeIndexToMoleculeTable(ce.first);
	
	int height = table.size();
	int width = ce.first.size() + ce.second.size();
	
	for (int i = 0; i < table.size(); i++) {
		vector<ll> list;
		for (int j = 0; j < width; j++) {
			list.push_back(0);
		}
		output.push_back(list);
	}
	
	int index = 0;
	for (MoleculeSequence::const_iterator it1 = ce.first.begin(); it1 != ce.first.end(); it1++) {
		for (Molecule::const_iterator it2 = it1->begin(); it2 != it1->end(); it2++) {
			output[table[it2->first]][index] = it2->second;
		}
		index++;
	}
	for (MoleculeSequence::const_iterator it1 = ce.second.begin(); it1 != ce.second.end(); it1++) {
		for (Molecule::const_iterator it2 = it1->begin(); it2 != it1->end(); it2++) {
			output[table[it2->first]][index] = -it2->second;
		}
		index++;
	}
	
	return output;
}

#define UNDEFINED -1

int getFirstElementIndex(const vector<ll> &v, int offset) {
	for (int i = offset; i < v.size(); i++) {
		if (v[i] != 0) {
			return i;
		}
	}
	return -1;
}

int getFirstElementIndex(const vector<ll> &v) {
	return getFirstElementIndex(v, 0);
}


bool isZeroVector(const vector<ll> &v) {
	return getFirstElementIndex(v) < 0;
}

vector<ll> substitution(const vector<ll> &a, const vector<ll> &b) {
	int aFirstIndex = getFirstElementIndex(a);
	ll aElement = a[aFirstIndex];
	ll bElement = b[aFirstIndex];
	
	if (bElement != 0) {
		vector<ll> output;
		for (int i = 0; i < a.size(); i++) {
			if (i != aFirstIndex) {
				output.push_back(b[i]*aElement - a[i]*bElement);
			} else {
				output.push_back(0);
			}
		}
		return output;
	} else {
		return b;
	}
}

vector<vector<int> > generateNextMatrix(const vector<vector<int> > &matrix) {
	vector<vector<int> > output;
	
}

ll gcd(ll a, ll b) {
	return a%b == 0 ? b : gcd(b, a%b);
}

ll gcdabs(ll a, ll b) {
//	return gcd(labs(a), labs(b));
	if (a < 0) {
		a *= -1;
	}
	if (b < 0) {
		b *= -1;
	}
	return gcd(a, b);
}

vector<ll> divVector(const vector<ll> &v, ll divisor) {
	vector<ll> output;
	for (vector<ll>::const_iterator it = v.begin(); it != v.end(); it++) {
		output.push_back(*it / divisor);
	}
	return output;
}

vector<ll> normalizeVector(const vector<ll> &v) {
	int firstIndex = getFirstElementIndex(v);
	ll g = v[firstIndex];
	for (int i = firstIndex + 1; i < v.size(); i++) {
		if (v[i] != 0) {
			g = gcdabs(g, v[i]);
		}
	}
	
	if (v[firstIndex] < 0) {
		return divVector(v, -g);
	} else {
		return divVector(v, g);
	}
}

bool sameVector(const vector<ll> &v1, const vector<ll> &v2) {
	for (int i = 0; i < v1.size(); i++) {
		if (v1[i] != v2[i]) {
			return false;
		}
	}
	return true;
}

bool containsVector(const vector<vector<ll> > &matrix, const vector<ll> v) {
	for (int i = 0; i < matrix.size(); i++) {
		if (sameVector(matrix[i], v)) {
			return true;
		}
	}
	return false;
}

vector<ll> multiplyVector(const vector<ll>& v, ll multiplier) {
	vector<ll> output;
	for (vector<ll>::const_iterator it = v.begin(); it != v.end(); it++) {
		output.push_back(*it * multiplier);
	}
	return output;
}

void printVector(const vector<ll> &v) {
	for (vector<ll>::const_iterator it = v.begin(); it != v.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}

void printMatrix(const vector<vector<ll> > &matrix) {
	for (vector<vector<ll> >::const_iterator it = matrix.begin(); it != matrix.end(); it++) {
		printVector(*it);
	}
}

vector<ll> solveMatrix(const vector<vector<ll> > &matrix) {
	if (matrix.size() == 1) {
		vector<ll> output;
		int firstElementIndex = -1;
		int secondElementIndex = -1;
		for (int i = 0; i < matrix[0].size(); i++) {
			if (matrix[0][i] != 0) {
				if (firstElementIndex < 0) {
					firstElementIndex = i;
				} else if(secondElementIndex < 0) {
					secondElementIndex = i;
				} else {
					cout << "ERROR @ sovleMatrix LINE: " << __LINE__ << endl;
				}
			}
			output.push_back(0);
		}
		ll firstElement = matrix[0][firstElementIndex];
		ll secondElement = matrix[0][secondElementIndex];
		if (secondElement < 0) {
			secondElement *= -1;
			firstElement *= -1;
		}
		output[firstElementIndex] = secondElement;
		output[secondElementIndex] = -firstElement;
		return output;
	} else {
		vector<ll> firstVector = matrix[0];
		vector<vector<ll> > nextMatrix;
		for (int i = 1; i < matrix.size(); i++) {
			vector<ll> nextVector = substitution(firstVector, matrix[i]);
			if (isZeroVector(nextVector)) {
				continue;
			}
			nextVector = normalizeVector(nextVector);
			if (!containsVector(nextMatrix, nextVector)) {
				nextMatrix.push_back(nextVector);
			}
		}
		
		vector<ll> result = solveMatrix(nextMatrix);
		int firstElementIndex = getFirstElementIndex(firstVector);
		ll firstElement = firstVector[firstElementIndex];
		result = multiplyVector(result, firstElement);
		ll sum = 0;
		for (int i = 0; i < firstVector.size(); i++) {
			sum += firstVector[i]*result[i];
		}
		result[firstElementIndex] = -sum / firstElement;
		return normalizeVector(result);
	}
}

vector<vector<ll> > reduceDuplicateVector(const vector<vector<ll> > &input) {
	vector<vector<ll> > output;
	for (int i = 0; i < input.size(); i++) {
		if (!containsVector(output, input[i])) {
			output.push_back(input[i]);
		}
	}
	return output;
}

void solve(const string &line) {
	pair<ChemicalEquation, int> parse = parseChemicalEquation(line, 0);
	vector<vector<ll> > matrix = makeMatrix(parse.first);
	matrix = reduceDuplicateVector(matrix);
	vector<ll> answer = solveMatrix(matrix);
	
	cout << answer[0];
	for (int i = 1; i < answer.size(); i++) {
		cout << " " << answer[i];
	}
	cout << endl;
}

int main(void) {
	while (true) {
		string line;
		cin >> line;
		if (line == ".") {
			break;
		}
		solve(line);
	}
}
