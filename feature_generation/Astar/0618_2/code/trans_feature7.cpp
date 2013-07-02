#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<set>
#include<cassert>
using namespace std;

#define MAX_A 2293837
#define MAX_P 2259021
#define MAX_C 5222
#define MAX_J 22228

vector<int> pa[MAX_P+10];
vector<int> ap[MAX_A+10];
set<int> aco[MAX_A+10];

void add(int pid,int aid){
	for(int i=0; i<(int)pa[pid].size(); i++){
		int co = pa[pid][i];
		aco[aid].insert(co);
	}

}


int main(int argc, char *argv[]){

	FILE *fp1;

	int nnum;
	int count;

	printf("read ../feature/author.paperlist\n");
	fp1 = fopen("../feature/author.paperlist","r");
	assert(fp1!=NULL);

	count = 1;
	while(fscanf(fp1,"%d",&nnum)!=EOF){
		for(int i=0; i<nnum; i++){
			int tmp;
			fscanf(fp1,"%d",&tmp);
			ap[count].push_back(tmp);
		}
		count++;
	}
	fclose(fp1);

	printf("read ../feature/paper.authorlist\n");
	fp1 = fopen("../feature/paper.authorlist","r");
	assert(fp1!=NULL);

	count = 1;
	while(fscanf(fp1,"%d",&nnum)!=EOF){
		for(int i=0; i<nnum; i++){
			int tmp;
			fscanf(fp1,"%d",&tmp);
			pa[count].push_back(tmp);
		}
		count++;
	}
	fclose(fp1);

	printf("start...\n");

	for(int i=1; i<=MAX_A; i++){
		if(i%10000==0)
			printf("iter %d\n",i/10000);
		for(int j=0; j<(int)ap[i].size(); j++){
			add(ap[i][j],i);
		}
	}
	
	fp1 = fopen("../feature/author.coauthornum","w");
	for(int i=1; i<=MAX_A; i++){
		fprintf(fp1,"%d\n",(int)aco[i].size());
	}
	fclose(fp1);

	fp1 = fopen("../feature/author.coauthorlist","w");
	for(int i=1; i<=MAX_A; i++){
		std::set<int>::iterator it;
		fprintf(fp1,"%d",(int)aco[i].size());
		for(it=aco[i].begin(); it!=aco[i].end(); it++)
			fprintf(fp1," %d",*it);
		fprintf(fp1,"\n");
	}
	fclose(fp1);

	return 0;
}
