#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char **argv){
	FILE *fin, *fout;
	char str[100];
	if (argc != 2){
		cerr << "error!" << endl;
		return -1;
	}
	fin = fopen(argv[1], "r");
	sprintf(str, "%s.tmp", argv[1]);
	fout = fopen(str, "w");
	while (fscanf(fin, "%s", str) > 0){
		if (strcmp(str,"-nan")==0 || strcmp(str,"nan")==0)
			fprintf(fout,"0\n");
		else
			fprintf(fout,"%s\n", str);
	}
	return 0;
}
