#include <iostream>
#include <iomanip>

using namespace std;

int n;
int p[55];
int inv[55];

int main(void) {
	int t;
	cout << fixed << setprecision(2);
	cin >> t;
	while(t--) {
		cin >> n;
		for(unsigned i=0;i<n;++i)
		    cin >> p[i];
		for(unsigned i=0;i<n;++i) {
			inv[i] = 0;
			for(unsigned j=0;j<n;++j)
				if(p[i]>p[j])
				    ++inv[i];
		}
		double average = 0.0;
		for(unsigned i=0;i<n;++i) {
			double chance = 1.0;
			for(unsigned pos=0 ; pos < n ; ++pos) {
			    average += ((double)n-(double)pos)*chance/(double)n;
			    if(inv[i]==pos)
					break;
			    chance *= ((double)inv[i]-(double)pos)/((double)n-(double)pos-1.0);
			}
		}
		cout << average << endl;
	}
	return 0;
}
