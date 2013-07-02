/********************************************************************
 *																	*
 * 	This code use Valid.id, Valid.num, Valid.dup and predirct file	*
 *  generate the final csv_file. We use the value in Valud.dup to	*
 *  decide the corresponding paperId is the first rank or the		*
 *  second rank. If the value greater than 1, the corresponding		*
 *  paperId will have first rank Otherwise, the corresponding		*
 *  paperId will have second tank. The final rank is the sorted		*
 *  first rank concatenating the sorted second rank.				*
 *																	*
 ********************************************************************/


#include<cstdio>
#include<cstdlib>
#include<algorithm>
#include<vector>
#include<set>
using namespace std;

struct Data{
	int y;
	double v;
};

int cmp(Data a,Data b){
	return a.v>b.v;
}


int main(int argc, char *argv[]){
	if(argc<3){
		printf("- usage: main predict_file output_file\n");
		exit(0);
	}

	FILE *fp1 = fopen("Valid.num","r");
	FILE *fp2 = fopen(argv[1],"r");
	FILE *fp3 = fopen("Valid.id","r");
	FILE *fp4 = fopen(argv[2],"w");
	FILE *fp5 = fopen("Valid.dup","r");

	fprintf(fp4,"AuthorId,PaperIds\n");
	
	int p;

	int cc = 0;

	while(fscanf(fp1,"%d",&p)!=EOF){
		int aid;
		fscanf(fp3,"%d",&aid);

		//printf("%d\n",p);
		
		vector<Data> vec1; // save first rank
		vector<Data> vec2; // save second rank
		vector<Data> rec;  // save duplicated paperId
		set<int> rep;
		for(int i=0; i<p; i++){
			double v;
			int pid;
			double dup;
			fscanf(fp2,"%lf",&v);
			fscanf(fp3,"%d",&pid);
			fscanf(fp5,"%lf",&dup);
			
			Data t = {pid,v};
			if(rep.find(pid)==rep.end()){
				if(dup>=1.5)
					vec1.push_back(t);
				else
					vec2.push_back(t);
				rep.insert(pid);
			}
			else
				rec.push_back(t);
		}

		// sorting
		sort(vec1.begin(),vec1.end(),cmp);
		sort(vec2.begin(),vec2.end(),cmp);
		sort(rec.begin(),rec.end(),cmp);
		
		// first rank
		fprintf(fp4,"%d,",aid);
		for(int i=0; i<(int)vec1.size(); i++){
			fprintf(fp4,"%d ",vec1[i].y);
		}
		// second rank
		for(int i=0; i<(int)vec2.size(); i++){
			fprintf(fp4,"%d ",vec2[i].y);
		}
		// duplicated
		for(int i=0; i<(int)rec.size(); i++){
			fprintf(fp4,"%d ",rec[i].y);
		}
		fprintf(fp4,"\n");

		cc++;
		
	}
	

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	

	return 0;
}
