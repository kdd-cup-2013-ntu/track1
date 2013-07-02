#include <cstdio>
#include <cstdlib>
#include <cassert>

#define NUM_RECORD 1000000

struct RECORD
{
    int paper_id;
    double score;
}records[NUM_RECORD];

int compare(const void *_a, const void *_b)
{
    struct RECORD* a = (struct RECORD*) _a, *b = (struct RECORD *)_b;
    if (a->score > b->score)
        return -1;
    if (a->score < b->score)
        return 1;
    return 0;
}
          

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage : %s score_file meta_file output_file\n", argv[0]);
        return -1;
    }

    FILE *fptr_score_file;
    fptr_score_file = fopen(argv[1], "r");
    assert(fptr_score_file != NULL);

    FILE *fptr_meta_file;
    fptr_meta_file = fopen(argv[2], "r");
    assert(fptr_meta_file != NULL);

    FILE *fptr_output_file;
    fptr_output_file = fopen(argv[3], "w+");
    assert(fptr_output_file != NULL);

    int author_id;
    int paper_count;
//    int paper_id;

    fputs("AuthorId,PaperIds\n", fptr_output_file);

    while(fscanf(fptr_meta_file, "%d %d", &author_id, &paper_count) == 2)
    {
        for (int i = 0; i < paper_count; i++)
        {
            assert(fscanf(fptr_meta_file, "%d", &records[i].paper_id) == 1);
            assert(fscanf(fptr_score_file, "%lf", &records[i].score) == 1);
        }
        qsort(records, paper_count, sizeof(struct RECORD), compare);
        fprintf(fptr_output_file, "%d,", author_id);

        for (int i = 0; i < paper_count; i++)
            fprintf(fptr_output_file, "%d ", records[i].paper_id);
        fputs("\n", fptr_output_file);
    }

    return 0;
}
        


