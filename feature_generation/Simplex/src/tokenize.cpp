#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cctype>
#include "stemming.h"

bool skip[256];
int get_section(char *str, char *buffer)
{
    int offset = 0;
    int state = 0;
    
    for (int i = 0; ; i++)
    {
        switch (state)
        {
            case 0:
                if (str[i] == '\"')
                    state = 1;
                else if (str[i] == ',' || str[i] == 0 || str[i] == '\n')
                {
                    buffer[offset] = 0;
                    return i + 1;
                }
                else if (!skip[(int)str[i]])
                    buffer[offset++] = str[i];
                break;
            case 1:
                if (str[i] == '\"')
                    state = 0;
                else
                    buffer[offset++] = str[i];
                break;
            default:
                assert(false);
        }
    }
}
        

void init_skip()
{
    //skip['<'] = skip['>'] = skip['-'] = skip['('] = skip[')'] = true;
}

int main(int argc, char *argv[])
{
    init_skip();
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
        return -1;
    }

    FILE *fptr_input, *fptr_output;
    fptr_input = fopen(argv[1], "r");
    if (fptr_input == NULL)
    {
        fprintf(stderr, "Cannot open input file\n");
        return -2;
    }

    fptr_output = fopen(argv[2], "w+");
    if (fptr_output == NULL)
    {
        fprintf(stderr, "Cannot open output file\n");
        return -3;
    }

    char line[100000];

    assert(fgets(line, sizeof(line), fptr_input));
    fprintf(fptr_output, "%s", line);

    int paper_id;
    char title[1000];
    int year;
    int conference_id;
    int journal_id;
    char keyword[1000];
    while(fgets(line, sizeof(line), fptr_input))
    {

#ifdef DEBUG
        fprintf(stderr, "%s", line);
#endif
        char tmp[1000];
        int offset = 0;

        offset += get_section(line + offset, tmp);
        assert(sscanf(tmp, "%d", &paper_id) == 1);

        offset += get_section(line + offset, tmp);
        strcpy(title, tmp);
        
        offset += get_section(line + offset, tmp);
        assert(sscanf(tmp, "%d", &year) == 1);

        offset += get_section(line + offset, tmp);
        assert(sscanf(tmp, "%d", &conference_id) == 1);

        offset += get_section(line + offset, tmp);
        assert(sscanf(tmp, "%d", &journal_id) == 1);

        offset += get_section(line + offset, tmp);
        strcpy(keyword, tmp);

        
        fprintf(fptr_output, "%d ", paper_id);


        int len = (int)strlen(title);
        for (int i = 0; i < len; i++)
            if (title[i] < 'a' || title[i] > 'z')
                title[i] = (char)tolower(title[i]);

//        printf("%s\n", title);

        int idx = 0;
        for (int i = 0; ;)
        {
            if (i == len || title[i] < 'a' || title[i] > 'z')
            {
                if (idx != 0)
                {
                    tmp[stem(tmp, 0, idx - 1) + 1] = 0;
//                    tmp[idx] = 0;
                    fprintf(fptr_output, "%s ", tmp);
                    idx = 0;
                }
                if (i == len) break;
                i++;
            }
            else
            {
                tmp[idx] = title[i];
                idx++;
                i++;
            }
        }

        //fprintf(fptr_output, ",%d,%d,%d", year, conference_id, journal_id);

        //fprintf(fptr_output, ",%s\n", keyword);
        fprintf(fptr_output, "\n");

    }
    fclose(fptr_input);
    fclose(fptr_output);
}

