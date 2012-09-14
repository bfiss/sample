#include <iostream>

using namespace std;

int n;
unsigned x[18][132000];

void fill(int level) {
	int num = 1 << level;
	if(num>=n*2)
	    return;
	for(int in = 0 ; in < n ; in+=num) {
		int a, b, c;
		a = c = in;
		b = a + num/2;
		while( a < in + num/2 && b < in+num ) {
			if( x[level-1][b] < x[level-1][a] )
				x[level][c++] = x[level-1][b++];
			else
			    x[level][c++] = x[level-1][a++];
		}
		while( a < in + num/2 )
			x[level][c++] = x[level-1][a++];
		while( b < in + num )
			x[level][c++] = x[level-1][b++];
	}
	fill(level+1);
}

#define MAX_BIT 18
#define IDENTITY_ELEMENT -1

int find(int low, int high, int LOW, int HIGH, int toFind, int bit, int level) {
	if( low == high || bit == -1)
	    return x[level][low];
	if( !(x[level][high] & (1 << bit)) || x[level][low] & (1 << bit) )
	    return find(low,high,low,high,toFind,bit-1,level);
	if( HIGH-LOW <= 1 ) {
	    if( toFind & (1 << bit) )
	    	return find(low,LOW,low,LOW,toFind,bit-1,level);
	    return find(HIGH,high,HIGH,high,toFind,bit-1,level);
	}
	int mid = (HIGH-LOW)/2 + LOW;
	if( x[level][mid] & (1 << bit) )
	    return find(low,high,LOW,mid,toFind,bit,level);
    return find(low,high,mid,HIGH,toFind,bit,level);
}

int get(int low, int high, int LOW, int HIGH, int toFind, int level) {
	if( low > HIGH || high < LOW )
	    return IDENTITY_ELEMENT;
	if( low <= LOW && high >= HIGH ) {
		if( HIGH < n )
	    	return find(LOW,HIGH,LOW,HIGH,toFind,MAX_BIT,level);
	    return find(LOW,n-1,LOW,n-1,toFind,MAX_BIT,level);
	}
	int diff = (HIGH+1-LOW)/2;
	int a = get(low,high,LOW,LOW+diff-1,toFind,level-1);
	int b = get(low,high,LOW+diff,HIGH,toFind,level-1);
	if(a == IDENTITY_ELEMENT)
	    return b;
	if(b == IDENTITY_ELEMENT)
	    return a;
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
		    cin >> x[0][i];
		for(int i = n ; i < pot ; ++i)
		    x[0][i] = (unsigned) IDENTITY_ELEMENT;
		fill(1);
		/*cout << "TABLE\n";
		for(int i = lvl ; i >= 0 ; --i) {
			for(int j = 0 ; j < n ; ++j )
				cout << x[i][j] << " ";
			cout << endl;
		}
		cout << "END\n";*/
		//long long int sum = 0;
		while(q--) {
			int a, b, c;
			cin >> a >> b >> c;
			/*cout << "Best given " << a << " in " << b << " and " << c << " was: " << get(b-1,c-1,0,pot-1,a,lvl) << endl;
			*/cout << (a ^ get(b-1,c-1,0,pot-1,a,lvl)) << endl;
			//sum += a ^ get(b-1,c-1,0,pot-1,a,lvl);
		}
		//cout << sum << endl;
	}
	return 0;
}

