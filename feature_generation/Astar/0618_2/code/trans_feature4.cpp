#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<algorithm>
#include<cassert>
using namespace std;

#define MAX_A 2293837
#define MAX_P 2259021

vector<int> ap[MAX_A+10];
vector<int> pa[MAX_P+10];

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
		
		pa[pid].push_back(aid);
		ap[aid].push_back(pid);
		
	}
	fclose(fp1);
	
	printf("%d %d\n",maxa,maxp);

	fp1 = fopen("../feature/author.paperlist","w");
	assert(fp1 != NULL);
	for(int i=1; i<=MAX_A; i++){
		sort(ap[i].begin(),ap[i].end());
		fprintf(fp1,"%d",(int)ap[i].size());
		for(int j=0; j<(int)ap[i].size(); j++)
			fprintf(fp1," %d",ap[i][j]);
		fprintf(fp1,"\n");
	}
	fclose(fp1);

	fp1 = fopen("../feature/paper.authorlist","w");
	assert(fp1 != NULL);
	for(int i=1; i<=MAX_P; i++){
		sort(pa[i].begin(),pa[i].end());
		fprintf(fp1,"%d",(int)pa[i].size());
		for(int j=0; j<(int)pa[i].size(); j++)
			fprintf(fp1," %d",pa[i][j]);
		fprintf(fp1,"\n");
	}
	fclose(fp1);

	return 0;
}
