#ifndef _H_UTIL_
#define _H_UTIL_

#include <cstdio>

#include <vector>
#include <set>
#include <string>
#include <map>
using namespace std;


int get_section(char *str, char *buffer, bool *skip);

int get_levenshtein_distance(const char *a, const char *b);

class Author
{
    public:
        char author_name[500];
        char affiliation_name[500];
        int author_id;
        vector<int> paper_ids;
        double *topic_feature;
        set<int> paper_ids_set;

};

class Paper
{
    public:
        char title[500];
        int paper_id;
        int year;
        int journal_id;
        int conference_id;
        char keywords[500];
        vector<int> author_ids;
        double **authorwise_score;
        int get_author_idx(int author_id);
        set<int> author_ids_set;
        
};

class Group
{
    public:
        vector<int> author_ids;
        double *distances;
};

class TrainRecord
{
    public:
    int author_id;
    vector<int> confirmed_paper_ids;
    vector<int> deleted_paper_ids;
};

class TestRecord
{
public:
    int author_id;
    vector<int> paper_ids;
};
int load_papers(FILE *fptr_paper, vector<Paper>& papers);
int load_paper_authors(FILE *fptr_paper_author, Author *authors, Paper *papers);
int load_groups(FILE *fptr_group, vector<Group>& groups);
int load_train_csv(FILE *fptr_train_csv, vector<TrainRecord>& train_records);
int load_test_csv(FILE *fptr_test_csv, vector<TestRecord>& test_records);
int load_author_topic_feature(FILE *fptr_author_topic_feature_file, Author *authors, int K);

int get_coauthor_ids(int author_id, Paper *paper, vector<int> &coauthor_ids);
int compute_coauthor_score(Paper *paper, Author *authors, int K);
double get_variance(vector<double> &items);

void split_string(const char *str, vector<string> &slices, const char *dem);
int load_confidence_score(FILE *fptr_confidence_score, map<int, map<int, double> >& confidence_score);

#endif

