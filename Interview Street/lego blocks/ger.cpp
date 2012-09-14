#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

int main() {
	FILE * s = fopen("i.txt","w");
	srand(time(NULL));
	fprintf(s,"100\n");
	for(int i = 0 ; i < 100 ; ++i)
		fprintf(s,"%d %d\n",1000-i,1000);
	fclose(s);
	return 0;
}
