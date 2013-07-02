#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <vector>
#include "kdd2013.h"
#include "util.h"
using namespace std;

class MyAuthor;
class MyPaper
{
    public:
        int paper_id;
        int year;
        int conference_id;
        int journal_id;
        int word_count;
        int *topic_count;
        double *topic_feature;
        int topic;
        vector<MyAuthor*> authors;
};
class Conference
{
    public:
        int conference_id;
        double *topic_feature;
        vector<MyPaper*> papers;
        int topic;
};

class Journal
{
    public :
        int journal_id;
        double *topic_feature;
        vector<MyPaper*> papers;
        int topic;
};

class MyAuthor
{
    public:
        int author_id;
        int topic;
        double *topic_feature;
        vector<MyPaper*> papers;
        int valid_paper_count;
};


MyPaper papers[MAX_PAPER_ID_IN_PAPER_AUHTOR + 1];
Conference conferences[MAX_CONFERENCE_ID_IN_PAPER + 1];
Journal journals[MAX_JOURNAL_ID_IN_PAPER + 1];
MyAuthor authors[MAX_AUTHOR_ID_IN_PAPER_AUHTOR + 1];


int main(int argc, char *argv[])
{
    if (argc != 8)
    {
        fprintf(stderr, "Usage : %s MyPaper.csv MyPaperMyAuthor.csv MyPaperTopic MyAuthorTopicFeature MyPaperTopicFeature ConferenceTopicFeature JournalTopicFeature\n", argv[0]);
        return -1;
    }

    FILE *fptr_paper;
    fptr_paper = fopen(argv[1], "r");
    assert(fptr_paper != NULL);

    FILE *fptr_paper_author;
    fptr_paper_author = fopen(argv[2], "r");
    assert(fptr_paper_author != NULL);

    FILE *fptr_paper_topic;
    fptr_paper_topic = fopen(argv[3], "r");
    assert(fptr_paper_topic != NULL);

    FILE *fptr_author_topic_feature;
    fptr_author_topic_feature = fopen(argv[4], "w+");
    assert(fptr_author_topic_feature != NULL);

    FILE *fptr_paper_topic_feature;
    fptr_paper_topic_feature = fopen(argv[5], "w+");
    assert(fptr_paper_topic_feature != NULL);

    FILE *fptr_conference_topic_feature;
    fptr_conference_topic_feature = fopen(argv[6], "w+");
    assert(fptr_conference_topic_feature != NULL);

    FILE *fptr_journal_topic_feature;
    fptr_journal_topic_feature = fopen(argv[7], "w+");
    assert(fptr_journal_topic_feature != NULL);

    int K;

    vector<MyPaper*> active_papers;
    vector<Conference*> active_conferences;
    vector<Journal*> active_journals;
    vector<MyAuthor*> active_authors;

    char line[100000];
    int line_count;
    int max_journal_id = -1;
    int max_conference_id = -1;

    line_count = 0;

    assert(fgets(line, sizeof(line), fptr_paper));

    while (fgets(line, sizeof(line), fptr_paper))
    {
        char tmp[10000];
        int paper_id;
        int year;
        int conference_id;
        int journal_id;
        int offset = 0;

        offset += get_section(line + offset, tmp, NULL);
        assert(sscanf(tmp, "%d", &paper_id) == 1);

        offset += get_section(line + offset, tmp, NULL);

        offset += get_section(line + offset, tmp, NULL);
        assert(sscanf(tmp, "%d", &year) == 1);

        offset += get_section(line + offset, tmp, NULL);
        assert(sscanf(tmp, "%d", &conference_id) == 1);
        
        offset += get_section(line + offset, tmp, NULL);
        assert(sscanf(tmp, "%d", &journal_id) == 1);


        if (journal_id > max_journal_id)
            max_journal_id = journal_id;

        if (conference_id > max_conference_id)
            max_conference_id = conference_id;

        papers[paper_id].paper_id = paper_id;
        papers[paper_id].year = year;
        papers[paper_id].conference_id = conference_id;
        papers[paper_id].journal_id = journal_id;

        active_papers.push_back(papers + paper_id);
        if (conference_id > 0)
        {
            if (conferences[conference_id].conference_id == 0)
            {
                conferences[conference_id].conference_id = conference_id;
                active_conferences.push_back(conferences + conference_id);   
            }
            conferences[conference_id].papers.push_back(papers + paper_id);
        }
        
        
        if (journal_id > 0)
        {
            if (journals[journal_id].journal_id == 0)
            {
                journals[journal_id].journal_id = journal_id;
                active_journals.push_back(journals + journal_id);
            }

            journals[journal_id].papers.push_back(papers + paper_id);
        }
    
        line_count++;
    }
    fclose(fptr_paper);

//    printf("max_conference_id = %d\n", max_conference_id);
//    printf("max_journal_id = %d\n", max_journal_id);
    fprintf(stdout, "MyPaper.csv loaded ... (%d)\n", line_count);
//    fprintf(stdout, "1842:%d\n", (int)conferences[1842].papers.size());


    line_count = 0;
    assert(fgets(line, sizeof(line), fptr_paper_author));

    while(fgets(line, sizeof(line), fptr_paper_author))
    {
        //int offset = 0;
        int paper_id;
        int author_id;

        assert(sscanf(line, "%d,%d", &paper_id, &author_id) == 2);

        MyAuthor &a = authors[author_id];
        MyPaper &p = papers[paper_id];

        if (a.author_id == 0)
        {
            a.author_id = author_id;
            active_authors.push_back(authors + author_id);
        }
        a.papers.push_back(papers + paper_id);

        if (p.paper_id == 0)
        {
            p.paper_id = paper_id;
            active_papers.push_back(papers + paper_id);
        }
        p.authors.push_back(authors + author_id);

        line_count++;
    }

    fprintf(stderr, "MyPaperMyAuthor.csv loaded ... (%d)\n", line_count);
    fclose(fptr_paper_author);
        
    fprintf(stdout, "1842:%d\n", (int)conferences[1842].papers.size());

    assert(fscanf(fptr_paper_topic, "%d", &K) == 1);

    unsigned long long int *global_topic_count = new unsigned long long int[K];
    unsigned long long int word_count = 0;
    for (int i = 0; i < K; i++)
        global_topic_count[i] = 0;

    int paper_id;
    while(fscanf(fptr_paper_topic, "%d", &paper_id) == 1)
    {
        MyPaper &p = papers[paper_id];
        assert(p.paper_id != 0);

        p.topic_count = new int[K];
        p.word_count = 0;
        for (int i = 0; i < K; i++)
        {
            assert(fscanf(fptr_paper_topic, "%d", p.topic_count + i) == 1);
            if (p.topic_count[i] != 0)
            {
                global_topic_count[i] += p.topic_count[i];
                word_count += p.topic_count[i];
                p.word_count += p.topic_count[i];
//                p.word_count += p.topic_count[i] * p.topic_count[i];
            }
        }
    }

    fclose(fptr_paper_topic);

    double *global_paper_topic_feature = new double[K];
    for (int i = 0; i < K; i++)
        global_paper_topic_feature[i] = (double)((long double)global_topic_count[i] / word_count);

    double assert_sum = 0.0;
    for (int i = 0; i < K; i++)
        assert_sum += global_paper_topic_feature[i];

    assert((assert_sum > 1.0 ? assert_sum - 1.0 : 1.0 - assert_sum) < 1e-7);



    for (vector<MyPaper*>::iterator it = active_papers.begin(), end = active_papers.end(); it != end; it++)
    {
        MyPaper *p = *it;
        if (p->word_count == 0)
        {
            fprintf(stdout, "paper %d has no title\n", p->paper_id);
            continue;
        }
        p->topic_feature = new double[K];
        int max_count = 0;
        for (int i = 0; i < K; i++)
        {
            if (max_count < p->topic_count[i])
            {
                max_count = p->topic_count[i];
                p->topic = i;
            }
            p->topic_feature[i] = (double)p->topic_count[i]  / p->word_count;
//            p->topic_feature[i] = (double)p->topic_count[i] * p->topic_count[i]  / p->word_count;
        }
    }
#define NO_SMOOTH
#ifndef NO_SMOOTH
    double lambda = 0.1;
    for (vector<MyPaper*>::iterator it = active_papers.begin(), end = active_papers.end(); it != end; it++)
    {
        MyPaper *p = *it;
        assert(p->paper_id != 0);
    
        if (p->topic_feature != NULL)
        {
            for (int i = 0; i < K; i++)
                p->topic_feature[i] = (1 - lambda) * p->topic_feature[i] + lambda * global_paper_topic_feature[i];
        }
    }
#endif

    for (vector<MyAuthor*>::iterator it = active_authors.begin(), end = active_authors.end(); it != end; it++)
    {
        MyAuthor *a = *it;
        assert(a->author_id != 0);

        int featured_paper_count = 0;
        assert(a->papers.size() != 0);

        a->topic_feature = new double[K];
        for (int i = 0; i < K; i++)
            a->topic_feature[i] = 0.0;

        for (vector<MyPaper*>::iterator it2 = a->papers.begin(), end2 = a->papers.end(); it2 != end2; it2++)
        {
            MyPaper *p = *it2;
            if (p->topic_feature != NULL)
            {
                for (int i = 0; i < K; i++)
                    a->topic_feature[i] += p->topic_feature[i];

                featured_paper_count++;
            }
        }
        if (featured_paper_count == 0)
        {
            fprintf(stdout, "Warning author %d(%d) has no featured paper!\n", a->author_id, (int)a->papers.size());
        }
        else
        {
            double max_topic = 0.0;
            for (int i = 0; i < K; i++)
            {
                if (a->topic_feature[i] > max_topic)
                {
                    max_topic = a->topic_feature[i];
                    a->topic = i;
                }
                a->topic_feature[i] /= featured_paper_count;
            }
        }
        a->valid_paper_count = featured_paper_count;
    }

    for (vector<Conference*>::iterator it = active_conferences.begin(), end = active_conferences.end(); it != end; it++)
    {
        Conference *c = *it;
        assert(c->conference_id != 0);

        int featured_paper_count = 0;
        assert(c->papers.size() != 0);

        c->topic_feature = new double[K];
        for (int i = 0; i < K; i++)
            c->topic_feature[i] = 0.0;

        for (vector<MyPaper*>::iterator it2 = c->papers.begin(), end2 = c->papers.end(); it2 != end2; it2++)
        {
            MyPaper *p = *it2;
            if (p->topic_feature != NULL)
            {
                for (int i = 0; i < K; i++)
                    c->topic_feature[i] += p->topic_feature[i];

                featured_paper_count++;
            }
        }
        if (featured_paper_count == 0)
        {
            fprintf(stdout, "Warning conference %d(%d) has no featured paper\n", c->conference_id, (int)c->papers.size());
        }
        else
        {
            for (int i = 0; i < K; i++)
                c->topic_feature[i] /= featured_paper_count;
        }
    }

    for (vector<Journal*>::iterator it = active_journals.begin(), end = active_journals.end(); it != end; it++)
    {
        Journal *j = *it;
        assert(j->journal_id != 0);

        int featured_paper_count = 0;
        assert(j->papers.size() != 0);

        j->topic_feature = new double[K];
        for (int i = 0; i < K; i++)
            j->topic_feature[i] = 0.0;

        for (vector<MyPaper*>::iterator it2 = j->papers.begin(), end2 = j->papers.end(); it2 != end2; it2++)
        {
            MyPaper *p = *it2;
            if (p->topic_feature != NULL)
            {
                for (int i = 0; i < K; i++)
                    j->topic_feature[i] += p->topic_feature[i];

                featured_paper_count++;
            }
        }
        if (featured_paper_count == 0)
        {
            fprintf(stdout, "Warning journal %d (%d) has no featured paper\n", j->journal_id, (int)j->papers.size());
        }
        else
        {
            for (int i = 0; i < K; i++)
                j->topic_feature[i] /= featured_paper_count;
        }
    }

    int global_topic = 0;
    double global_max_count = 0.0;
    for (int i = 0; i < K; i++)
    {
        if (global_paper_topic_feature[i] > global_max_count)
        {
            global_max_count = global_paper_topic_feature[i];
            global_topic = i;
        }
    }


    fprintf(stdout, "outputing author topic feature\n");
    //output author topic feature
    for (vector<MyAuthor*>::iterator it = active_authors.begin(), end = active_authors.end(); it != end; it++)
    {
        MyAuthor *a = *it;
        assert(a->author_id != 0);

        fprintf(fptr_author_topic_feature, "%d", a->author_id);

        fprintf(fptr_author_topic_feature, " %d", a->valid_paper_count);

        if (a->topic_feature == NULL)
        {
            fprintf(fptr_author_topic_feature, " 1");
            fprintf(fptr_author_topic_feature, " %d", global_topic);
            for (int i = 0; i < K; i++)
                fprintf(fptr_author_topic_feature, " %.5lf", global_paper_topic_feature[i]);
        }
        else
        {
            fprintf(fptr_author_topic_feature, " 0");
            fprintf(fptr_author_topic_feature, " %d", a->topic);
            for (int i = 0; i < K; i++)
                fprintf(fptr_author_topic_feature, " %.5lf", a->topic_feature[i]);
        }
        fprintf(fptr_author_topic_feature, "\n");
    }
    fprintf(stdout, "done\n");
    fclose(fptr_author_topic_feature);

    //output paper topic feature
    fprintf(stdout, "outputing paper topic feature\n");
    for (vector<MyPaper*>::iterator it = active_papers.begin(), end = active_papers.end(); it != end; it++)
    {
        MyPaper *p = *it;
        assert(p->paper_id != 0);

        fprintf(fptr_paper_topic_feature, "%d", p->paper_id);
        fprintf(fptr_paper_topic_feature, " %d %d", p->conference_id, p->journal_id);

        if (p->topic_feature == NULL)
        {
            fprintf(fptr_paper_topic_feature, " 1");
            fprintf(fptr_paper_topic_feature, " %d", global_topic);
            for (int i = 0; i < K; i++)
                fprintf(fptr_paper_topic_feature, " %.5lf", global_paper_topic_feature[i]);
        }
        else
        {
            fprintf(fptr_paper_topic_feature, " 0");
            fprintf(fptr_paper_topic_feature, " %d", p->topic);
            for (int i = 0; i < K; i++)
                fprintf(fptr_paper_topic_feature, " %.5lf", p->topic_feature[i]);
        }
        fprintf(fptr_paper_topic_feature, "\n");
    }

    fclose(fptr_paper_topic_feature);
    fprintf(stdout, "done\n");

    //output conference topic feature
    fprintf(stdout, "outputing conference topic feature\n");
    for (vector<Conference*>::iterator it = active_conferences.begin(), end = active_conferences.end(); it != end; it++)
    {
        Conference *c = *it;
        assert(c->conference_id != 0);

        fprintf(fptr_conference_topic_feature, "%d", c->conference_id);
        
        if (c->topic_feature == NULL)
        {
            for (int i = 0; i < K; i++)
                fprintf(fptr_conference_topic_feature, " %.5lf", global_paper_topic_feature[i]);
        }
        else
        {
            for (int i = 0; i < K; i++)
                fprintf(fptr_conference_topic_feature, " %.5lf", c->topic_feature[i]);
        }
        fprintf(fptr_conference_topic_feature, "\n");
    }
    fclose(fptr_conference_topic_feature);
    fprintf(stdout, "done\n");
    
    //output journal topic feature
    fprintf(stdout, "outputing journal topic feature\n");
    for (vector<Journal*>::iterator it = active_journals.begin(), end = active_journals.end(); it != end; it++)
    {
        Journal *j = *it;
        assert(j->journal_id != 0);

        fprintf(fptr_journal_topic_feature, "%d", j->journal_id);

        if (j->topic_feature == NULL)
        {
          //  fprintf(fptr_journal_topic_feature, " %d", 1);
            for (int i = 0; i < K; i++)
                fprintf(fptr_journal_topic_feature, " %.5lf", global_paper_topic_feature[i]);
        }
        else
        {
        //    fprintf(fptr_journal_topic_feature, " %d", 0);
            for (int i = 0; i < K; i++)
                fprintf(fptr_journal_topic_feature, " %.5lf", j->topic_feature[i]);
        }
        fprintf(fptr_journal_topic_feature, "\n");
    }
    fclose(fptr_journal_topic_feature);
    fprintf(stdout, "done\n");
    return 0;
}

