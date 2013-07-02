#include <cstdio>
#include <cstdlib>
#include "kdd2013.h"
#include <cassert>
#include <list>
#include <set>
#include <map>
#include "util.h"
#include <cstring>
#include <vector>
#include <cmath>
#include <float.h>

using namespace std;

class MyPaper;
class MyAuthor
{
    public:
    set<int> papers;
    set<int> journal_ids;
    set<int> conference_ids;
    set<int> coauthor_ids;
    int has_conference;
    int has_journal;
    int coauthor_count;
    int active_year;
    int earliest_year;
    int latest_year;
    int conference_count;
    int journal_count;
    char affiliation[500];
    char name[500];
    int coauthor_missing_affiliation;
    int missing_name;
    int missing_affiliation;
    int coauthor_missing_name;
    int coauthor_paper_count;
    double coauthor_paper_count_mean;
    double coauthor_paper_count_variance;
    int missing_year;
    int no_journal;
    double log_conference_journal_ratio;
    double log_conference_journal_id_ratio;
    long long int one_step_count;
    MyAuthor():has_conference(0), has_journal(0), missing_name(1), missing_affiliation(1){};
};

class MyPaper
{
    public:
    int journal_id;
    int conference_id;
    set<int> authors;
    int year;
    int author_paper_count;
    double author_paper_count_mean;
    double author_paper_count_variance;
    int is_conference;
    int is_journal;
};

class Journal
{
    public:
    Journal():earliest_year(10000000), latest_year(-1000000){};
    int earliest_year;
    int latest_year;
};

class Conference
{
    public:
    Conference():earliest_year(10000000), latest_year(-1000000){};
    int earliest_year;
    int latest_year;
};

map<int, Conference> conferences;
map<int, Journal> journals;


MyAuthor *authors;
MyPaper *papers;

#ifdef SVM
#define SVM_IDX_INIT(_idx_) int idx = _idx_
#define SVM_IDX(__) fprintf(__, "%d:", idx++)
#else
#define SVM_IDX_INIT(_idx_) ;
#define SVM_IDX(__) ;
#endif
void generate_vector(int author_id, int paper_id, FILE *fptr)
{

    SVM_IDX_INIT(1);



    set<int> journal_ids, conference_ids, paper_ids;
    for (set<int>::iterator it = papers[paper_id].authors.begin(); it != papers[paper_id].authors.end(); it++)
    {
        if ((*it) == author_id) continue;
        journal_ids.insert(authors[*it].journal_ids.begin(), authors[*it].journal_ids.end());
        conference_ids.insert(authors[*it].conference_ids.begin(), authors[*it].conference_ids.end());
        paper_ids.insert(authors[*it].papers.begin(), authors[*it].papers.end());
    }

    int delta_conference_ids = 0, delta_journal_ids = 0, delta_papers = 0;
    int same_conference_count = 0, same_journal_count = 0;

    for (set<int>::iterator it = authors[author_id].journal_ids.begin(); it != authors[author_id].journal_ids.end(); it++)
    {
        if (journal_ids.find(*it) == journal_ids.end())
            delta_journal_ids++;
    }

    for (set<int>::iterator it = authors[author_id].conference_ids.begin(); it != authors[author_id].conference_ids.end(); it++)
        if (conference_ids.find(*it) == conference_ids.end())
            delta_conference_ids++;
    
    int paper_journal_id = papers[paper_id].journal_id;
    int paper_conference_id = papers[paper_id].conference_id;
    int coauthorship_count = 0;
    for (set<int>::iterator it = authors[author_id].papers.begin(); it != authors[author_id].papers.end(); it++)
    {
//        printf("aid:%d pid:%d cid:%d jid:%d\n", author_id, *it, papers[*it].conference_id, papers[*it].journal_id);
        if (paper_ids.find(*it) == paper_ids.end())
            delta_papers++;
        else
            coauthorship_count++;

        if (paper_journal_id > 0 &&  *it != paper_id && papers[*it].journal_id == paper_journal_id )
            same_journal_count++;
        if (paper_conference_id > 0 && *it != paper_id && papers[*it].conference_id == paper_conference_id)
            same_conference_count++;
    }

   
    MyPaper &p = papers[paper_id];
    MyAuthor &a = authors[author_id];

    SVM_IDX(fptr);
    fprintf(fptr, "%d ", coauthorship_count);

    SVM_IDX(fptr);
    if (a.coauthor_ids.size() != 0)
        fprintf(fptr, "%LF ", coauthorship_count/(long double)a.coauthor_ids.size());
    else
        fprintf(fptr, "0 ");

    
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", delta_journal_ids);

    SVM_IDX(fptr);
    fprintf(fptr, "%d ", delta_conference_ids);

    SVM_IDX(fptr);
    fprintf(fptr, "%d ", delta_journal_ids + (int)journals.size());

    SVM_IDX(fptr);
    fprintf(fptr, "%d ", delta_conference_ids + (int)conferences.size());

    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)paper_ids.size());

    SVM_IDX(fptr);
    fprintf(fptr, "%d ", delta_papers + (int)paper_ids.size());

    /*
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", papers[paper_id].year);

    SVM_IDX(fptr);
    fprintf(fptr, "%d ", authors[author_id].active_year - papers[paper_id].year);

    SVM_IDX(fptr);
    fprintf(fptr, "%d ", authors[author_id].active_year);

    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (paper_conference_id > 0 ? 0 : 1));

    SVM_IDX(fptr);
    fprintf(fptr, "%d ",(paper_journal_id  > 0 ? 0 : 1));

    SVM_IDX(fptr);
    fprintf(fptr, "%d ", same_conference_count);

    SVM_IDX(fptr);
    fprintf(fptr, "%d ", same_journal_count);

    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)authors[author_id].papers.size());

    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)papers[paper_id].authors.size());
    */

    
    //1
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.papers.size());

    //2
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.earliest_year);

    //3
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.latest_year);

    //4
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.active_year);

    //5
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.coauthor_ids.size());

    //6
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.coauthor_count);

    //7
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.conference_ids.size());

    //8
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.conference_count);

    //9
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.journal_ids.size());

    //10
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.journal_count);

    //11
    SVM_IDX(fptr);
    fprintf(fptr, "%lf ", a.log_conference_journal_id_ratio);

    //12
    SVM_IDX(fptr);
    fprintf(fptr, "%lf ", a.log_conference_journal_ratio);

    //13
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.coauthor_paper_count);

    //14
    SVM_IDX(fptr);
    fprintf(fptr, "%lf ", a.coauthor_paper_count_mean);

    //15
    SVM_IDX(fptr);
    fprintf(fptr, "%lf ", a.coauthor_paper_count_variance);

    //16
    //SVM_IDX(fptr);
    //fprintf(fptr, "%d ", a.missing_name);

    //17
    //SVM_IDX(fptr);
    //fprintf(fptr, "%d ", a.coauthor_missing_name);

    //18
    //SVM_IDX(fptr);
    //fprintf(fptr, "%d ", a.missing_affiliation);

    //19
    //SVM_IDX(fptr);
    //fprintf(fptr, "%d ", a.coauthor_missing_affiliation);

    //20
    //SVM_IDX(fptr);
    //fprintf(fptr, "%d ", a.has_conference);

    //21
    //SVM_IDX(fptr);
    //fprintf(fptr, "%d ", a.has_journal);

    //22
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)p.authors.size());

    //23
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)p.year);


    SVM_IDX(fptr);
    fprintf(fptr, "%d ", p.author_paper_count);

    //25
    SVM_IDX(fptr);
    fprintf(fptr, "%lf ", p.author_paper_count_mean);

    //26
    SVM_IDX(fptr);
    fprintf(fptr, "%lf ", p.author_paper_count_variance);


    //29
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", same_conference_count);

    //30
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", same_journal_count);

    //31
    SVM_IDX(fptr);
    fprintf(fptr, "%lld ", a.one_step_count);

    //32
    SVM_IDX(fptr);
    if (a.coauthor_ids.size() != 0)
        fprintf(fptr, "%LF ", a.one_step_count / (long double)a.coauthor_ids.size());
    else
        fprintf(fptr, "%lf ", 0.0);

    //33
    SVM_IDX(fptr);
    if (a.coauthor_count != 0)
        fprintf(fptr, "%LF ", (long double)a.one_step_count / a.coauthor_count);
    else
        fprintf(fptr, "%lf ", 0.0);


    //28
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", p.journal_id > 0 ? 1 : 0);

    //27
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", p.conference_id > 0 ? 1 : 0);
}

void generate_author_vector(int author_id, FILE *fptr)
{

    MyAuthor &a = authors[author_id];

    SVM_IDX_INIT(1);

    //1
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.papers.size());

    //2
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.earliest_year);

    //3
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.latest_year);

    //4
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.active_year);

    //5
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.coauthor_ids.size());
    
    //6
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.coauthor_count);

    //7
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.conference_ids.size());

    //8
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.conference_count);

    //9
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.journal_ids.size());

    //10
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)a.journal_count);

    //11
    SVM_IDX(fptr);
    fprintf(fptr, "%LF ", (long double)a.log_conference_journal_id_ratio);
    
    //12
    SVM_IDX(fptr);
    fprintf(fptr, "%lf ", (double)a.log_conference_journal_ratio);

    //13
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", a.coauthor_paper_count);

    //14
    SVM_IDX(fptr);
    fprintf(fptr, "%lf ", a.coauthor_paper_count_mean);

    //15
    SVM_IDX(fptr);
    fprintf(fptr, "%lf ", a.coauthor_paper_count_variance);

    //16
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", a.missing_name);

    //17
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", a.coauthor_missing_name);

    //18
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", a.missing_affiliation);

    //19
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", a.coauthor_missing_affiliation);

    //20
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", a.has_conference);

    //21
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", a.has_journal);

    //22
    SVM_IDX(fptr);
    fprintf(fptr, "%d", a.missing_year);
}

void generate_paper_vector(int paper_id, FILE *fptr)
{
    SVM_IDX_INIT(1001);

    MyPaper &p = papers[paper_id];
    //1001
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", (int)p.authors.size());

    //1002
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", p.year);

    //1003
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", p.author_paper_count);

    //1004
    SVM_IDX(fptr);
    fprintf(fptr, "%lf ", p.author_paper_count_mean);

    //1005
    SVM_IDX(fptr);
    fprintf(fptr, "%lf ", p.author_paper_count_variance);

    //1006
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", p.is_conference);

    //1007
    SVM_IDX(fptr);
    fprintf(fptr, "%d ", p.is_journal);
}


#pragma GCC diagnostic ignored "-Wconversion"
int main(int argc, char *argv[])
{
    if (argc != 12)
    {
        fprintf(stderr, "Usage : %s Author.csv PaperAuthor.csv Paper.csv Train.csv Test.csv train_output test_output test_meta_output paper_feature_output author_feature_output extra\n", argv[0]);
        return -1;
    }

    //initializaion
    authors = new MyAuthor[MAX_AUTHOR_ID_IN_PAPER_AUHTOR + 1];
    papers = new MyPaper[MAX_PAPER_ID_IN_PAPER_AUHTOR + 1];

    char line[1000000];

    int line_count;


    //open files
    FILE *fptr_author;
    fptr_author = fopen(argv[1], "r");
    assert(fptr_author != NULL);

    FILE *fptr_paper_author;
    fptr_paper_author = fopen(argv[2], "r");
    assert(fptr_paper_author != NULL);

    FILE *fptr_paper;
    fptr_paper = fopen(argv[3], "r");
    assert(fptr_paper != NULL);

    FILE *fptr_train;
    fptr_train = fopen(argv[4], "r");
    assert(fptr_train != NULL);
    

    FILE *fptr_test;
    fptr_test = fopen(argv[5], "r");
    assert(fptr_test != NULL);

    FILE *fptr_train_output;
    fptr_train_output = fopen(argv[6], "w+");
    assert(fptr_train_output != NULL);

    FILE *fptr_test_output;
    fptr_test_output = fopen(argv[7], "w+");
    assert(fptr_test_output != NULL);

    FILE *fptr_test_meta_output;
    fptr_test_meta_output = fopen(argv[8], "w+");
    assert(fptr_test_meta_output != NULL);

    FILE *fptr_paper_feature_output;
    fptr_paper_feature_output = fopen(argv[9], "w+");
    assert(fptr_paper_feature_output != NULL);

    FILE *fptr_author_feature_output;
    fptr_author_feature_output = fopen(argv[10], "w+");
    assert(fptr_author_feature_output != NULL);

    vector<TrainRecord> train_csv;
    assert(load_train_csv(fptr_train, train_csv) == 0);
    vector<TestRecord> test_csv;
    assert(load_test_csv(fptr_test, test_csv) == 0);
    printf("%d\n", (int)test_csv.size());

    FILE *fptr_one_step;
    fptr_one_step = fopen(argv[11], "r");
    assert(fptr_one_step != NULL);

    while(fgets(line, sizeof(line), fptr_one_step))
    {
        char *cptr = strtok(line, " \n");
        int author_id;
        assert(sscanf(cptr, "%d", &author_id) == 1);
        int  times;
        cptr = strtok(NULL, "\n ");
        MyAuthor &a = authors[author_id];
        a.one_step_count = 0;
        while (cptr)
        {
            assert(sscanf(cptr, "%d:%d", &author_id, &times) == 2);
            a.one_step_count  += times;
            cptr = strtok(NULL, "\n ");
        }
    }



    //read Author
    puts("Loading Author.csv");
    line_count = 0;
    assert(fgets(line, sizeof(line), fptr_author));

    while(fgets(line, sizeof(line), fptr_author))
    {
        char tmp[1000];
        int offset = 0;
        int author_id; 
        MyAuthor &a = authors[author_id];

        offset += get_section(line + offset, tmp, NULL);
        assert(sscanf(tmp, "%d", &author_id) == 1);

        offset += get_section(line + offset, authors[author_id].name, NULL);

        offset += get_section(line + offset, authors[author_id].affiliation, NULL);

        if (strlen(a.name) == 0)
            a.missing_name = 1;
        else
            a.missing_name = 0;

        if (strlen(a.affiliation) == 0)
            a.missing_affiliation = 1;
        else
            a.missing_affiliation = 0;
    }

    //read MyPaperMyAuthor
    puts("Loading PaperAuthor.csv ...");
    line_count = 0;
    assert(fgets(line, sizeof(line), fptr_paper_author));

    while(fgets(line, sizeof(line), fptr_paper_author))
    {
        int paper_id, author_id;
        assert(sscanf(line, "%d,%d", &paper_id, &author_id) == 2);
        papers[paper_id].authors.insert(author_id);
        authors[author_id].papers.insert(paper_id);
        line_count++;
    }
    fprintf(stdout, "PaperAuthor loaded (total %d lines)\n", line_count);

    fclose(fptr_paper_author);
    
    //read MyPaper
    fprintf(stdout, "loading Paper.csv ...\n");
    line_count = 0;
    assert(fgets(line, sizeof(line), fptr_paper));

    while(fgets(line, sizeof(line), fptr_paper))
    {
        int offset = 0;
        int paper_id;
        int year;
        int conference_id;
        int journal_id;
        char content[1000];

        offset += get_section(line + offset, content, NULL);
        assert(sscanf(content, "%d", &paper_id) == 1);
        
        offset += get_section(line + offset, content, NULL);
        
        offset += get_section(line + offset, content, NULL);
        assert(sscanf(content, "%d", &year) == 1);

        offset += get_section(line + offset, content, NULL);
        assert(sscanf(content, "%d", &conference_id) == 1);

        offset += get_section(line + offset, content, NULL);
        assert(sscanf(content, "%d", &journal_id) == 1);
        
        assert(paper_id <= MAX_PAPER_ID_IN_PAPER_AUHTOR);
        papers[paper_id].journal_id = journal_id;
        papers[paper_id].conference_id = conference_id;
        papers[paper_id].year = year;

        line_count++;
    }

    printf("MyPaper.csv loaded! (%d lines)\n", line_count);
    fclose(fptr_paper);

    //compute journal and conference ids of author 
    
    printf("computing journal and conference ids for each users\n");
    for (int i = 0; i <= MAX_PAPER_ID_IN_PAPER_AUHTOR; i++)
    {
        
        set<int> &lst = papers[i].authors;
        MyPaper &p = papers[i];
        p.author_paper_count = 0;
        for (set<int>::iterator it = lst.begin(); it != lst.end(); it++)
        {
            if (papers[i].journal_id > 0)
                authors[*it].journal_ids.insert(papers[i].journal_id);
            if (papers[i].conference_id > 0)
                authors[*it].conference_ids.insert(papers[i].conference_id);
            p.author_paper_count += (int)authors[*it].papers.size();
        }
        p.author_paper_count_mean = p.author_paper_count / (double)p.authors.size();

        for (set<int>::iterator it = lst.begin(); it != lst.end(); it++)
            p.author_paper_count_variance += pow((int)authors[*it].papers.size() - p.author_paper_count_mean, 2);
        
        p.author_paper_count_variance /= (double)p.authors.size();
        p.author_paper_count_variance = sqrt(p.author_paper_count_variance);


        if (lst.size() != 0)
        {
            if (p.conference_id > 0 && conferences[p.conference_id].latest_year < p.year)
                conferences[p.conference_id].latest_year = p.year;
            if (p.conference_id > 0 && conferences[p.conference_id].earliest_year > p.year)
                conferences[p.conference_id].earliest_year = p.year;
            
            if (p.journal_id > 0 && journals[p.journal_id].latest_year < p.year)
                journals[p.journal_id].latest_year = p.year;
            if (p.journal_id > 0 && journals[p.journal_id].earliest_year > p.year)
                journals[p.journal_id].earliest_year = p.year;
            

        }
            
    }
    fprintf(stdout, "done!\n");


    //compute active year 
    printf("computing author informations  ...\n");
    vector<int> author_missing_year;
    vector<int> author_no_journal;
    vector<int> author_no_conference;
    
    double max_log_conference_journal_ratio = 0.0;
    double max_log_conference_journal_id_ratio = 0.0;
    double min_log_conference_journal_ratio = DBL_MAX;
    double min_log_conference_journal_id_ratio = DBL_MAX;
    double mean_active_year = 0;
    double mean_earliest_year = 0;
    double mean_latest_year = 0;
    int year_count = 0;
    for (int i = 0; i <= MAX_AUTHOR_ID_IN_PAPER_AUHTOR; i++)
    {
        MyAuthor &a = authors[i];
        a.earliest_year = 10000000;
        a.latest_year = -1000000;
        a.missing_year = 1;
        a.conference_count = 0;
        a.journal_count = 0;
        a.coauthor_paper_count = 0;
        a.has_conference = 0;
        a.has_journal = 0;
        a.coauthor_count = 0;


        if (a.papers.size() != 0)
        {
            for (set<int>::iterator it = authors[i].papers.begin(); it != authors[i].papers.end(); it++)
            {
                MyPaper &p = papers[*it];
                a.coauthor_count += (int)p.authors.size() - 1;
                if (p.year != 0 && p.year < a.earliest_year)
                {
                    a.earliest_year = p.year;
                    a.missing_year = 0;
                }
                if (p.year != 0 && p.year > a.latest_year)
                {
                    a.latest_year = p.year;
                    a.missing_year = 0;
                }

                if (p.conference_id > 0)
                {
                    a.conference_count++;
                    a.has_conference = 1;
                }
                if (p.journal_id > 0)
                {
                    a.journal_count++;
                    a.has_journal = 1;
                }

                a.coauthor_ids.insert(p.authors.begin(), p.authors.end());
            }

            a.coauthor_ids.erase(i);

            if (a.coauthor_ids.size() != 0)
            {

                for (set<int>::iterator it = a.coauthor_ids.begin(), end = a.coauthor_ids.end(); it != end; it++)
                {
                    if (authors[*it].missing_name) a.coauthor_missing_name = 1;
                    if (authors[*it].missing_affiliation) a.coauthor_missing_affiliation = 1;
                    a.coauthor_paper_count += (int)authors[*it].papers.size();
                }


                a.coauthor_paper_count_mean = a.coauthor_paper_count / (double)a.coauthor_ids.size();

                a.coauthor_paper_count_variance = 0.0;

                for (set<int>::iterator it = a.coauthor_ids.begin(), end = a.coauthor_ids.end(); it != end; it++)
                {
                    a.coauthor_paper_count_variance += pow((double)authors[*it].papers.size() - a.coauthor_paper_count_mean, 2);
                    assert(a.coauthor_paper_count_variance >= 0);
                }

                a.coauthor_paper_count_variance /= (double) a.coauthor_ids.size();
                assert(a.coauthor_paper_count_variance >= 0);
                a.coauthor_paper_count_variance = sqrt(a.coauthor_paper_count_variance);
            }
            else
                a.coauthor_paper_count_variance = 0.0;

            a.active_year = a.latest_year - a.earliest_year;
        }

        if (a.journal_count == 0)
        {
            a.no_journal = 1;
            author_no_journal.push_back(i);
        }
        else if (a.conference_count == 0)
        {
            author_no_conference.push_back(i);
        }
        else
        {
            a.log_conference_journal_ratio = log(a.conference_count / (double) a.journal_count);
            if (a.log_conference_journal_ratio > max_log_conference_journal_ratio)
                max_log_conference_journal_ratio = a.log_conference_journal_ratio;
            if (a.log_conference_journal_ratio < min_log_conference_journal_ratio)
                min_log_conference_journal_ratio = a.log_conference_journal_ratio;
            a.log_conference_journal_id_ratio = log(a.conference_ids.size() / (double) a.journal_ids.size());
            if (a.log_conference_journal_id_ratio > max_log_conference_journal_id_ratio)
                max_log_conference_journal_id_ratio = a.log_conference_journal_id_ratio;
            if (a.log_conference_journal_id_ratio < min_log_conference_journal_id_ratio)
                min_log_conference_journal_id_ratio = a.log_conference_journal_id_ratio;
        }

        if (a.missing_year)
        {
            author_missing_year.push_back(i);
        }
        else
        {
            mean_active_year += a.active_year;
            mean_earliest_year += a.earliest_year;
            mean_latest_year += a.latest_year;
            year_count++;
        }

    }

    mean_active_year /= year_count;
    mean_earliest_year /= year_count;
    mean_latest_year /= year_count;
    for (int i = 0, d = (int)author_no_journal.size(); i < d; i++)
    {
        MyAuthor &a = authors[author_no_journal[i]];
        a.log_conference_journal_ratio = max_log_conference_journal_ratio + 1;
        a.log_conference_journal_id_ratio = max_log_conference_journal_id_ratio + 1;
    }
    for (int i = 0, d = (int)author_missing_year.size(); i < d; i++)
    {
        MyAuthor &a = authors[author_missing_year[i]];
        a.active_year = (int)mean_active_year;
        a.latest_year = (int)mean_latest_year;
        a.earliest_year = (int)mean_earliest_year;
    }
    for (int i = 0, d = (int)author_no_conference.size(); i < d; i++)
    {
        MyAuthor &a = authors[author_no_conference[i]];
        a.log_conference_journal_ratio = min_log_conference_journal_ratio - 1;
        a.log_conference_journal_id_ratio = min_log_conference_journal_id_ratio - 1;
    }


    fprintf(stdout, "done!\n");

    //    fprintf(stdoit, "computing jounrnal/conference information");



    line_count = 0;
    fprintf(stdout, "loading Train.csv ...\n");
        

    for (vector<TrainRecord>::iterator it = train_csv.begin(), end = train_csv.end(); it != end; it++)
    {
        int author_id = it->author_id;

        for (vector<int>::iterator it2 = it->confirmed_paper_ids.begin(), end2 = it->confirmed_paper_ids.end(); it2 != end2; it2++)
        {
            fprintf(fptr_train_output, "1 ");
            generate_vector(author_id, *it2, fptr_train_output);
            fputs("\n", fptr_train_output);
        }
        for (vector<int>::iterator it2 = it->deleted_paper_ids.begin(), end2 = it->deleted_paper_ids.end(); it2 != end2; it2++)
        {
            fprintf(fptr_train_output, "-1 ");
            generate_vector(author_id, *it2, fptr_train_output);
            fputs("\n", fptr_train_output);
        }
    }
    fprintf(stdout, "Train.csv loaded...\n");
    
    fclose(fptr_train);
    fclose(fptr_train_output);

    fprintf(stderr, "Loading Valid.csv\n");



    

    for (vector<TestRecord>::iterator it = test_csv.begin(), end = test_csv.end(); it != end; it++)
    {

        int author_id = it->author_id;
        for (vector<int>::iterator it2 = it->paper_ids.begin(), end2 = it->paper_ids.end(); it2 != end2; it2++)
        {
            fprintf(fptr_test_output, "1 ");
            generate_vector(author_id, *it2, fptr_test_output);
            fprintf(fptr_test_output, "\n");
        }
    }


    fclose(fptr_test);
    fclose(fptr_test_output);
    fclose(fptr_test_meta_output);

    fprintf(stdout, "Valid.csv loaded\n");

    fprintf(stdout, "Generating paper feature vector...\n");
    for (int i = 0; i <= MAX_PAPER_ID_IN_PAPER_AUHTOR; i++)
    {
        if (papers[i].authors.size() != 0)
        {
            fprintf(fptr_paper_feature_output, "%d ", i);
            generate_paper_vector(i, fptr_paper_feature_output);
            fputs("\n", fptr_paper_feature_output);
        }
    }
    fclose(fptr_paper_feature_output);
    fprintf(stdout, "done!\n");

    fprintf(stdout, "Generating author feature vector...\n");
    for (int i = 0; i <= MAX_AUTHOR_ID_IN_PAPER_AUHTOR; i++)
    {
        if (authors[i].papers.size() != 0)
        {
            fprintf(fptr_author_feature_output, "%d ", i);
            generate_author_vector(i, fptr_author_feature_output);
            fputs("\n", fptr_author_feature_output);
        }
    }
    fprintf(stdout, "done!\n");
    fclose(fptr_author_feature_output);
    return 0;
}


#pragma GCC diagnostic warning "-Wconversion"
