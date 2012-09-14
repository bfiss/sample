#include <cstdio>
#include <iostream>

using namespace std;

int n;
unsigned x[18][100500];

void fill() {
	int num = 1;
	for(int level = 1 ; num < n ; ++level ) {
		//cout << "level:" << level << endl;
		num *= 2;
		for(int in = 0 ; in < n ; in+=num) {
			int a, b, c;
			a = c = in;
			b = a + num/2;
			while( a < in + num/2 && b < in+num && b < n && a < n) {
				if( x[level-1][b] < x[level-1][a] )
					x[level][c++] = x[level-1][b++];
				else
					x[level][c++] = x[level-1][a++];
			}
			while( a < in + num/2 && a < n )
				x[level][c++] = x[level-1][a++];
			while( b < in + num  && b < n )
				x[level][c++] = x[level-1][b++];
		}
	}
}

#define MAX_BIT 18
#define IDENTITY_ELEMENT -1

int toFind;

int find(int low, int high, int level) {
	int bit = MAX_BIT;
	while( high > low && bit >= 0 ) {
		int setBit = (1 << bit);
		if( !(x[level][high] & setBit) || x[level][low] & setBit ) {
			--bit;
			continue;
		}
		int LOW = low, HIGH = high;
		while( HIGH-LOW > 1 ) {
			int mid = (HIGH-LOW)/2 + LOW;
			if( x[level][mid] & setBit )
				HIGH = mid;
			else
				LOW = mid;
		}
		if( toFind & setBit )
			high = LOW;
		else
			low = HIGH;
		--bit;
    }
	return x[level][low];
}

int low, high;

int get(int LOW, int HIGH, int level) {
	if( low <= LOW && high >= HIGH ) {
		if( HIGH < n )
	    	return find(LOW,HIGH,level);
	    return find(LOW,n-1,level);
	}
	int diff = (HIGH+1-LOW)/2;
	if( LOW+diff-1 < low )
		return get(LOW+diff,HIGH,level-1);
	if( LOW+diff > high )
		return get(LOW,LOW+diff-1,level-1);
	int a = get(LOW,LOW+diff-1,level-1);
	int b = get(LOW+diff,HIGH,level-1);
	return (a^toFind)<(b^toFind)?b:a;
}

int main() {
	int t;
	cin >> t;
	while (t--) {
		int q, pot, lvl;
		cin >> n >> q;
		pot = 1;
		lvl = 0;
		while(pot<n) {
			pot <<= 1;
			lvl++;
		}
		for(int i = 0 ; i < n ; ++i)
		    scanf("%d",&x[0][i]);
		fill();
		/*cout << "TABLE\n";
		for(int i = lvl ; i >= 0 ; --i) {
			for(int j = 0 ; j < n ; ++j )
				cout << x[i][j] << " ";
			cout << endl;
		}
		cout << "END\n";*/
		//long long int sum = 0;
		while(q--) {
			scanf("%d%d%d",&toFind,&low,&high);
			--low;
			--high;
			/*cout << "Best given " << toFind << " in " << low+1 << " and "
			       << high+1 << " was: " << get(0,pot-1,lvl) << endl;*/
			printf("%d\n",toFind ^ get(0,pot-1,lvl));
			//sum += toFind ^ get(0,pot-1,lvl);
		}
		//cout << sum << endl;
	}
	return 0;
}
