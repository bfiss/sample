#include <iostream>

using namespace std;

#define MAX_P 80000
#define MOD 1000007

long long int prime[MAX_P];
long long int fac[MAX_P];

int main() {
	long long int n;
	cin >> n;
	/* find primes */
	int num_p = 0;
	for(int i = 2 ; i <= n ; ++i) {
        int j = 2;
		for( ; j*j <= i ; ++j)
			if(!(i%j))
			    break;
		if(j*j > i)
			prime[num_p++] = i;
	}
	
	/* factorize n! */
	for(int i = 0 ; i < num_p ; ++i) {
		long long int sum = 0;
		for(long long int j = prime[i]; n/j ; j *= prime[i])
			sum += n/j;
		fac[i] = sum;
	}
	
 	/* print factorization
	for(int i = 0 ; i < num_p ; ++i)
	    cout << prime[i] << "^" << fac[i] << " ";
	cout << endl;*/
	
	long long int ways = 1;
	for(int i = 0 ; i < num_p ; ++i)
		ways = (ways * (2*fac[i] + 1)) % MOD;
	
	cout << ways << endl;
	return 0;
}
