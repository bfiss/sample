#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class VendingMachine {
public:
	int curr;
	int shelves;
	int columns;
	int price[60][60];
	bool valid[60][60];
	int colPrice[60];
	int motorSeconds;
	void move(int column) {
		int v1 = column - curr;
		int v2 = curr - column;
		if(v1 < 0) v1 += columns;
		if(v2 < 0) v2 += columns;
		curr = column;
		int cost = (v1 > v2 ? v2 : v1);
		//cout << "cost for moving is " << cost << endl;
		motorSeconds += cost;
	} 
	void moveBest() {
		int col = 0, biggest = 0;
		for(int i = 0 ; i < columns ; ++i) {
			if(biggest < colPrice[i]) {
				biggest = colPrice[i];
				col = i;
			}
		}
		//cout << "moving best: best column is " << col << endl;
		move(col);
	}
	int motorUse(vector <string> prices, vector <string> purchases) {
		motorSeconds = 0;
		curr = 0;
		shelves = prices.size();
		stringstream ss;
		ss << prices[0];
		columns = 1;
		for( ; ; ++columns) {
			int tmp = 0;
			ss >> tmp;
			if(ss.eof())
				break;
		}
		//cout << "shelves is " << shelves << " and columns is " << columns << endl;
		for(int i = 0 ; i < columns ; ++i)
			colPrice[i] = 0;
		for(int i = 0 ; i < shelves ; ++i) {
			stringstream ss2;
			ss2 << prices[i];
			for(int j = 0 ; j < columns ; ++j) {
				int currPrice;
				ss2 >> currPrice;
				price[i][j] = currPrice;
				colPrice[j] += currPrice;
				valid[i][j] = true;
			}
		}
		int lastT = -10000;
		//cout << "got here" << endl;
		for(int i = 0 ; i < purchases.size() ; ++i) {
			int s,c,t;
			stringstream ss;
			purchases[i][purchases[i].find_first_of(',')] = ' ';
			purchases[i][purchases[i].find_first_of(':')] = ' ';
			ss << purchases[i];
			ss >> s >> c >> t;
			if(valid[s][c])
				valid[s][c] = false;
			else
				return -1;
			//cout << "purchase at time " << t << " and the last one was at time " << lastT << endl;
			if(t >= lastT + 5) {
				moveBest();
			}
			lastT = t;
			move(c);
			colPrice[c] -= price[s][c];
			price[s][c] = 0;
		}
		moveBest();
		return motorSeconds;
	}
};