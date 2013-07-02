#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <vector>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include "kdd2013.h"
#include "util.h"

#ifndef DF_MAX
#define DF_MAX 100000
#endif

class VSMElement
{
    public:
        VSMElement(int i, double w):index(i), weight(w)
    {
    };
    int index;
    double weight;
    bool operator < (const VSMElement& v) const
    {
        return index < v.index;
    };
};

using namespace std;
Author *authors;
Paper *papers;
map<string, int> word_index;
vector<VSMElement> *paper_title;
vector<VSMElement> *author_vector;
map<int, double> idf;

int paper_title_length[MAX_PAPER_ID_IN_PAPER_AUHTOR + 1];

#define NUM_PAPER 2257250.0
void load_word(FILE *fptr_word)
{
    char word[1000];
    int wf, df;
    int idx = 0;
    while(fscanf(fptr_word, "%s %d %d", word, &wf, &df) == 3)
    {
        bool ok = true;
        if (wf > DF_MAX)
            ok = false;
        
        if (ok)
        {
            word_index[word] = idx;
            idf[idx] = log(NUM_PAPER /  df);
            idx++;
        }
        else
            word_index[word] = -1;
    }
}

void load_paper_title(FILE *fptr_paper_title)
{
    char line[1000000];
    
    assert(fgets(line, sizeof(line), fptr_paper_title));

    while (fgets(line, sizeof(line), fptr_paper_title))
    {
        vector<string> slices;
        int paper_id;
        map<string, int> count;

        split_string(line, slices, " \n");
        assert(slices.size() > 0);
        assert(sscanf(slices[0].c_str(), "%d", &paper_id) == 1);
        
        paper_title_length[paper_id] = (int)slices.size() - 1;

        for (vector<string>::iterator it = slices.begin() + 1; it != slices.end(); it++)
            count[*it]++;

        vector<VSMElement>& pt = paper_title[paper_id];

        for (map<string, int>::iterator it = count.begin(); it != count.end(); it++)
        {
            map<string, int>::iterator it2 = word_index.find(it->first);
            assert(it2 != word_index.end());
            int index = it2->second;

            pt.push_back(VSMElement(index, (double)it->second));
        }
    }
}

void compute_author_vector(int author_id)
{
    map<int, double> acc;

    Author *a = authors + author_id;

    for (vector<int>::iterator it = a->paper_ids.begin(), end = a->paper_ids.end(); it != end; it++)
    {
        vector<VSMElement> *pt = paper_title + *it;
        for (vector<VSMElement>::iterator it2 = pt->begin(), end2 = pt->end(); it2!= end2; it2++)
            acc[it2->index] += it2->weight;
    }

    vector<VSMElement> *v = author_vector + author_id;
    
    int paper_count = (int)a->paper_ids.size();
    for (map<int, double>::iterator it = acc.begin(); it != acc.end(); it++)
        v->push_back(VSMElement(it->first, it->second / paper_count));

}
inline double inner_product(vector<VSMElement>& va, vector<VSMElement>& vb)
{
    vector<VSMElement>::iterator ita = va.begin(), itb = vb.begin(), enda = va.end(), endb = vb.end();
    
    if (va.size() ==0 || vb.size() == 0)
        return 0.0;

    double sum = 0.5;

    while (ita != enda && itb != endb)
    {
        if (ita->index == itb->index)
        {
            sum += ita->weight * itb->weight ;//* idf[ita->index];
            ita++;
            itb++;
        }
        else if (ita->index < itb->index)
            ita++;
        else
            itb++;
    }
    return sum;
}

void generate_feature(int author_id, int paper_id, FILE *fptr_output)
{
    double score = inner_product(author_vector[author_id], paper_title[paper_id]);

    fprintf(fptr_output, "%lf ", score);
    fprintf(fptr_output, "%d ", paper_title_length[paper_id]);
}
    
void generate_train_output(vector<TrainRecord> &train_csv, FILE *fptr_output)
{
    for (vector<TrainRecord>::iterator it = train_csv.begin(), end = train_csv.end(); it != end; it++)
    {
        int author_id = it->author_id;

        for (vector<int>::iterator it2 = it->confirmed_paper_ids.begin(), end2 = it->confirmed_paper_ids.end(); it2 != end2; it2++)
        {
            fprintf(fptr_output, "1 ");
            generate_feature(author_id, *it2, fptr_output);
            fprintf(fptr_output, "\n");
        }

        for (vector<int>::iterator it2 = it->deleted_paper_ids.begin(), end2 = it->deleted_paper_ids.end(); it2 != end2; it2++)
        {
            fprintf(fptr_output, "-1 ");
            generate_feature(author_id, *it2, fptr_output);
            fprintf(fptr_output, "\n");
        }
    }
}

void generate_test_output(vector<TestRecord> &test_csv, FILE *fptr_output)
{
    for (vector<TestRecord>::iterator it = test_csv.begin(), end = test_csv.end(); it != end; it++)
    {
        int author_id = it->author_id;

        for (vector<int>::iterator it2 = it->paper_ids.begin(), end2 = it->paper_ids.end(); it2 != end2; it2++)
        {
            fprintf(fptr_output, "0 ");
            generate_feature(author_id, *it2, fptr_output);
            fprintf(fptr_output, "\n");
        }
    }
}
    

int main(int argc, char *argv[])
{
    if (argc != 1 + 7)
    {
        fprintf(stderr, "Usage : %s PaperAuthor.csv PaperTitle PaperTitleStat Train.csv Test.csv train_output test_outputfile\n", argv[0]);
        return -1;
    }

    FILE *fptr_paper_author_csv;
    fptr_paper_author_csv = fopen(argv[1], "r");
    assert(fptr_paper_author_csv != NULL);

    FILE *fptr_paper_title;
    fptr_paper_title = fopen(argv[2], "r");
    assert(fptr_paper_title != NULL);

    FILE *fptr_paper_title_stat;
    fptr_paper_title_stat= fopen(argv[3], "r");
    assert(fptr_paper_title_stat != NULL);

    FILE *fptr_train_csv;
    fptr_train_csv = fopen(argv[4], "r");
    assert(fptr_train_csv);

    FILE *fptr_test_csv;
    fptr_test_csv = fopen(argv[5], "r");
    assert(fptr_test_csv != NULL);

    FILE *fptr_train_output;
    fptr_train_output = fopen(argv[6], "w+");
    assert(fptr_train_output != NULL);

    FILE *fptr_test_output;
    fptr_test_output = fopen(argv[7], "w+");
    assert(fptr_test_output != NULL);

    authors = new Author[MAX_AUTHOR_ID_IN_PAPER_AUHTOR + 1];
    papers = new Paper[MAX_PAPER_ID_IN_PAPER_AUHTOR + 1];
    paper_title = new vector<VSMElement>[MAX_PAPER_ID_IN_PAPER_AUHTOR + 1];
    author_vector = new vector<VSMElement>[MAX_AUTHOR_ID_IN_PAPER_AUHTOR + 1];

    printf("Loading word\n");
    load_word(fptr_paper_title_stat);

    printf("Loading paper title\n");
    load_paper_title(fptr_paper_title);

    printf("Loading PaperAuthor.csv\n");
    assert(load_paper_authors(fptr_paper_author_csv, authors, papers) == 0);

    printf("Loading Train.csv\n");
    vector<TrainRecord> train_csv;
    assert(load_train_csv(fptr_train_csv, train_csv) == 0);

    printf("Loading Test.csv\n");
    vector<TestRecord> test_csv;
    assert(load_test_csv(fptr_test_csv, test_csv) == 0);

    printf("Computing Author Vector\n");
    for (int i = 1; i <= MAX_AUTHOR_ID_IN_PAPER_AUHTOR; i++)
        compute_author_vector(i);

    printf("Outputing Train feature\n");
    generate_train_output(train_csv, fptr_train_output);

    printf("Output Test feature\n");
    generate_test_output(test_csv, fptr_test_output);

    return 0;
}

