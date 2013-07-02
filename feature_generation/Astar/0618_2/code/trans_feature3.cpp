#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<cassert>
using namespace std;

#define MAX_A 2293837
#define MAX_P 2259021

vector<int> cc[MAX_A+10];
vector<int> jj[MAX_A+10];

int pc[MAX_P+10];
int pj[MAX_P+10];

int main(int argc, char *argv[]){
	

	FILE *fp1;
	FILE *fp2;
	fp1 = fopen("../mydata/PaperAuthor.my","r");
	fp2 = fopen("../mydata/Paper.my","r");
	assert(fp1!=NULL);
	assert(fp2!=NULL);

	int maxa = 0;
	int maxp = 0;
	
	int aid;
	int pid;

	int cid;
	int jid;
	
	int maxc = 0;
	int maxj = 0;

	while(fscanf(fp2,"%d%d%d",&pid,&cid,&jid)!=EOF){

		maxc = (maxc>cid) ? maxc:cid;
		maxj = (maxj>jid) ? maxj:jid;
		
		pc[pid] = cid;
		pj[pid] = jid;
	}
	fclose(fp2);

	printf("%d %d\n",maxc,maxj);
	
	printf("------\n");

	while(fscanf(fp1,"%d%d",&pid,&aid)!=EOF){

		maxa = (maxa>aid) ? maxa:aid;
		maxp = (maxp>pid) ? maxp:pid;

		//printf("%d %d\n",pid,aid);
		
		cc[aid].push_back(pc[pid]);
		jj[aid].push_back(pj[pid]);
	}
	fclose(fp1);
	
	printf("%d %d\n",maxa,maxp);

	fp1 = fopen("../feature/author.conferencelist","w");
	assert(fp1 != NULL);
	for(int i=1; i<=MAX_A; i++){
		fprintf(fp1,"%d",(int)cc[i].size());
		for(int j=0; j<(int)cc[i].size(); j++)
			fprintf(fp1," %d",cc[i][j]);
		fprintf(fp1,"\n");
	}
	fclose(fp1);

	fp1 = fopen("../feature/author.journallist","w");
	assert(fp1 != NULL);
	for(int i=1; i<=MAX_A; i++){
		fprintf(fp1,"%d",(int)jj[i].size());
		for(int j=0; j<(int)jj[i].size(); j++)
			fprintf(fp1," %d",jj[i][j]);
		fprintf(fp1,"\n");
	}
	fclose(fp1);

	return 0;
}
