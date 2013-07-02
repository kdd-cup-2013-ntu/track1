#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <map>
#include <string>
#include <cmath>

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
};

map<string, statistic> stat;
map<int, int> dist;

int get_section(char *str, char *buffer)
{
    int offset = 0;
    
    for (int i = 0; ; i++)
    {
        if (str[i] == ',' || str[i] == 0 || str[i] == '\n')
        {
            buffer[offset] = 0;
            return i + 1;
        }
        else
            buffer[offset++] = str[i];
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage:%s input_file output_file distribution_file\n", argv[0]);
        return -1;
    }

    FILE *fptr_input;
    fptr_input = fopen(argv[1], "r");
    if (fptr_input == NULL)
    {
        fprintf(stderr, "cannot open input_file\n");
        return -3;
    }

    FILE *fptr_output;
    fptr_output = fopen(argv[2], "w+");
    if (fptr_output == NULL)
    {
        fprintf(stderr, "cannot open output\n");
        return -4;
    }

    FILE *fptr_distribution_output;
    fptr_distribution_output = fopen(argv[3], "w+");
    if (fptr_distribution_output == NULL)
    {
        fprintf(stderr, "cannot opne distribution output\n");
        return -5;
    }

    char line[100000];

    assert(fgets(line, sizeof(line), fptr_input));

   // int paper_id;
    char title[1000];
//    int year;
//    int conference_id;
//    int journal_id;
//    char keyword[1000];
    while(fgets(line, sizeof(line), fptr_input))
    {

#ifdef DEBUG
        fprintf(stderr, "%s", line);
#endif

//        offset += get_section(line + offset, tmp);
//        assert(sscanf(tmp, "%d", &paper_id) == 1);

        for (int i = 0; ;i++)
            if (line[i] > '9' || line[i] < '0')
            {
                strcpy(title, line + i);
                break;
            }

        
//        offset += get_section(line + offset, tmp);
//        assert(sscanf(tmp, "%d", &year) == 1);

//        offset += get_section(line + offset, tmp);
//        assert(sscanf(tmp, "%d", &conference_id) == 1);

//        offset += get_section(line + offset, tmp);
//        assert(sscanf(tmp, "%d", &journal_id) == 1);

//        offset += get_section(line + offset, tmp);
//        strcpy(keyword, tmp);

        


        map<string, bool> seen;
        char *cptr = strtok(title, " \n\r");
        while (cptr != NULL)
        {
			if (*cptr == 0x12)
				continue;
            if (!seen[cptr])
            {
                seen[string(cptr)] = true;
                stat[string(cptr)].df++;
            }
            stat[string(cptr)].wf++;
            cptr = strtok(NULL, " \n\r");
        }


    }

    int max_df = 0;
    fputs("wf = [", fptr_distribution_output);
    for (map<string, statistic>::iterator it = stat.begin(); it != stat.end(); it++)
    {
        if (max_df < it->second.df)
            max_df = it->second.df;
        fprintf(fptr_output, "%s %d %d\n", it->first.c_str(), it->second.wf, it->second.df);
//        dist[it->second.wf]++;

        if ( it->second.df < 20)
            fprintf(fptr_distribution_output, "%d, ", it->second.df);
            
    }

  /*  for (map<int, int>::iterator it = dist.begin(); it != dist.end(); it++)
    {
        fprintf(fptr_distribution_output, "%lf, ", log10(it->first));
    }
    fputs("];\n", fptr_distribution_output);
    fputs("freq = [", fptr_distribution_output);
    for (map<int, int>::iterator it = dist.begin(); it != dist.end(); it++)
    {
        fprintf(fptr_distribution_output, "%lf, ", log10(it->second));
    }
    */
    fputs("];\n", fptr_distribution_output);
    
    fprintf(fptr_distribution_output, "h = figure;\n");
    fprintf(fptr_distribution_output, "hist(wf);\n");
//    fprintf(fptr_distribution_output, "plot(wf, freq, '-b');\n");
//    fprintf(fptr_distribution_output, "xlabel('word frequency');\nylabel('frequency');\n");
    fprintf(fptr_distribution_output, "xlabel('document frequency');\nylabel('frequency');\n");
    fprintf(fptr_distribution_output, "saveas(h, '~/htdocs/plot.jpg', 'jpg');\n");


    printf("max document frequency:%d\n", max_df);
    fclose(fptr_input);
    fclose(fptr_output);
    fclose(fptr_distribution_output);

    return 0;
}
    
