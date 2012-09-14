#include <iostream>
#include <vector>

using namespace std;

class BinaryCode {
public:
	int toInt(char c) { return c - '0'; }
	char toChar(int i) { return i + '0'; }
	string assuming(int assumedValue, string message) {
		string ans;
		bool valid = true;
		int p = 0;
		int c = assumedValue;
		ans.push_back(toChar(c));
		for(int i = 0 ; i < message.size() ; ++i) {
			int n = toInt(message[i]) - p - c;
			if( (n<0 || n >1) || (i == message.size()-1 && n) ) {
				valid = false;
				break;
			}
			p = c;
			c = n;
			if(i!=message.size()-1)
				ans.push_back(toChar(c));
		}
		if(valid)
			return ans;
		ans.clear();
		ans += "NONE";
		return ans;
	}
	vector<string> decode(string message) {
		vector<string> ans;
		ans.push_back(assuming(0,message));
		ans.push_back(assuming(1,message));
		return ans;
	}
};