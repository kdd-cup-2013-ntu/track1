#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cassert>
using namespace std;

#define MAX_A 2293837
#define MAX_P 2259021

int numA[MAX_A+10];
int numP[MAX_P+10];

int main(int argc, char *argv[]){
	

	FILE *fp1;
	fp1 = fopen("../mydata/PaperAuthor.my","r");
	assert(fp1!=NULL);

	int maxa = 0;
	int maxp = 0;
	
	int aid;
	int pid;
	while(fscanf(fp1,"%d%d",&pid,&aid)!=EOF){

		maxa = (maxa>aid) ? maxa:aid;
		maxp = (maxp>pid) ? maxp:pid;

		//printf("%d %d\n",pid,aid);
		
		
		numA[aid]++;
		numP[pid]++;
	}
	fclose(fp1);
	
	printf("%d %d\n",maxa,maxp);

	fp1 = fopen("../feature/author.papernum","w");
	assert(fp1 != NULL);
	for(int i=1; i<=MAX_A; i++)
		fprintf(fp1,"%d\n",numA[i]);
	fclose(fp1);

	fp1 = fopen("../feature/paper.authornum","w");
	assert(fp1 != NULL);
	for(int i=1; i<=MAX_P; i++)
		fprintf(fp1,"%d\n",numP[i]);
	fclose(fp1);

	return 0;
}
