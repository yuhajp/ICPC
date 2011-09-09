#include<iostream>
#include<cstdio>
#include<string>
#include<sstream>
#include<vector>

using namespace std;

//rank, isRed
typedef pair<int, bool> card;

#define P_INF (1<<28)
#define M_INF (-(1<<28))

#define BLANK_CARD card(0, true)

#define DEBUG cout<<"DEBUG:"<<__LINE__<<endl

card floors[3][3];
card peaks[4][8];
vector<card> deck;
card holdingCard[2];

int initialSimulateScore[] = {M_INF, P_INF};

int cardAsScoreAxelMinusBirgit(card c){
	if(c.second) {
		return c.first;
	} else {
		return -c.first;
	}
}


int calculateScoreTriangle(card c1, card c2, card c3) {
	int sum = c1.first + c2.first + c3.first;
	
	if(c1.second && c2.second) {
		return sum;
	} else if(c2.second && c3.second) {
		return sum;
	} else if(c3.second && c1.second) {
		return sum;
	} else {
		return -sum;
	}
}

void availableVacantFloor(vector<pair<int, int> > &list) {
	for(int i = 0; i <= 2 ; i++) {
		for(int j = 0; j <= i; j++) {
			if(floors[i][j] == BLANK_CARD && peaks[i+1][2*j+1] != BLANK_CARD && peaks[i+1][2*j+2] != BLANK_CARD) {
				list.push_back(make_pair(i, j));
			}
		}
	}
}

int calculateScoreCoverValley(int i, int j) {
	return calculateScoreTriangle(floors[i][j], peaks[i+1][2*j+1], peaks[i+1][2*j+2]);
}

void availableVacantPeak(vector<pair<int, int> > &list) {
	for(int i = 0; i <= 2 ; i++) {
		for(int j = 0; j <= i; j++) {
			if(floors[i][j] != BLANK_CARD && peaks[i][2*j] == BLANK_CARD && peaks[i][2*j+1] == BLANK_CARD) {
				list.push_back(make_pair(i, j));
			}
		}
	}
}

int calculateScoreFormPeak(int i, int j) {
	return calculateScoreTriangle(floors[i][j], peaks[i][2*j], peaks[i][2*j+1]);
}

int selectMaxScore(int turn, int score1, int score2){
	if(turn == 0) {
		return max(score1, score2);
	} else {
		return min(score1, score2);
	}
}

void printCard(card c) {
	if(c == BLANK_CARD) {
		printf("***");
	} else {
		printf("%02d%c", c.first, c.second ? 'R' : 'B');
	}
}

void printSpaces(int n) {
	for(int i = 0; i < n; i++) {
		printf(" ");
	}
}

void printState(int turn, int score, int cursorNextDrawing) {
	cout << "===========================================================" << endl;
	cout << "Score: " << score << endl;
	cout << "turn: " << (turn == 0 ? "Axel" : "Birgit") << endl;
	cout << "A's Holding: ";
	printCard(holdingCard[0]);
	cout << endl;
	cout << "B's Holding: ";
	printCard(holdingCard[1]);
	cout << endl;
	cout << "Deck: ";
	for(int i = cursorNextDrawing; i < deck.size(); i++) {
		printCard(deck[i]);
		printf(", ");
	}
	cout << endl;
	cout << "-----------------------------------------------------------" << endl;
	
	for(int i = 0; i < 4; i++) {
		printSpaces(4*(3-i));
		for(int j = 0; j <= i; j++) {
			printCard(peaks[i][2*j]);
			printf(" ");
			printCard(peaks[i][2*j+1]);
			printf(" ");
		}
		printf("\n");
		if(i == 3)break;
		printf("    ");
		printSpaces(4*(2-i));
		for(int j = 0; j <= i; j++) {
			printf("--");
			printCard(floors[i][j]);
			printf("-- ");
		}
		printf("\n");
	}
}

void updateAlphaAndBeta(int turn, int newValue, int *alphaAndBeta) {
	if(turn == 0) {
		alphaAndBeta[0] = max(alphaAndBeta[0], newValue);
	} else {
		alphaAndBeta[1] = min(alphaAndBeta[1], newValue);
	}
}

void updateAB(int turn, int newValue, int &a, int &b) {
	if(turn == 0) {
		a = max(a, newValue);
	} else {
		b = min(b, newValue);
	}
}

int outputABCut(int turn, int a, int b) {
	if(turn == 0) {
		return b;
	} else {
		return a;
	}
}

int outputNonABCut(int turn, int a, int b) {
	if(turn == 0) {
		return a;
	} else {
		return b;
	}
}

//int turn
//A -> 0
//B -> 1
//int score 
//score is defined as Axel's score - Birgit's score
//alpha -> alphaAndBeta[0]
//beta -> alphaAndBeta[1]
int simulate(int turn, int score, int cursorNextDrawing, int a, int b) {
//	printState(turn, score, cursorNextDrawing);
	
	if(cursorNextDrawing >= deck.size()) {
//		DEBUG;
//		cout << score + cardAsScoreAxelMinusBirgit(holdingCard[0]) + cardAsScoreAxelMinusBirgit(holdingCard[1]) << endl;
		return score + cardAsScoreAxelMinusBirgit(holdingCard[0]) + cardAsScoreAxelMinusBirgit(holdingCard[1]);
	}
	
	int output = initialSimulateScore[turn];
	
	card cardDrawing = deck[cursorNextDrawing];
	
	vector<pair<int, int> > listAvailableVacantFloor;
	availableVacantFloor(listAvailableVacantFloor);
	
//	DEBUG;
	if(holdingCard[turn] == BLANK_CARD) { //holding or cover valley
		int tempOutput, nextScore;
		for(vector<pair<int, int> >::iterator it = listAvailableVacantFloor.begin(); it != listAvailableVacantFloor.end(); it++) {
//	DEBUG;
			int i = it->first;
			int j = it->second;
			floors[i][j] = cardDrawing;
			
			nextScore = score + calculateScoreCoverValley(i, j);
			tempOutput = simulate(1-turn, nextScore, cursorNextDrawing+1, a, b);
//			output = selectMaxScore(turn, output, tempOutput);
			updateAB(turn, tempOutput, a, b);
			
			floors[i][j] = BLANK_CARD;
			
			if(a >= b)return outputABCut(turn, a, b);
		}
		
		holdingCard[turn] = cardDrawing;
		tempOutput = simulate(1-turn, score, cursorNextDrawing+1, a, b);
//		output = selectMaxScore(turn, output, tempOutput);
		updateAB(turn, tempOutput, a, b);
		holdingCard[turn] = BLANK_CARD;
		
	} else {	// cover valley or form peak
		
		card cardsHaving[2][2] = {
			{holdingCard[turn], cardDrawing},
			{cardDrawing, holdingCard[turn]},
		};
		
		vector<pair<int, int> > listAvailableVacantPeak;
		availableVacantPeak(listAvailableVacantPeak);
		
		for(int x = 0; x < 2; x++) {
//	DEBUG;
			
			int tempOutput, nextScore;
			for(vector<pair<int, int> >::iterator it = listAvailableVacantFloor.begin(); it != listAvailableVacantFloor.end(); it++) {
				int i = it->first;
				int j = it->second;
				
				card temp = holdingCard[turn];
				floors[i][j] = cardsHaving[x][0];
				holdingCard[turn] = cardsHaving[x][1];
				
				nextScore = score + calculateScoreCoverValley(i, j);
				tempOutput = simulate(1-turn, nextScore, cursorNextDrawing+1, a, b);
//				output = selectMaxScore(turn, output, tempOutput);
				updateAB(turn, tempOutput, a, b);
				
				floors[i][j] = BLANK_CARD;
				holdingCard[turn] = temp;
				
				if(a >= b)return outputABCut(turn, a, b);
			}
			
			for(vector<pair<int, int> >::iterator it = listAvailableVacantPeak.begin(); it != listAvailableVacantPeak.end(); it++) {
				int i = it->first;
				int j = it->second;
				
				card temp = holdingCard[turn];
				holdingCard[turn] = BLANK_CARD;
				peaks[i][2*j]   = cardsHaving[x][0];
				peaks[i][2*j+1] = cardsHaving[x][1];
				
				nextScore = score + calculateScoreFormPeak(i, j);
				tempOutput = simulate(1-turn, nextScore, cursorNextDrawing+1, a, b);
//				output = selectMaxScore(turn, output, tempOutput);
				updateAB(turn, tempOutput, a, b);
				
				peaks[i][2*j]   = BLANK_CARD;
				peaks[i][2*j+1] = BLANK_CARD;
				holdingCard[turn] = temp;
				
				if(a >= b)return outputABCut(turn, a, b);
			}
		}
	}
	return outputNonABCut(turn, a, b);
}


/*
int cardAsScoreAxelMinusBirgit(card c){
	if(c.second) {
		return c.first;
	} else {
		return -c.first;
	}
}

void availableVacantFloor(vector<pair<int, int> > &list) {
	for(int i = 0; i <= 2 ; i++) {
		for(int j = 0; j <= i; j++) {
			if(floors[i][j] == BLANK_CARD && peaks[i+1][2*j+1] != BLANK_CARD && peaks[i+1][2*j+2] != BLANK_CARD) {
				list.push_back(make_pair(i, j));
			}
		}
	}
}

int calculateScoreAxelMinusBirgitCover(card c1, card c2, card c3) {
	int sum = c1.first + c2.first + c3.first;
	
	if(c1.second || c2.second) {
		return sum;
	} else if(c2.second || c3.second) {
		return sum;
	} else if(c3.second || c1.second) {
		return sum;
	} else {
		return -sum;
	}
}

int calculateScoreAxelMinusBirgitCoverValley(int i, int j, card c) {
	
}

int simulate(bool turnIsAxel, card cardAxelHolding, card cardBirgitHolding, int scoreAxelMinusBirgit, int cursorNextDrawing){
	if(cursorNextDrawing >= deck.size()) {
		return scoreAxelMinusBirgit + cardAsScoreAxelMinusBirgit(cardAxelHolding) + cardAsScoreAxelMinusBirgit(cardBirgitHolding);
	}
	
	card cardHolding;
	int outputScoreAxelMinusBirgit;
	if(turnIsAxel) {
		cardHolding = cardAxelHolding;
		outputScoreAxelMinusBirgit = M_INF;
	} else {
		cardHolding = cardBirgitHolding;
		outputScoreAxelMinusBirgit = P_INF;
	}
	
	card cardDrawing = deck[cursorNextDrawing];
	
	
	vector<pair<int, int> listAvailableVacantFloor;
	availableVacantFloor(listAvailableVacantFloor);
	
	if(cardHolding == BLANK_CARD) { //holding or cover valley
		for(vector<pair<int, int> >::iterator it = listAvailableVacantFloor.begin(); it != listAvailableVacantFloor.end(); it++) {
			int nextScoreAxelMinusBirgit = scoreAxelMinusBirgit + calculateScoreAxelMinusBirgitCoverValley(it->first, it->second, cardDrawing);
			int tempOutputScoreAxelMinusBirgit = simulate(!turnIsAxel, 
		}
	} else {	// cover valley or form peak
		card cardsHaving[2][2] = {
			{cardHolding, cardDrawing},
			{cardDrawing, cardHolding},
		};
		
		for(int i = 0; i < 2; i++) {
			
			
			
			int tempOutputScoreAxelMinusBirgit = 
			
		}
	}
	return outputScoreAxelMinusBirgit;
}
*/

card parseCard(string str) {
	istringstream is(str);
	
	int num;
	char c;
	is >> num >> c;
//	cout << num << endl;
	return card(num, c == 'R');
}

void init() {
	int m;
	cin >> m;
	
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			floors[i][j] = BLANK_CARD;
			peaks[i][2*j] = BLANK_CARD;
			peaks[i][2*j+1] = BLANK_CARD;
		}
	}
	holdingCard[0] = BLANK_CARD;
	holdingCard[1] = BLANK_CARD;
	deck.clear();
	
	for(int i = 0; i < 8; i++) {
		string str;
		cin >> str;
		peaks[3][i] = parseCard(str);
	}
	for(int i = 8; i < 2*m; i++) {
		string str;
		cin >> str;
		deck.push_back(parseCard(str));
	}
}

int main(void) {
	for(int caseNumber = 1; true; caseNumber++) {
		string name;
		cin >> name;
		if(name == "End")break;
		
		init();
		
		int answer;
		int alphaAndBeta[2] = {M_INF, P_INF};
		if(peaks[3][0].second) {
			answer = simulate(0, 0, 0, M_INF, P_INF);
		} else {
			answer = simulate(1, 0, 0, M_INF, P_INF);
		}
		
		if(name == "Birgit") {
			answer *= -1;
		}
		
		cout << "Case " << caseNumber << ": ";
		if(answer > 0) {
			cout << name << " wins " << answer << endl;
		} else if(answer < 0) {
			cout << name << " loses " << -answer << endl;
		} else {
			cout << "Axel and Birgit tie" << endl;
		}
		
		
		/*
		for(int i = 0; i < 2*m; i++) {
			printf("%2d %s\n", deck[i].first, deck[i].second ? "R" : "B");
		}
		*/
	}
}
