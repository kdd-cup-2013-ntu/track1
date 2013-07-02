#include <cstdio>
#include <cstring>
#include <cassert>
#include <map>
#include <string>

using namespace std;

class statistic
{
public:
    statistic(int _df = 0, int _wf = 0)
    {
        df = _df;
        wf = _wf;
    }

    int df;
    int wf;
    int idx;
};

map<string, statistic> tokens;

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        fprintf(stderr, "Usage :%s input_file stat_file stop_word doc_output_file word_output meta_output\n", argv[0]);
        return -1;
    }

    FILE *fptr_input;
    fptr_input = fopen(argv[1], "r");
    assert(fptr_input != NULL);

    FILE *fptr_stat;
    fptr_stat = fopen(argv[2], "r");
    assert(fptr_stat != NULL);

    FILE *fptr_stop_word;
    fptr_stop_word = fopen(argv[3], "r");
    assert(fptr_stop_word !=NULL);

    FILE *fptr_output;
    fptr_output = fopen(argv[4], "w+");
    assert(fptr_output != NULL);

    FILE *fptr_word_output;
    fptr_word_output = fopen(argv[5], "w+");
    assert(fptr_word_output != NULL);

    FILE *fptr_meta_output;
    fptr_meta_output = fopen(argv[6], "w+");
    assert(fptr_meta_output != NULL);


    char line[1000000];
    
    map<string, bool> IsStopWord;
    while (fscanf(fptr_stop_word, "%s", line) != EOF)
    {
        IsStopWord[string(line)] = true;
    }

    int df, wf;
    char token[10000];
    int cur_idx = 0;

    while(fgets(line, sizeof(line), fptr_stat))
    {
        assert(sscanf(line, "%s %d %d", token, &df, &wf) == 3);
        bool ok = true;
        if (IsStopWord.find(string(token)) != IsStopWord.end())
        {
            printf("stop word : %s\n", token);
            ok = false;
        }
        tokens[string(token)].df = df;
        tokens[string(token)].wf = wf;
    
#ifdef DF_MIN
        if (df < DF_MIN)
            ok = false;
#endif
#ifdef DF_MAX
        if (df > DF_MAX)
            ok = false;
#endif
#ifdef WF_MIN
        if (wf < WF_MIN)
            ok = false;
#endif
#ifdef WF_MAX
        if (wf > WF_MAX)
            ok = false;
#endif
        if (ok)
            tokens[string(token)].idx = cur_idx++;
        else
            tokens[string(token)].idx = -1;

    }

    fclose(fptr_stat);

	assert(fgets(line, sizeof(line), fptr_input));
    while(fgets(line, sizeof(line), fptr_input))
    {
        int paper_id;
        char *cptr = strtok(line, " \n");
        assert(sscanf(cptr, "%d", &paper_id) == 1);
        cptr = strtok(NULL, " \n");
        bool isEmpty = true;
        bool has_word = false;;
        printf("%d:", paper_id);
        while (cptr)
        {
            statistic &st = tokens[string(cptr)];
            has_word = true;
            if (st.idx != -1)
            {
                fprintf(fptr_output, "%s ", cptr);
                isEmpty = false;
            }
            else
            {
                printf("%s ", cptr);
            }
            cptr = strtok(NULL, " \n");
        }
        
        if (!isEmpty)
        {
            fputs("\n", fptr_output);
            fprintf(fptr_meta_output, "%d\n", paper_id);
            puts("");
        }
        else
        {
            if (has_word)
            {
                fprintf(stdout, "Warning paper %d title is removed\n", paper_id);
            }
                puts("");

        }

    }

    fclose(fptr_input);
    fclose(fptr_output);

    for (map<string, statistic>::iterator it = tokens.begin(); it != tokens.end(); it++)
        if (it->second.idx != -1)
            fprintf(fptr_word_output, "%s %d\n", it->first.c_str(), it->second.idx);    


    return 0;
}


      

