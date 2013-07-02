#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include "kdd2013.h"
#include "util.h"

const int K = 100;
using namespace std;

Paper *papers;
Author *authors;
inline double get_score(double *va, double *vb)
{
    if (vb == NULL || va == NULL)
        return 0.0;
    double sum = 0.0;
    for (int i = 0; i < K; i++)
        sum += va[i] * vb[i];

    return sum;
}
inline int get_author_idx(int author_id, vector<int> &author_ids)
{
    for (int i = 0;;i++)
        if (author_ids[i] == author_id)
            return i;
}
void get_coauthor_scores(int author_id, int paper_id, vector<double> &scores)
{
    assert(scores.size() == 0);

    vector<int> coauthor_ids;
    assert(get_coauthor_ids(author_id, papers + paper_id, coauthor_ids) == 0);
    if (coauthor_ids.size() == 0)
    {
        scores.push_back(1.0);
        return ;
    }
    
    double *author_topic_feature = authors[author_id].topic_feature;

    for (vector<int>::iterator it = coauthor_ids.begin(), end = coauthor_ids.end(); it != end; it++)
    {
        scores.push_back(get_score(author_topic_feature, authors[*it].topic_feature));
    }
}

void generate_coauthor_topic_feature(FILE* fptr_output, int author_id, int paper_id)
{
    vector<double> scores;
    get_coauthor_scores(author_id, paper_id, scores);
    //int coauthor_count = (int)paper->author_ids.size();


    double max_score = 0.0;
    double min_score = 1.0;
    double mean_score = 0.0;
    int score_count = (int)scores.size();

    for (vector<double>::iterator it = scores.begin(); it != scores.end(); it++)
    {
        if (*it > max_score)
            max_score = *it;
        if (*it < min_score)
            min_score = *it;

        mean_score += *it;
    }

    mean_score /= score_count;

    fprintf(fptr_output, "%lf %lf %lf %lf %lf", max_score, min_score, mean_score, max_score - min_score, max_score - mean_score);
/*
    vector<double> w;
    vector<double> wo;
    for (int i = 0; i < coauthor_count
  */  
}

void generate_train_feature_file(FILE* fptr_output, vector<TrainRecord> &train_csv)
{
    for (vector<TrainRecord>::iterator it = train_csv.begin(), end = train_csv.end(); it != end; it++)
    {
        for (vector<int>::iterator it2 = it->confirmed_paper_ids.begin(), end2 = it->confirmed_paper_ids.end(); it2 != end2; it2++)
        {
            fprintf(fptr_output, "1 ");
            generate_coauthor_topic_feature(fptr_output, it->author_id, *it2);
            fprintf(fptr_output, "\n");
        }

        for (vector<int>::iterator it2 = it->deleted_paper_ids.begin(), end2 = it->deleted_paper_ids.end(); it2 != end2; it2++)
        {
            fprintf(fptr_output, "-1 ");
            generate_coauthor_topic_feature(fptr_output, it->author_id, *it2);
            fprintf(fptr_output, "\n");
        }
    }
}

void generate_test_feature_file(FILE* fptr_output, vector<TestRecord> &test_csv)
{
    for (vector<TestRecord>::iterator it = test_csv.begin(), end = test_csv.end(); it != end; it++)
    {
        for (vector<int>::iterator it2 = it->paper_ids.begin(), end2 = it->paper_ids.end(); it2 != end2; it2++)
        {
            fprintf(fptr_output, "0 ");
            generate_coauthor_topic_feature(fptr_output, it->author_id, *it2);
            fprintf(fptr_output, "\n");
        }
    }
}   
void compute_coauthor_score_for_all()
{
    Paper* paper = papers + 1;
    for (int i = 1; i <= MAX_PAPER_ID_IN_PAPER_AUHTOR; i++)
    {
        if (paper->paper_id != i) continue;
        compute_coauthor_score(paper, authors, K);
    }
}
        
int main(int argc, char *argv[])
{
    if (argc != 1 + 4 + 2)
    {
        fprintf(stderr, "Usage : %s AuthorTopicFeature PaperAuthor.csv Train.csv Test.csv TrainFeatureOutput TestFeatureOutput\n", argv[0]);
        return -1;
    }

    papers = new Paper[MAX_PAPER_ID_IN_PAPER_AUHTOR + 1];
    authors = new Author[MAX_AUTHOR_ID_IN_PAPER_AUHTOR + 1];

    FILE *fptr_author_topic_feature;
    fptr_author_topic_feature = fopen(argv[1], "r");
    assert(fptr_author_topic_feature != NULL);

    FILE *fptr_paper_author;
    fptr_paper_author = fopen(argv[2], "r");
    assert(fptr_paper_author != NULL);
    
    FILE *fptr_train_csv;
    fptr_train_csv = fopen(argv[3], "r");
    assert(fptr_train_csv != NULL);

    FILE *fptr_test_csv;
    fptr_test_csv = fopen(argv[4], "r");
    assert(fptr_test_csv != NULL);

    FILE *fptr_train_feature_output;
    fptr_train_feature_output = fopen(argv[5], "w+");
    assert(fptr_train_feature_output != NULL);

    FILE *fptr_test_feature_output;
    fptr_test_feature_output = fopen(argv[6], "w+");
    assert(fptr_test_feature_output != NULL);

    vector<TrainRecord> train_csv;
    vector<TestRecord> test_csv;

    assert(load_train_csv(fptr_train_csv, train_csv) == 0);
    assert(load_test_csv(fptr_test_csv, test_csv) == 0);
    assert(load_paper_authors(fptr_paper_author, authors, papers) == 0);
    assert(load_author_topic_feature(fptr_author_topic_feature, authors, 100) == 0);

//    compute_coauthor_score_for_all();

    generate_train_feature_file(fptr_train_feature_output, train_csv);
    generate_test_feature_file(fptr_test_feature_output, test_csv);


    return 0;
}

