#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "kdd2013.h"
#include <vector>
#include <cstring>

using namespace std;

vector<int> paper_topic_distribution[MAX_PAPER_ID_IN_PAPER_AUHTOR + 1];
int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage : %s PaperTopicAssignment PaperIdxMeta K Output\n", argv[0]);
        return -1;
    }

    FILE *fptr_assignment;
    fptr_assignment = fopen(argv[1], "r");
    assert(fptr_assignment != NULL);

    FILE *fptr_idx_meta;
    fptr_idx_meta = fopen(argv[2], "r");
    assert(fptr_idx_meta != NULL);

    FILE *fptr_output_file;
    fptr_output_file = fopen(argv[4], "w+");
    assert(fptr_output_file != NULL);

    int K;
    assert(sscanf(argv[3], "%d", &K) == 1);

    vector<int> active_paper_id;

    char line[1000000];
    int paper_id;
    while(fscanf(fptr_idx_meta, "%d", &paper_id )== 1)
    {
        vector<int> &topic_distribution = paper_topic_distribution[paper_id];
//        topic_distribution.resize(K);
        for (int i = 0; i < K; i++)
            topic_distribution.push_back(0);
        assert(fgets(line, sizeof(line), fptr_assignment));

        char *cptr;
        cptr = strtok(line, " \n");

        active_paper_id.push_back(paper_id);

        while (cptr != NULL)
        {
            int word_idx;
            int topic_idx;
            assert(sscanf(cptr, "%d:%d", &word_idx, &topic_idx) == 2);
            assert(topic_idx < K);
            topic_distribution[topic_idx] += 1;
            cptr = strtok(NULL, " \n");
        }
    }

    fprintf(fptr_output_file, "%d\n", K);
    for (vector<int>::iterator it = active_paper_id.begin(), end = active_paper_id.end(); it != end; it++)
    {
        vector<int> &topic_distribution = paper_topic_distribution[*it];

        fprintf(fptr_output_file, "%d ", *it);

        for (int i = 0; i < K; i++)
        {
            fprintf(fptr_output_file, "%d ", topic_distribution[i]);
        }

        fprintf(fptr_output_file, "\n");
    }
    return 0;
}
