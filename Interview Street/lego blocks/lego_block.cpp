#include <iostream>
#include <cstdlib>

using namespace std;

typedef long long int numtype;

#define MOD (1000000007LL)
#define SIZE 1005
#define UNDEFI (-1LL)

numtype line[SIZE];
numtype any[SIZE][SIZE];
numtype correct[SIZE][SIZE];

numtype f_line(int m) {
	if(m<0) {
		return 0;
	} else if(line[m] + 1LL) {
	} else if(!m) {
	    line[m] = 1LL;
	} else {
		line[m] = ((f_line(m-1) + f_line(m-2)) % MOD + (f_line(m-3) + f_line(m-4)) % MOD) % MOD;
	}
	//cout << "line called with " << m << " with result " << line[m] << endl;
	return line[m];
}

numtype f_any(int n, int m) {
	if(any[n][m] + 1LL) {
	} else if(!n) {
	    any[n][m] = 1LL;
	} else if(n==1) {
	    any[n][m] = f_line(m);
	} else {
		any[n][m] = (f_any(n/2,m) * f_any(n-n/2,m)) % MOD;
	}
	//cout << "any called with " << n << " and " << m << " with result " << any[n][m] << endl;
	return any[n][m];
}

numtype f_right(int n, int m) {
	if(correct[n][m] + 1LL) {
	} else if(!m) {
		correct[n][m] = 1LL;
	} else if(m==1) {
		correct[n][m] = f_any(n,m);
	} else {
		numtype sum = 0LL;
		for(int d = 1 ; d < m ; ++d)
			sum = (sum + (f_right(n,d) * f_any(n,m-d))%MOD)%MOD;
		correct[n][m] = (f_any(n,m) + MOD - sum)%MOD;
	}
	//cout << "right called with " << n << " and " << m << " with result " << correct[n][m] << endl;
	return correct[n][m];
}

int main() {
	int t;
	std::cin >> t;
	for(int i = 0 ; i < SIZE ; ++i) {
		line[i] = UNDEFI;
	    for(int j = 0 ; j < SIZE ; ++j) {
	        any[i][j] = UNDEFI;
	        correct[i][j] = UNDEFI;
		}
	}
	while(t--) {
		int n, m;
		std::cin >> n >> m;
		std::cout << f_right(n,m) << std::endl;
		/*std::cout << "for " << n << " and " << m << " the calculation was:\n";
		std::cout << "Line: ";
		for(int i = 0 ; i <= m ; ++i)
			std::cout << line[i] << " ";
		std::cout << "\nAny:\n";
		for(int i = 0 ; i <= n ; ++i) {
		    for(int j = 0 ; j <= m ; ++j)
		        std::cout << any[i][j] << " ";
			std::cout << std::endl;
		}
		std::cout << "Right:\n";
		for(int i = 0 ; i <= n ; ++i) {
		    for(int j = 0 ; j <= m ; ++j)
		        std::cout << correct[i][j] << " ";
			std::cout << std::endl;
		}
		std::cout << "\n\n";*/
	}
	return 0;
}

