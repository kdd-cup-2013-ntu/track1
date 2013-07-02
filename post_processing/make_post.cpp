/************************************************************************
 *																		*
 * This code use test_file to generate two file for post-processing.	*
 * Valid.id contains the authorId and paperId in test_file.				*
 * Valid.num contains the number of paper for each authorId.			*
 * Both Valid.id and Valid.num will be used in post_processing.			*
 *																		*
 ************************************************************************/

#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<cassert>
using namespace std;

#define MAX_A 2293837
#define MAX_P 2259021
#define MAX_C 5222
#define MAX_J 22228


int main(int argc,char *argv[]){

	if(argc<2){
		printf("- argc not enough!\n");
		printf("- main test_file\n");
		exit(0);
	}

	FILE *fp1 = fopen(argv[1],"r");
	assert(fp1!=NULL);
	FILE *fp2 = fopen("Valid.id","w");
	FILE *fp3 = fopen("Valid.num","w");

	char tmp[100000];
	fgets(tmp,100000,fp1);
	int count = 0;

	// read test_file
	while(fgets(tmp,100000,fp1)!=NULL){
		count++;

		char *ptr;
		int aid;
		int pid;
		char conf[100000];
		ptr = strtok(tmp,",\x0d\n");
		aid = atoi(ptr);
		ptr = strtok(NULL,",\x0d\n");
		strcpy(conf,ptr);

		// write authorId in Valid.id
		fprintf(fp2,"%d\n",aid);
		
		int ans = 0;
		ptr = strtok(conf," ");
		while(ptr!=NULL){
			pid = atoi(ptr);
			// write paperId in Valid.id
			fprintf(fp2,"%d\n",pid);
			ans++;
			ptr = strtok(NULL," ");
		}

		// write the number of paper in Valid.num
		fprintf(fp3,"%d\n",ans);

	}

	printf("count = %d\n",count);

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);

	return 0;
}
