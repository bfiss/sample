#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

typedef long long int Int;

Int fat(int n) {
	Int ret = 1;
	for(int i = 1 ; i <= n ; ++i)
		ret *= i;
	return ret;
}

Int ff(int c, int b) {
	Int ret = 1;
	for(int i = 0 ; i < b ; ++i)
		ret *= c;
	return ret;
}

Int TF[110][10];

Int tf(int c, int b) {
	if(TF[c][b] != -1)
		return TF[c][b];
	if(b == 0)
		return TF[c][b] = 1;
	Int ret = 0;
	for(int i = 0 ; i < c ; ++i)
		ret += tf(c-i,b-1);
	return TF[c][b] = ret;
}

Int ft(int c, int b) {
	Int ret = 1;
	for(int i = 0 ; i < b ; ++i)
		ret *= c-i;
	return ret;
}

Int tt(int c, int b) {
	return ft(c,b)/fat(b);
}

class Score {
public:
	string name;
	Int val;
	bool operator<(Score rhs) const {
		if( val < rhs.val )
			return true;
		if( val > rhs.val )
			return false;
		return name < rhs.name;
	}
};

class Lottery {
public:
	Lottery() { for(int i = 0 ; i < 110 ; ++i) for(int j = 0 ; j < 10 ; ++j) TF[i][j] = -1; }
	vector<string> sortByOdds(vector<string> rls) {
		vector<Score> res;
		for(int i = 0 ; i < rls.size() ; ++i) {
			stringstream ss (stringstream::in | stringstream::out);
			size_t pos =  rls[i].find_first_of(':');
			Score sc;
			Int val = 0;
			sc.name = rls[i].substr(0,pos);
			//cout << "Processed: " << sc.name << endl;
			ss << rls[i].substr(pos+1);
			int c,b;
			char o,u;
			ss >> c >> b >> o >> u;
			//cout << "And the corresponding values were: " << c << " " << b << " " << o << " " << u << endl;
			if(o == 'F') {
				if(u == 'F') {
					val = ff(c,b);
				} else {
					val = ft(c,b);
				}
			} else {
				if(u == 'F') {
					val = tf(c,b);
				} else {
					val = tt(c,b);
				}
			}
			sc.val = val;
			res.push_back(sc);
		}
		sort(res.begin(),res.end());
		vector<string> ret;
		for(int i = 0 ; i < res.size() ; ++i)
			ret.push_back(res[i].name);
		return ret;
	}
};
