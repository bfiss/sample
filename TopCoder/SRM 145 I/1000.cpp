#include <iostream>
#include <vector>

using namespace std;

class HillHike {
public:
	long long dp[110][60][60][2];
	long long numPaths(int distance, int maxHeight, vector <int> landmarks) {
		for(int i = 0 ; i < 110 ; ++i)
			for(int j = 0 ; j < 60 ; ++j)
				for(int z = 0 ; z < 60 ; ++z)
					for(int k = 0 ; k < 2 ; ++k)
						dp[i][j][z][k] = 0;
		dp[0][0][0][0] = 1;
		for(int i = 1 ; i < distance ; ++i) {
			for(int j = 1 ; j <= maxHeight ; ++j) {
				for(int z = 0 ; z <= landmarks.size() ; ++z) {
					int min = 0, max = 0;
					if(z < landmarks.size() && j == landmarks[z])
						min = 1;
					if(z > 0 && j == landmarks[z-1])
						max = 1;
					for(int diff = min ; diff <= max ; ++diff) {
						for(int k = 0 ; k < 2 ; ++k) {
							dp[i][j][z][k] += dp[i-1][j+1][z-diff][k];
							dp[i][j][z][k] += dp[i-1][j][z-diff][k];
							if(j == maxHeight && k == 1) {
								dp[i][j][z][k] += dp[i-1][j-1][z-diff][0];
								dp[i][j][z][k] += dp[i-1][j-1][z-diff][1];
							} else if(j < maxHeight) {
								dp[i][j][z][k] += dp[i-1][j-1][z-diff][k];
							}
						}
					}
					//cout << "dp[" << i << "][" << j << "][" << z << "][" << 0 << "] = " << dp[i][j][z][0] << endl;
					//cout << "dp[" << i << "][" << j << "][" << z << "][" << 1 << "] = " << dp[i][j][z][1] << endl;
				}
			}
		}
		return dp[distance-1][1][landmarks.size()][1];
	}
};
