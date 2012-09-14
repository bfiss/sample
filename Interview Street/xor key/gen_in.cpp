#include <cstdio>
#include <cstdlib>
#include <ctime>

int main() {
	srand(time(NULL));
	FILE * saida = fopen("inp.txt","w");
	fprintf(saida,"6\n");
	for(int t = 0 ; t < 6 ; ++t) {
		fprintf(saida,"100000 50000\n");
		for(int i = 0 ; i < 100000 ; ++i) {
			fprintf(saida,"%d ",rand()%(1<<15));
		}
		fprintf(saida,"\n");
		for(int i = 0 ; i < 50000 ; ++i) {
			int a = rand()%100000 + 1;
			int b = rand()%100000 + 1;
			if(b<a) {
				int tmp = b;
				b = a;
				a = tmp;
			}
			fprintf(saida,"%d %d %d\n",rand()%(1<<15),a,b);
		}
	}
	fclose(saida);
	return 0;
}
