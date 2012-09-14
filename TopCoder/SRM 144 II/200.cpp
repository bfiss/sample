#include <iostream>
#include <sstream>

using namespace std;

class Time {
public:
	string whatTime(int seconds) {
		string ans;
		stringstream ss(stringstream::in | stringstream::out);
		ss << (seconds/3600) << ":" << (seconds%3600)/60 << ":" << seconds%60 << endl;
		ss >> ans;
		return ans;
	}
};
