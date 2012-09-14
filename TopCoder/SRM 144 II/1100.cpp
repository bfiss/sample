#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class PowerOutage {
public:
	vector <int> fromJunction;
	vector <int> toJunction;
	vector <int> ductLength;
	pair<int,int> pd[100];
	pair<int,int> calculateSize(int junction) {
		if(pd[junction].first != -1) {
			//cout << "entered this with junction " << junction << endl;
			return pd[junction];
		}
		int ans = 0;
		int max_cost = 0;
		for(int i = 0 ; i < fromJunction.size() ; ++i) {
			if(fromJunction[i] == junction) {
				pair<int,int> p = calculateSize(toJunction[i]);
				int cost = ductLength[i]*2 + p.first;
				if(max_cost < p.second + ductLength[i])
					max_cost = p.second + ductLength[i];
				ans += cost;
			}
		}
		//cout << "junction " << junction << " is returning a cost of " << ans << " and max_path of " << max_cost << endl;
		return pd[junction] = make_pair(ans,max_cost);
	}
	int estimateTimeOut(vector <int> _fromJunction, vector <int> _toJunction, vector <int> _ductLength) {
		fromJunction = _fromJunction;
		toJunction = _toJunction;
		ductLength = _ductLength;
		for(int i = 0 ; i < 100 ; ++i) pd[i] = make_pair(-1,1);
		pair<int,int> p = calculateSize(0);
		return p.first - p.second;
	}
};
