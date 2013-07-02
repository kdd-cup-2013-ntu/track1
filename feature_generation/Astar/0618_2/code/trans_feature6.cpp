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

int ays[MAX_A+10];
int aye[MAX_A+10];
int see[MAX_A+10];
int py[MAX_P+10];

int main(int argc, char *argv[]){
	

	FILE *fp1;
	printf("read PaperYear.my\n");
	fp1 = fopen("../mydata/PaperYear.my","r");
	assert(fp1!=NULL);

	int pid;
	int aid;
	int year;

	while(fscanf(fp1,"%d%d",&pid,&year)!=EOF){
		py[pid] = year;
	}
	fclose(fp1);

	printf("read PaperAuthor.my\n");
	fp1 = fopen("../mydata/PaperAuthor.my","r");
	assert(fp1!=NULL);
	while(fscanf(fp1,"%d%d",&pid,&aid)!=EOF){
		if(py[pid]==0)
			continue;

		if(!see[aid]){
			ays[aid] = py[pid];
			aye[aid] = py[pid];
		}
		else{
			ays[aid] = ays[aid]<py[pid] ? ays[aid]:py[pid];
			aye[aid] = aye[aid]>py[pid] ? aye[aid]:py[pid];
		}
		see[aid] = 1;
	}
	fclose(fp1);

	printf("write...\n");
	
	fp1 = fopen("../feature/paper.year","w");
	for(int i=1; i<=MAX_P; i++)
		fprintf(fp1,"%d\n",py[i]);
	fclose(fp1);

	fp1 = fopen("../feature/author.yearstart","w");
	for(int i=1; i<=MAX_A; i++)
		fprintf(fp1,"%d\n",ays[i]);
	fclose(fp1);

	fp1 = fopen("../feature/author.yearend","w");
	for(int i=1; i<=MAX_A; i++)
		fprintf(fp1,"%d\n",aye[i]);
	fclose(fp1);


	return 0;
}
