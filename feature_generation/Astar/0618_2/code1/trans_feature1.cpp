#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cassert>
#include<cmath>
#include<ctime>
#include<iostream>
#include<map>
#include<algorithm>

#define MAX_A_LINE (247203+10)
#define MAX_PA_LINE (12775820+10)
#define MAX_AID (2293837+10)
#define MAX_PID (2259021+10)
#define MAX_AP_CNT (3316+10)
#define MAX_PA_CNT (3258+10)
#define LINE (256)
#define K (20)
#define N_iTrain (168323)
#define N_iValid (67586)
#define N_oTrain (235909)
#define N_oValid (90088)
#define NAME_THRESHOLD (0.8)
using namespace std;

int APCnt[MAX_AID];
int PACnt[MAX_PID];

struct padata{
	int Aid;
	int Key;
};
typedef struct padata PAData;

void findLastName(char *Name, char *lastName)
{
	char tmp[LINE]={0};
	

	for(int i=0,j=0; Name[i]!='\0'; i++)
	{
		if( Name[i] == ' ' || Name[i] == '.'){
			if (strlen(lastName) != 0)
				strncpy(tmp,lastName,LINE);
			memset(lastName,'\0',LINE);
			j = 0;
		}
		else
			lastName[j++] = Name[i];
	}
	if (strlen(lastName) == 0)
		strncpy(lastName,tmp,LINE);
}

double  strcmp95(char *ying, char *yang, long y_length, int *ind_c[]);

int main( int argc, char *argv[])
{
	if (argc != 5)
	{
		fprintf(stderr,"./a.out Train.csv Valid.csv TrainOut.F ValidOut.F\n");
		return 0;
	}

	FILE *rd1_fp = fopen(argv[1],"r"); // TrainData 
	FILE *rd2_fp = fopen(argv[2],"r"); // ValidData
	FILE *rd3_fp = fopen("./mydata/PaperAuthor.my","r"); 
	FILE *rd4_fp = fopen("./mydata/PaperAuthor.Name","r");
	FILE *rd6_fp = fopen("./mydata/Author.my","r");
	FILE *rd7_fp = fopen("./mydata/Author.Name","r");

	char str1[1024],str2[1024];
	sprintf(str1,"%s",argv[3]);
	sprintf(str2,"%s",argv[4]);
	FILE *wr1_fp = fopen(str1,"w");
	FILE *wr2_fp = fopen(str2,"w");
	//FILE *wr3_fp = fopen("Train.log","w");
	//FILE *wr4_fp = fopen("Valid.log","w");
	assert(rd1_fp != NULL);
	assert(rd2_fp != NULL);
	assert(rd3_fp != NULL);
	assert(rd4_fp != NULL);
	assert(rd6_fp != NULL);
	assert(rd7_fp != NULL);

	fprintf(stderr,"\nInitializing...");
	PAData **PAList = (PAData **)malloc(MAX_PID*sizeof(PAData*));
	for (int i = 0; i < MAX_PID; i++)
		PAList[i] = (PAData *)malloc(MAX_PA_CNT*sizeof(PAData));
	
	char **PAName = (char **)malloc(MAX_PA_LINE*sizeof(char *));
	for (int i = 0; i < MAX_PA_LINE; i++)
		PAName[i] = (char *)malloc(1024*sizeof(char));

	char **AName = (char **)malloc(MAX_A_LINE*sizeof(char *));
	for (int i = 0; i < MAX_A_LINE; i++)
		AName[i] = (char *)malloc(1024*sizeof(char));
	
	int *AKey = (int *)malloc(MAX_AID*sizeof(int));
	
	char str[9096];
	int read_cnt = 0;
	fprintf(stderr,"\nreading PaperAuthor.my");
	while (fgets(str,LINE,rd3_fp) != NULL)
	{
		int Pid,Aid,Key;
		sscanf(str,"%d%d%d",&Pid,&Aid,&Key);
		PAList[Pid][PACnt[Pid]].Aid = Aid;
		PAList[Pid][PACnt[Pid]].Key = Key;
		PACnt[Pid]++;
		read_cnt++;
		if (read_cnt % 200000 == 0) fprintf(stderr,".");
	}
	
	read_cnt = 1;
	fprintf(stderr,"\nreading PaperAuthor.Name");
	while (fgets(str,LINE,rd4_fp) != NULL)
	{
		strncpy( PAName[read_cnt++], str, LINE);
		if (read_cnt % 500000 == 0) fprintf(stderr,".");
	}

	
	read_cnt = 1;
	fprintf(stderr,"\nreading Author.my");
	while (fgets(str,LINE,rd6_fp) != NULL)
	{
		int Aid,Key;
		sscanf(str,"%d%d",&Aid,&Key);
		AKey[Aid] = Key;
		read_cnt++;
		if (read_cnt % 10000 == 0) fprintf(stderr,".");
	}
	
	read_cnt = 1;
	fprintf(stderr,"\nreading Author.Name");
	while (fgets(str,LINE,rd7_fp) != NULL)
	{
		strncpy( AName[read_cnt++], str, LINE);
		if (read_cnt % 10000 == 0) fprintf(stderr,".");
	}

	char str3[1000000]={0};
	read_cnt = 0; 
	fprintf(stderr, "\nreading TrainData and making dupPidCnt feature");
	fgets(str3,1000000,rd1_fp);
	while (fgets(str3,1000000,rd1_fp) != NULL)
	{
		if(read_cnt % 1000 == 0) fprintf(stderr,".");
		
		int Aid;
		char *pch,*confList,*deleList;
		pch = strtok( str3, ",");
		sscanf( pch, "%d", &Aid);
		confList = strtok( NULL, ",");
		deleList = strtok( NULL, ","); 		

		char ALastName[LINE]={0};
		AName[AKey[Aid]][strlen(AName[AKey[Aid]])-1] = '\0';
		ALastName[strlen(ALastName)-1] = '\0';
		findLastName(AName[AKey[Aid]],ALastName);
		
		char paperList[1000000]={0};
		sprintf(paperList,"%s %s",confList,deleList);
		//printf("%d\n",Aid);
		// all papar list
		char *pch1;
		pch1 = strtok( paperList, " ");
		while (pch1 != NULL)
		{
			int curPid;
			sscanf (pch1, "%d", &curPid); 
			pch1 = strtok (NULL, " ");
			int coAuthorCnt = 0;
			int dupCoAidCnt = 0;
			map<int,double> coAuthorDis;
			map<int,double> coAuthorLNDis;
	
			//printf("%d %d\n",Aid,curPid);

			for (int j=0; j < PACnt[curPid]; j++)
			{
				int coAid = PAList[curPid][j].Aid;
				int Key = PAList[curPid][j].Key;
				int *ind_c[2] = {0,0};
				char coALastName[LINE]={0};
				
				PAName[Key][strlen(PAName[Key])-1] = '\0';
				coALastName[strlen(coALastName)-1] = '\0';
				findLastName(PAName[Key],coALastName);
				
				int max_len = max(strlen(AName[AKey[Aid]]),strlen(PAName[Key]));
				double Name_JaroDis = strcmp95( AName[AKey[Aid]], PAName[Key], max_len, ind_c);
				int max_len1 = max( strlen(ALastName), strlen(coALastName) );
				double LastName_JaroDis = strcmp95( ALastName, coALastName, max_len1, ind_c);
				if (Aid == coAid)	
					dupCoAidCnt++;
				if (Aid != coAid){
					coAuthorDis[coAuthorCnt] = Name_JaroDis;
					coAuthorLNDis[coAuthorCnt++] = LastName_JaroDis;
				}
				//fprintf(wr3_fp,"       Aid:%d JaroDis:%lf\n", coAid,Name_JaroDis);
				//fprintf(wr3_fp,"			  Name:%s\n",PAName[Key] );
				
			}
			
			double f1=-1,f2=-1;
			map<int,double>::iterator x=coAuthorDis.begin();
			for(; x!=coAuthorDis.end(); x++)
			{
				/*
				if(Aid == 3144 && curPid == 1752173)
				{
					fprintf(stderr,"dis=%lf\n",x->second);
				}
				*/
				if (f1 < x->second)
					f1 = x->second;
			}
			map<int,double>::iterator y=coAuthorLNDis.begin();
			for(; y!=coAuthorLNDis.end(); y++)
				if (f2 < y->second)
					f2 = y->second;
			
	
			//fprintf(wr1_fp,"%d %d 0 %d %lf %f\n",Aid,curPid,dupCoAidCnt,f1,f2);
			//fprintf(wr1_fp,"%d %d %d %lf %f\n",Aid,curPid,dupCoAidCnt,f1,f2);
			fprintf(wr1_fp,"%d %lf %f\n",dupCoAidCnt,f1,f2);
			//fprintf(wr1_fp,"%d %d %lf %lf\n",Aid,curPid,f1,f2);
		}
		read_cnt++;
	}
	//return 0;	
	
	read_cnt = 0; 
	fprintf(stderr, "\nreading ValidData and making dupPidCnt feature");
	fgets(str3,1000000,rd2_fp);
	while (fgets(str3,1000000,rd2_fp) != NULL)
	{
		if(read_cnt % 1000 == 0) fprintf(stderr,".");
		
		int Aid;
		char *pch,*confList;
		pch = strtok( str3, ",");
		sscanf( pch, "%d", &Aid);
		confList = strtok( NULL, ",");
		//deleList = strtok( NULL, ","); 		

		char ALastName[LINE]={0};
		AName[AKey[Aid]][strlen(AName[AKey[Aid]])-1] = '\0';
		ALastName[strlen(ALastName)-1] = '\0';
		findLastName(AName[AKey[Aid]],ALastName);
		
		char paperList[1000000]={0};
		sprintf(paperList,"%s",confList);
	
		// all papar list
		char *pch1;
		pch1 = strtok( paperList, " ");
		while (pch1 != NULL)
		{
			int curPid;
			sscanf (pch1, "%d", &curPid); 
			pch1 = strtok (NULL, " ");
			
			int coAuthorCnt = 0;
			int dupCoAidCnt = 0;
			map<int,double> coAuthorDis;
			map<int,double> coAuthorLNDis;

			
			for (int j=0; j < PACnt[curPid]; j++)
			{
				int coAid = PAList[curPid][j].Aid;
				int Key = PAList[curPid][j].Key;
				int *ind_c[2] = {0,0};
				char coALastName[LINE]={0};
				
				PAName[Key][strlen(PAName[Key])-1] = '\0';
				coALastName[strlen(coALastName)-1] = '\0';
				findLastName(PAName[Key],coALastName);
				
				int max_len = max(strlen(AName[AKey[Aid]]),strlen(PAName[Key]));
				double Name_JaroDis = strcmp95( AName[AKey[Aid]], PAName[Key], max_len, ind_c);
				int max_len1 = max( strlen(ALastName), strlen(coALastName) );
				double LastName_JaroDis = strcmp95( ALastName, coALastName, max_len1, ind_c);
				if (Aid == coAid)	
					dupCoAidCnt++;
				if (Aid != coAid){
					coAuthorDis[coAuthorCnt] = Name_JaroDis;
					coAuthorLNDis[coAuthorCnt++] = LastName_JaroDis;
				}
				//fprintf(wr4_fp,"       Aid:%d JaroDis:%lf\n", coAid,Name_JaroDis);
				//fprintf(wr4_fp,"			  Name:%s\n",PAName[Key] );
				
			}
			
			double f1=-1,f2=-1;
			map<int,double>::iterator x=coAuthorDis.begin();
			for(; x!=coAuthorDis.end(); x++)
				if (f1 < x->second)
					f1 = x->second;
			
			map<int,double>::iterator y=coAuthorLNDis.begin();
			for(; y!=coAuthorLNDis.end(); y++)
				if (f2 < y->second)
					f2 = y->second;

			//fprintf(wr1_fp,"%d %d 0 %d %lf %f\n",Aid,curPid,dupCoAidCnt,f1,f2);
			//fprintf(wr1_fp,"%d %d %d %lf %f\n",Aid,curPid,dupCoAidCnt,f1,f2);
			//fprintf(wr2_fp,"%d %d %lf \n",Aid,curPid,f1);
			fprintf(wr2_fp,"%d %lf %f\n",dupCoAidCnt,f1,f2);
		}
		read_cnt++;
	}
	
	fprintf(stderr,"\nDone!\n");
	return 0;
}
