#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <vector>
#include <cmath>

using namespace std;

#include "kdd2013.h"

class Author
{
    public:
        int author_id;
        int feature_missing;
        double *topic_feature;
        int topic;
        int valid_paper_count;
};

class Paper
{
    public:
        int paper_id;
        int conference_id;
        int journal_id;
        int feature_missing;
        double *topic_feature;
        int topic;
};

class Journal
{
    public:
        int journal_id;
        double *topic_feature;
};

class Conference
{
    public:
        int conference_id;
        double *topic_feature;
};


Author authors[MAX_AUTHOR_ID_IN_PAPER_AUHTOR + 1];
Paper papers[MAX_PAPER_ID_IN_PAPER_AUHTOR + 1];
Conference conferences[MAX_CONFERENCE_ID_IN_PAPER + 1];
Journal journals[MAX_JOURNAL_ID_IN_PAPER + 1];


inline double get_sim(Paper *p, Author *a, int K)
{
    
    if (a->valid_paper_count == 0)return 0;
    double s = 0.0;
    for (int i = 0; i < K; i++)
    {
//        s += (a->topic_feature[i] - p->topic_feature[i] / a->valid_paper_count ) * p->topic_feature[i];
//        s += a->topic_feature[i] * p->topic_feature[i];
        s += pow(a->topic_feature[i] - p->topic_feature[i], 2);
    }
    return s;
}
        
inline double get_journal_sim(Paper *p, Journal *j, int K)
{
    double s = 0.0;
    for (int i = 0; i < K; i++)
    {
      s += j->topic_feature[i] * p->topic_feature[i];
//        s += pow(j->topic_feature[i] - p->topic_feature[i], 2);
    }
    return s;
}

inline double get_conference_sim(Paper *p, Conference *c, int K)
{
    double s = 0.0;
    for (int i = 0; i < K; i++)
    {
        s += c->topic_feature[i] * p->topic_feature[i];
//        s += pow(c->topic_feature[i] - p->topic_feature[i], 2);
    }
    return s;
}

int main(int argc, char *argv[])
{
    if (argc != 10)
    {
        fprintf(stderr, "Usage : %s K AuthorTopicFeature PaperTopicFeature ConferenceTopicFeature JournalTopicFeature Train.csv Valid.csv TrainOutput ValidOutput\n", argv[0]);
        return -1;
    }

    int K;
    assert(sscanf(argv[1], "%d", &K) == 1);

    FILE *fptr_author_topic_feature;
    fptr_author_topic_feature = fopen(argv[2], "r");
    assert(fptr_author_topic_feature != NULL);

    FILE *fptr_paper_topic_feature;
    fptr_paper_topic_feature = fopen(argv[3], "r");
    assert(fptr_paper_topic_feature != NULL);

    FILE *fptr_train;
    fptr_train = fopen(argv[6], "r");
    assert(fptr_train != NULL);

    FILE *fptr_valid;
    fptr_valid = fopen(argv[7], "r");
    assert(fptr_valid != NULL);

    FILE *fptr_conference_topic_feature;
    fptr_conference_topic_feature = fopen(argv[4], "r");
    assert(fptr_conference_topic_feature != NULL);

    FILE *fptr_journal_topic_feature;
    fptr_journal_topic_feature = fopen(argv[5], "r");
    assert(fptr_journal_topic_feature != NULL);

    FILE *fptr_train_output;
    fptr_train_output = fopen(argv[8], "w+");
    assert(fptr_train_output != NULL);

    FILE *fptr_valid_output;
    fptr_valid_output = fopen(argv[9], "w+");
    assert(fptr_valid_output != NULL);

    //load author topic feature
    fprintf(stdout, "loading author topic feature...\n");
    while (1)
    {
        int author_id;
        if (fscanf(fptr_author_topic_feature, "%d", &author_id) != 1)
            break;
        
        Author *a = authors + author_id;
        a->author_id = author_id;

        a->topic_feature = new double[K];

        assert(fscanf(fptr_author_topic_feature, "%d", &a->valid_paper_count) == 1);
        assert(fscanf(fptr_author_topic_feature, "%d", &a->feature_missing) == 1);
        assert(fscanf(fptr_author_topic_feature, "%d", &a->topic) == 1);

        for (int i = 0; i < K; i++)
            assert(fscanf(fptr_author_topic_feature, "%lf", a->topic_feature + i) == 1);
    }
    fclose(fptr_author_topic_feature);
    fprintf(stdout, "done\n");

    //load paper topic feature
    fprintf(stdout, "loading paper topic feature...\n");
    while(1)
    {
        int paper_id;
        if (fscanf(fptr_paper_topic_feature, "%d", &paper_id) != 1)
            break;

        Paper *p = papers + paper_id;
        p->paper_id = paper_id;

        assert(fscanf(fptr_paper_topic_feature, "%d", &p->conference_id) == 1);
        assert(fscanf(fptr_paper_topic_feature, "%d", &p->journal_id) == 1);
        assert(fscanf(fptr_paper_topic_feature, "%d", &p->feature_missing) == 1);
        assert(fscanf(fptr_paper_topic_feature, "%d", &p->topic) == 1);
              
        p->topic_feature = new double[K];

        for (int i = 0; i < K; i++)
            assert(fscanf(fptr_paper_topic_feature, "%lf", p->topic_feature + i) == 1);

    }
    fclose(fptr_paper_topic_feature);
    fprintf(stdout, "done\n");

    //load conference topic feature
    fprintf(stdout, "loading conference topic feature...\n");
    while (1)
    {
        int conference_id;
        if (fscanf(fptr_conference_topic_feature, "%d", &conference_id) != 1)
            break;

        Conference *c = conferences + conference_id;
        c->conference_id = conference_id;

        c->topic_feature = new double[K];
        for (int i = 0; i < K; i++)
            assert(fscanf(fptr_conference_topic_feature, "%lf", c->topic_feature + i) == 1);
    }
    fprintf(stdout, "done!\n");


    //load journal topic feature
    fprintf(stdout, "loading journal topic feature..\n");
    while (1)
    {
        int journal_id;
        if (fscanf(fptr_journal_topic_feature, "%d", &journal_id) != 1)
            break;

        Journal *j = journals + journal_id;
        j->journal_id = journal_id;

        j->topic_feature = new double[K];
        for (int i = 0; i < K; i++)
            assert(fscanf(fptr_journal_topic_feature, "%lf", j->topic_feature + i) == 1);
    }
    fprintf(stdout, "done!\n");

    char line[1000000];
    fprintf(stdout, "loading Train.csv ...\n");
        
    assert(fgets(line, sizeof(line), fptr_train));

    while (fgets(line, sizeof(line), fptr_train))
    {
        char cp[100000];
        char dp[100000];
        int paper_id;
        int author_id;
        vector<int> label;
        vector<double> scores;
        vector<int> paper_feature_missing;
        vector<int> paper_topic;
        vector<double> journal_sim;
        vector<int> is_journal;
        vector<double> conference_sim;
        vector<int> is_conference;

        char *cptr_s, *cptr_p;
#ifdef DEBUG
//        printf("%s", line);
#endif
        cptr_s = strtok(line, ",");
        assert(sscanf(cptr_s, "%d", &author_id) == 1);
        Author *a = authors + author_id;
#ifdef DEBUG
        printf("%d,", author_id);
#endif
        cptr_s = strtok(NULL, ",");
        assert(cptr_s != NULL);
        strcpy(cp, cptr_s);

        cptr_s = strtok(NULL, ",\n");
        //assert(cptr_s != NULL);
        if (cptr_s == NULL)
        {
            dp[0] = 0;
        }
        else
        {
            strcpy(dp, cptr_s);
        }

        cptr_p = strtok(cp, " ");
        while (cptr_p)
        {
            assert(sscanf(cptr_p, "%d", &paper_id) == 1);

#ifdef DEBUG
            printf("%d", paper_id);
#endif

            //fprintf(fptr_train_output, "1 ");
            Paper *p = papers + paper_id;

            //fprintf(fptr_train_output, "%d ", p->feature_missing);
            label.push_back(1);
            paper_feature_missing.push_back(p->feature_missing);
            paper_topic.push_back(p->topic);
            double similiarity = 0.0;
            similiarity = get_sim(p, a, K);
            if (p->journal_id > 0)
            {
                is_journal.push_back(1);
                journal_sim.push_back(get_journal_sim(p, journals + p->journal_id, K));
            }
            else
            {
                is_journal.push_back(0);
                journal_sim.push_back(0.0);
            }

            if (p->conference_id > 0)
            {
                is_conference.push_back(1);
                conference_sim.push_back(get_conference_sim(p, conferences + p->conference_id, K));
            }
            else
            {
                is_conference.push_back(0);
                conference_sim.push_back(0);
            }

            scores.push_back(similiarity);
            //fprintf(fptr_train_output, "%lf", similiarity);
            //fputs("\n", fptr_train_output);
            
            cptr_p = strtok(NULL, " ");
#ifdef DEBUG
            if (cptr_p)
                printf(" ");
#endif
        }
        
        
#ifdef DEBUG
        printf(",");
#endif
        cptr_p = strtok(dp, " ");
        while (cptr_p)
        {
            assert(sscanf(cptr_p, "%d", &paper_id) == 1);
            Paper *p = papers + paper_id;
#ifdef DEBUG
            printf("%d", paper_id);
#endif
//            fprintf(fptr_train_output, "-1 ");
            label.push_back(-1);
//            fprintf(fptr_train_output, "%d ", p->feature_missing);
            
            paper_feature_missing.push_back(p->feature_missing);
            paper_topic.push_back(p->topic);
            double similiarity = 0.0;
            similiarity = get_sim(p, a, K);
            if (p->journal_id > 0)
            {
                is_journal.push_back(1);
                journal_sim.push_back(get_journal_sim(p, journals + p->journal_id, K));
            }
            else
            {
                is_journal.push_back(0);
                journal_sim.push_back(0.0);
            }

            if (p->conference_id > 0)
            {
                is_conference.push_back(1);
                conference_sim.push_back(get_conference_sim(p, conferences + p->conference_id, K));
            }
            else
            {
                is_conference.push_back(0);
                conference_sim.push_back(0);
            }

//            fprintf(fptr_train_output, "%lf", similiarity);
//            fputs("\n", fptr_train_output);
            scores.push_back(similiarity);
            cptr_p = strtok(NULL, " ");
#ifdef DEBUG
            if (cptr_p)
                printf(" ");
#endif
        }
#ifdef DEBUG
        puts("");
#endif

        double average_score = 0.0;
        for (vector<double>::iterator it = scores.begin(); it != scores.end(); it++)
            average_score += *it;

        average_score /= ((double)scores.size());

        for (int i = 0, d = (int)label.size(); i < d; i++)
        {
            fprintf(fptr_train_output, "%d ", label[i]);
            fprintf(fptr_train_output, "%d ", is_journal[i]);
            fprintf(fptr_train_output, "%lf ", journal_sim[i]);
            fprintf(fptr_train_output, "%d ", is_conference[i]);
            fprintf(fptr_train_output, "%lf ", conference_sim[i]);
            fprintf(fptr_train_output, "%d %lf %lf ", paper_feature_missing[i], scores[i], scores[i] - average_score);
            fprintf(fptr_train_output, "%d %d %d", a->feature_missing, a->topic, paper_topic[i]);
//            fprintf(fptr_train_output, "%d ", scores[i] > average_score ? 1 : 0);
            fprintf(fptr_train_output, "\n");
        }
    }
    
    fclose(fptr_train);
    fclose(fptr_train_output);

    fprintf(stderr, "Loading Valid.csv\n");
    assert(fgets(line, sizeof(line), fptr_valid));
    
    while(fgets(line, sizeof(line), fptr_valid))
    {

        int author_id;
        int paper_id;
        char *cptr = strtok(line, ",");
        assert(cptr != NULL);
        assert(sscanf(cptr, "%d", &author_id) == 1);
        cptr = strtok(NULL, ",\n");
        assert(cptr != NULL);
//        printf("%s\n", cptr);
        cptr = strtok(cptr, " ");
        assert(cptr != NULL);
        vector<int> label;
        vector<double> scores;
        vector<int> paper_feature_missing;
        vector<int> paper_topic;
        Author *a = authors + author_id;
        double average_score = 0.0;
        vector<double> journal_sim;
        vector<int> is_journal;
        vector<double> conference_sim;
        vector<int> is_conference;

        while (cptr)
        {
#ifndef NO_PATCH_ANSWER
//            fprintf(fptr_valid_output, "+1 ");
            label.push_back(0);
#endif
            assert(sscanf(cptr, "%d", &paper_id) == 1);
            Paper *p = papers + paper_id;

//            fprintf(fptr_valid_output, "%d ", p->feature_missing);
            paper_feature_missing.push_back(p->feature_missing);
            
            if (p->journal_id > 0)
            {
                is_journal.push_back(1);
                journal_sim.push_back(get_journal_sim(p, journals + p->journal_id, K));
            }
            else
            {
                is_journal.push_back(0);
                journal_sim.push_back(0.0);
            }

            if (p->conference_id > 0)
            {
                is_conference.push_back(1);
                conference_sim.push_back(get_conference_sim(p, conferences + p->conference_id, K));
            }
            else
            {
                is_conference.push_back(0);
                conference_sim.push_back(0);
            }

            double similiarity = 0.0;
            similiarity = get_sim(p, a, K);

//            fprintf(fptr_valid_output, "%lf", similiarity);
            scores.push_back(similiarity);
            paper_topic.push_back(p->topic);
//            fputs("\n", fptr_valid_output);

            cptr = strtok(NULL, " ");
        }

        for(vector<double>::iterator it = scores.begin(); it != scores.end(); it++)
            average_score += *it;

        average_score /= ((double)scores.size());

        for (int i = 0, d = (int)label.size(); i < d; i++)
        {
            fprintf(fptr_valid_output, "%d ", 0);
            fprintf(fptr_valid_output, "%d ", is_journal[i]);
            fprintf(fptr_valid_output, "%lf ", journal_sim[i]);
            fprintf(fptr_valid_output, "%d ", is_conference[i]);
            fprintf(fptr_valid_output, "%lf ", conference_sim[i]);
            fprintf(fptr_valid_output, "%d %lf %lf ", paper_feature_missing[i], scores[i], scores[i] - average_score);
            fprintf(fptr_valid_output, "%d %d %d", a->feature_missing, a->topic, paper_topic[i]);
//            fprintf(fptr_train_output, "%d ", scores[i] > average_score ? 1 : 0);
            fprintf(fptr_valid_output, "\n");
        }

    }

    fclose(fptr_valid);
    fclose(fptr_valid_output);

    return 0;
}


