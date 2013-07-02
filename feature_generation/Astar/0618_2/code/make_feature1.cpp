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


#define KA 4
#define KP 2

char namea[][1000] = {
"../feature/author.papernum",
"../feature/author.yearstart",
"../feature/author.yearend",
"../feature/author.coauthornum",
};

char namep[][1000] = {
"../feature/paper.authornum",
"../feature/paper.year"
};

int fa[MAX_A+10][KA+10];
int fp[MAX_P+10][KP+10];

int pc[MAX_P+10];
int pj[MAX_P+10];

vector<int> ac[MAX_A+10];
vector<int> aj[MAX_A+10];

vector<int> ap[MAX_A+10];
vector<int> pa[MAX_P+10];

int calc(int aid,int cid){
	int ans = 0;
	for(int i=0; i<(int)ac[aid].size(); i++){
		if(ac[aid][i]==cid)
			ans++;
	}
	return ans;
}

int calj(int aid,int jid){
	int ans = 0;
	for(int i=0; i<(int)aj[aid].size(); i++){
		if(aj[aid][i]==jid)
			ans++;
	}
	return ans;
}

int check_in(int pid,int aid){
	for(int i=0; i<(int)ap[aid].size(); i++){
		if(pid==ap[aid][i])
			return 1;
	}
	return 0;
}

int numco;
int numcop;
int totcop;
void calco(int aid,int pid){
	numco = 0;
	numcop = 0;
	totcop = 0;
	for(int i=0; i<(int)pa[pid].size(); i++){
		int co = pa[pid][i];

		
		if(co==aid)
			continue;
		
		//if(pid == 156528)
		//{
		//	printf("aid = %d, pid = %d, co = %d\n",aid,pid,co);
		//}

		int flag = 0;
		for(int j=0; j<(int)ap[co].size(); j++){
			int cop = ap[co][j];
			//if(pid == 156528)
			//	printf("aid = %d, pid = %d, cop = %d\n",aid,pid,cop);
			if(cop==pid)
				continue;
			if(check_in(cop,aid)){
				flag = 1;
				numcop++;
			}
			totcop++;
		}
		if(flag)
			numco++;
	}
	//if(aid == 791249)
	//{
	//	printf("aid = %d, pid = %d, numcop = %d, totcop = %d\n",aid,pid,numcop,totcop);
	//}
}

int main(int argc,char *argv[]){

	if(argc<5){
		printf("- argc not enough!\n");
		printf("- main train_file output_train_file valid_file output_valid_file \n");
		exit(0);
	}

	char *trainName = argv[1];
	char *trainOut = argv[2];
	char *validName = argv[3];
	char *validOut = argv[4];
	
	FILE *fp1;
	FILE *fp2;

	
	for(int i=0; i<KA; i++){
		printf("read %s...\n",namea[i]);
		fp1 = fopen(namea[i],"r");
		assert(fp1!=NULL);

		for(int j=0; j<MAX_A; j++)
			fscanf(fp1,"%d",&fa[j+1][i]);

		fclose(fp1);
	}


	for(int i=0; i<KP; i++){
		printf("read %s...\n",namep[i]);
		fp1 = fopen(namep[i],"r");
		assert(fp1!=NULL);

		for(int j=0; j<MAX_A; j++)
			fscanf(fp1,"%d",&fp[j+1][i]);

		fclose(fp1);
	}


	printf("read Paper.my.. \n");
	fp1 = fopen("../mydata/Paper.my","r");
	assert(fp1!=NULL);
	int ppid;
	int ccid;
	int jjid;
	while(fscanf(fp1,"%d%d%d",&ppid,&ccid,&jjid)!=EOF){
		pc[ppid] = ccid;
		pj[ppid] = jjid;
	}
	fclose(fp1);

	int nnum;
	int cnum = 0;

	printf("read conference list...\n");
	cnum = 0;
	fp1 = fopen("../feature/author.conferencelist","r");
	assert(fp1!=NULL);
	while(fscanf(fp1,"%d",&nnum)!=EOF){
		int tmp;
		for(int i=0; i<nnum; i++){
			fscanf(fp1,"%d",&tmp);
			ac[cnum+1].push_back(tmp);
		}
		cnum++;
	}
	fclose(fp1);

	printf("read journal list...\n");
	cnum = 0;
	fp1 = fopen("../feature/author.journallist","r");
	assert(fp1!=NULL);
	while(fscanf(fp1,"%d",&nnum)!=EOF){
		int tmp;
		for(int i=0; i<nnum; i++){
			fscanf(fp1,"%d",&tmp);
			aj[cnum+1].push_back(tmp);
		}
		cnum++;
	}
	fclose(fp1);

	printf("read author paper list...\n");
	cnum = 0;
	fp1 = fopen("../feature/author.paperlist","r");
	assert(fp1!=NULL);
	while(fscanf(fp1,"%d",&nnum)!=EOF){
		int tmp;
		for(int i=0; i<nnum; i++){
			fscanf(fp1,"%d",&tmp);
			ap[cnum+1].push_back(tmp);
		}
		cnum++;
	}
	fclose(fp1);

	printf("read paper author list...\n");
	cnum = 0;
	fp1 = fopen("../feature/paper.authorlist","r");
	assert(fp1!=NULL);
	while(fscanf(fp1,"%d",&nnum)!=EOF){
		int tmp;
		for(int i=0; i<nnum; i++){
			fscanf(fp1,"%d",&tmp);
			pa[cnum+1].push_back(tmp);
		}
		cnum++;
	}
	fclose(fp1);

	printf("making train feature...\n");

	fp1 = fopen(trainName,"r");
	assert(fp1!=NULL);
	fp2 = fopen(trainOut,"w");

	char tmp[100000];
	fgets(tmp,100000,fp1);

	int count = 0;
	while(fgets(tmp,100000,fp1)!=NULL){
		
		if(count%100==0)
			printf("iter %d\n",count/100);
		
		count++;

		char *ptr;
		int aid;
		int pid;
		char conf[100000];
		char dele[100000];
		ptr = strtok(tmp,",\x0d\n");
		aid = atoi(ptr);
		ptr = strtok(NULL,",\x0d\n");
		strcpy(conf,ptr);
		ptr = strtok(NULL,",\x0d\n");
		strcpy(dele,ptr);
		
		//printf("%d\n%s\n%s\n",aid,conf,dele);

		ptr = strtok(conf," ");
		while(ptr!=NULL){
			pid = atoi(ptr);
			
			//fprintf(fp2,"%d %d, ",aid,pid);

			//fprintf(fp2,"%d %d ",aid,pid);
			//fprintf(fp2,"1");
			/*for(int i=0; i<KA; i++)
				fprintf(fp2," %d",fa[aid][i]);
			for(int i=0; i<KP; i++)
				fprintf(fp2," %d",fp[pid][i]);*/
			
			//int cid = pc[pid];
			//int jid = pj[pid];
			//fprintf(fp2," %d",calc(aid,cid));
			//fprintf(fp2," %d",calj(aid,jid));
			//calc(aid,cid);
			//calj(aid,jid);
			calco(aid,pid);
			//fprintf(fp2," %d",numco);
			fprintf(fp2,"%d",numcop);
			if(totcop != 0)
				fprintf(fp2," %lf",(double)numcop/(double)totcop);
			else
				fprintf(fp2," 0.0");
			//fprintf(fp2," %lf",(double)numco/fp[pid][0]);

			fprintf(fp2,"\n");


			ptr = strtok(NULL," ");
		}

		ptr = strtok(dele," ");
		while(ptr!=NULL){
			pid = atoi(ptr);
			
			//fprintf(fp2,"%d %d, ",aid,pid);

			//fprintf(fp2,"%d %d ",aid,pid);
			//fprintf(fp2,"0");
			/*for(int i=0; i<KA; i++)
				fprintf(fp2," %d",fa[aid][i]);
			for(int i=0; i<KP; i++)
				fprintf(fp2," %d",fp[pid][i]);*/

			//int cid = pc[pid];
			//int jid = pj[pid];
			//fprintf(fp2," %d",calc(aid,cid));
			//fprintf(fp2," %d",calj(aid,jid));
			//calc(aid,cid);
			//calj(aid,jid);
			calco(aid,pid);
			//fprintf(fp2," %d",numco);
			fprintf(fp2,"%d",numcop);
			if(totcop != 0)
				fprintf(fp2," %lf",(double)numcop/(double)totcop);
			else
				fprintf(fp2," 0.0");
			//fprintf(fp2," %lf",(double)numco/fp[pid][0]);

			fprintf(fp2,"\n");

			ptr = strtok(NULL," ");
		}

	}
	printf("count = %d\n",count);

	fclose(fp1);
	fclose(fp2);

	printf("making valid feature...\n");

	fp1 = fopen(validName,"r");
	assert(fp1!=NULL);
	fp2 = fopen(validOut,"w");

	fgets(tmp,100000,fp1);

	count = 0;
	while(fgets(tmp,100000,fp1)!=NULL){
		
		if(count%100==0)
			printf("iter %d\n",count/100);
		
		count++;

		char *ptr;
		int aid;
		int pid;
		char conf[100000];
		ptr = strtok(tmp,",\x0d\n");
		aid = atoi(ptr);
		ptr = strtok(NULL,",\x0d\n");
		strcpy(conf,ptr);
		
		//printf("%d\n%s\n%s\n",aid,conf,dele);

		ptr = strtok(conf," ");
		while(ptr!=NULL){
			pid = atoi(ptr);
			
			//fprintf(fp2,"%d %d, ",aid,pid);

			//fprintf(fp2,"%d %d ",aid,pid);
			//fprintf(fp2,"1");
			/*for(int i=0; i<KA; i++)
				fprintf(fp2," %d",fa[aid][i]);
			for(int i=0; i<KP; i++)
				fprintf(fp2," %d",fp[pid][i]);*/
			
			//int cid = pc[pid];
			//int jid = pj[pid];
			//fprintf(fp2," %d",calc(aid,cid));
			//fprintf(fp2," %d",calj(aid,jid));
			//calc(aid,cid);
			//calj(aid,jid);
			calco(aid,pid);
			//fprintf(fp2," %d",numco);
			fprintf(fp2,"%d",numcop);
			
			if(totcop != 0)
				fprintf(fp2," %lf",(double)numcop/(double)totcop);
			else
				fprintf(fp2," 0.0");
			//fprintf(fp2," %lf",(double)numco/fp[pid][0]);

			fprintf(fp2,"\n");


			ptr = strtok(NULL," ");
		}

	}

	printf("count = %d\n",count);

	fclose(fp1);
	fclose(fp2);

	return 0;
}
