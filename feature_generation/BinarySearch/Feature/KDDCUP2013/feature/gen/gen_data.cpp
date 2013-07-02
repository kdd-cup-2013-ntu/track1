#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

//argv[1]: output
//argv[2]: label
//argv[3]: qid
//argv[4]: #feature
//argv[5]... : feature

int main(int agrc, char *argv[]){
	char str[1000];
	int i, j;
	FILE *flabel[2], *fqid[2], *fout[2];
	vector<FILE*> ffea[2];
	for (i=0; i<2; ++i){
		sprintf(str, "../../data/%s%d", argv[1], i);
		fout[i] = fopen(str, "w");
		sprintf(str, "../list/%s%d", argv[2], i);
		flabel[i] = fopen(str, "r");
		sprintf(str, "../list/%s%d", argv[3], i);
		fqid[i] = fopen(str, "r");
		for (j=0; j<atoi(argv[4]); ++j){
			ffea[i].push_back(new FILE);
			sprintf(str, "../list/%s%d", argv[j+5], i);
			ffea[i][j] = fopen(str, "r");
		}
	}
	for (i=0; i<2; ++i){
		while (fscanf(flabel[i], "%s", str) > 0){
			fprintf(fout[i], "%s", str);
			fscanf(fqid[i], "%s", str);
			//fprintf(fout[i], " qid:%s", str);
			for (j=0; j<atoi(argv[4]); ++j){
				fscanf(ffea[i][j], "%s", str);
				fprintf(fout[i], " %d:%s", j+1, str);
			}
			fprintf(fout[i], "\n");
		}
	}
	return 0;
}
