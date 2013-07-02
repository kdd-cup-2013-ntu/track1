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
#define LINE (4096)
#define K (20)
#define N_iTrain (168323)
#define N_iValid (67586)
#define N_oTrain (235909)
#define N_oValid (90088)
#define NAME_THRESHOLD (0.8)
using namespace std;

int APCnt[MAX_AID];
int PACnt[MAX_PID];

int DupPaper[MAX_AID] = {0};

struct data{
	int Aid;
	int Pid;
	int label;
	int dupPidCnt;
};
typedef struct data Data;

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


int compare(const void *_x, const void *_y)
{
	Data *x = (Data *)_x;
	Data *y = (Data *)_y;

	if (x->label < y->label)
		return -1;
	else if (x->label > y->label)
		return 1;
	else if (x->Aid < y->Aid)
		return -1;
	else if (x->Aid > y->Aid)
		return 1;
	else if (x->Pid < y->Pid)
		return -1;
	else if (x->Pid > y->Pid)
		return 1;
	else if (x->dupPidCnt < y->dupPidCnt)
		return -1;
	else if (x->dupPidCnt > y->dupPidCnt)
		return 1;	
	else 
		return 0;
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
	//FILE *rd4_fp = fopen("./PaperAuthor.Name","r");
	//FILE *rd6_fp = fopen("./Author.my","r");
	//FILE *rd7_fp = fopen("./Author.Name","r");

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
	//assert(rd4_fp != NULL);
	//assert(rd6_fp != NULL);
	//assert(rd7_fp != NULL);

	fprintf(stderr,"\nInitializing...");
	//PAData **PAList = (PAData **)malloc(MAX_PID*sizeof(PAData*));
	int **APList = (int **)malloc(MAX_AID*sizeof(int *));
        for (int i = 0; i < MAX_AID; i++)
                APList[i] = (int *)malloc(MAX_AP_CNT*sizeof(int));

	char **PAName = (char **)malloc(MAX_PA_LINE*sizeof(char *));
	for (int i = 0; i < MAX_PA_LINE; i++)
		PAName[i] = (char *)malloc(256*sizeof(char));

	char **AName = (char **)malloc(MAX_A_LINE*sizeof(char *));
	for (int i = 0; i < MAX_A_LINE; i++)
		AName[i] = (char *)malloc(256*sizeof(char));

	//int *AKey = (int *)malloc(MAX_AID*sizeof(int));

	char str[LINE];
	int read_cnt = 0;
	fprintf(stderr,"\nreading PaperAuthor.my");
	while (fgets(str,LINE,rd3_fp) != NULL)
	{
		int Pid,Aid,Key;
		sscanf(str,"%d%d%d",&Pid,&Aid,&Key);
		int i;
		for(i = 0;i < APCnt[Aid];i++)
		{
			if(Pid == APList[Aid][i])
			{
				DupPaper[Aid]++;
				break;
			}
		}
		if(i == APCnt[Aid])
			APList[Aid][APCnt[Aid]++] = Pid;

		read_cnt++;
		if (read_cnt % 200000 == 0) fprintf(stderr,".");
	}

	read_cnt = 1;
	fprintf(stderr,"\nreading PaperAuthor.Name");
	/*while (fgets(str,LINE,rd4_fp) != NULL)
	{
		strncpy( PAName[read_cnt++], str, LINE);
		if (read_cnt % 500000 == 0) fprintf(stderr,".");
	}*/

	read_cnt = 1;
	fprintf(stderr,"\nreading Author.my");
	/*while (fgets(str,LINE,rd6_fp) != NULL)
	{
		int Aid,Key;
		sscanf(str,"%d%d",&Aid,&Key);
		AKey[Aid] = Key;
		read_cnt++;
		if (read_cnt % 10000 == 0) fprintf(stderr,".");
	}*/

	read_cnt = 1;
	fprintf(stderr,"\nreading Author.Name");
	/*while (fgets(str,LINE,rd7_fp) != NULL)
	{
		strncpy( AName[read_cnt++], str, LINE);
		if (read_cnt % 10000 == 0) fprintf(stderr,".");
	}*/

	read_cnt = 0;
	fprintf(stderr, "\nreading TrainData and making dupPACnt feature");
	char str3[1000000]={0};
	fgets(str3,1000000,rd1_fp);
	while (fgets(str3,1000000,rd1_fp) != NULL)
	{
		if (read_cnt%1000 == 0)	fprintf(stderr,".");

		int Aid;
		char *pch,*confList,*deleList;
		pch = strtok( str3, ",");
		sscanf( pch, "%d", &Aid);
		confList = strtok( NULL, ",");
		deleList = strtok( NULL, ","); 		

		char paperList[1000000]={0};
		sprintf(paperList,"%s %s",confList,deleList);
		
		// all papar list
		char *pch1;
		pch1 = strtok( paperList, " ");
		while (pch1 != NULL)
		{
			int Pid;
			sscanf (pch1, "%d", &Pid); 
			pch1 = strtok (NULL, " ");

			fprintf(wr1_fp,"%d\n",DupPaper[Aid]);

		}

		read_cnt++;
	}


	fprintf(stderr, "\nreading ValidData and making dupPACnt feature");
	fgets(str3,1000000,rd2_fp);
	while( fgets(str3,1000000,rd2_fp) != NULL)
	{
		int Aid;
		char *pch,*paperList;
		pch = strtok( str3, ",");
		sscanf( pch, "%d", &Aid);
		paperList = strtok( NULL, ",");

		// confirm papar list
		char *pch1;
		pch1 = strtok( paperList, " ");
		while (pch1 != NULL)
		{
			int Pid;
			sscanf (pch1, "%d", &Pid); 
			pch1 = strtok (NULL, " ");

			fprintf(wr2_fp,"%d\n",DupPaper[Aid]);
		}

		read_cnt++;
	}		

	fprintf(stderr,"\nDone!\n");

	return 0;
}	
