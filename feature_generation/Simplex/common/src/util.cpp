#include <cassert>
#include <cstdlib>
#include <cstring>
#include "util.h"
#include <set>
#include <map>
using namespace std;
int load_confidence_score(FILE *fptr_confidence_score, map<int, map<int, double> >& confidence_score)
{
    int author_id, paper_id;
    double score;
    while(fscanf(fptr_confidence_score, "%d %d %lf", &author_id, &paper_id, &score) == 3)
    {
        confidence_score[author_id][paper_id] = score;
    }
    return 0;
}
void split_string(const char *str, vector<string> &slices, const char *dem)
{
    char *buf = new char[strlen(str) + 1];

    strcpy(buf, str);

    assert(slices.size() == 0);

    char *cptr = strtok(buf, dem);
    while (cptr != NULL)
    {
        slices.push_back(string(cptr));
        cptr = strtok(NULL, dem);
    }
}
    
double get_variance(vector<double> &items)
{
    int count = (int) items.size();
    if (count == 0)
        return 0.0;
    double mean = 0.0;
    double tmp = 0.0;
    for (int i = 0; i < count; i++)
    {
        mean += items[i];
        tmp += items[i] * items[i];
    }
    tmp /= count;
    mean = mean / count;
    return tmp - mean * mean;
}
inline double compute_authorwise_score(double *va, double *vb, int K)
{
    double sum = 0.0;
    for (int i = 0; i < K; i++)
        sum += va[i] *vb[i];
    
    return sum;
}
int compute_coauthor_score(Paper *paper, Author *authors, int K)
{
    int user_count = (int)paper->author_ids.size();
    if (user_count <= 1)
        return 0;

    paper->authorwise_score = new double*[user_count];
    for (int i = 0; i < user_count; i++)
        paper->authorwise_score[i] = new double[user_count];

    for (int i = 0; i < user_count; i++)
        paper->authorwise_score[i][i] = 0.0;

    for (int i = 0; i < user_count; i++)
        for (int j = i + 1; j < user_count; j++)
            paper->authorwise_score[i][j] = paper->authorwise_score[j][i] = compute_authorwise_score(authors[paper->author_ids[i]].topic_feature, authors[paper->author_ids[j]].topic_feature, K);

    return 0;
}

int get_coauthor_ids(int author_id, Paper *paper, vector<int> &coauthor_ids)
{
    set<int> seen;
    assert(coauthor_ids.size() == 0);

    for (vector<int>::iterator it = paper->author_ids.begin(), end = paper->author_ids.end(); it != end; it++)
    {
        if (*it == author_id || seen.find(*it) != seen.end())
            continue;
        coauthor_ids.push_back(*it);
    }
    return 0;
}
        
int load_author_topic_feature(FILE *fptr_author_topic_feature_file, Author *authors,int K)
{
    assert(fptr_author_topic_feature_file != NULL);
    int author_id;
    while(fscanf(fptr_author_topic_feature_file, "%d", &author_id) == 1)
    {
        Author *a = authors + author_id;
        a->topic_feature = new double[K];
        int dummy;
        for (int i = 0; i < 3; i++)
            assert(fscanf(fptr_author_topic_feature_file, "%d", &dummy) == 1);
        for (int i = 0; i < K; i++)
            assert(fscanf(fptr_author_topic_feature_file, "%lf", a->topic_feature + i) == 1);
    }
    return 0;

}
int load_train_csv(FILE *fptr_train, vector<TrainRecord>& train_records)
{
    char line[1000000];
    assert(fptr_train != NULL);

    assert(fgets(line, sizeof(line), fptr_train));

    assert(train_records.size() == 0);
    train_records.clear();

    while (fgets(line, sizeof(line), fptr_train))
    {
        char cp[100000];
        char dp[100000];
        int paper_id;
        int author_id;

        TrainRecord rec;
        
        int offset = 0;

        offset += get_section(line + offset, cp, NULL);

        assert(sscanf(cp, "%d", &author_id) == 1);
        rec.author_id = author_id;
        
        offset += get_section(line + offset, cp, NULL);
        offset += get_section(line + offset, dp, NULL);

        char *cptr_p;

        cptr_p = strtok(cp, " ");
        while (cptr_p)
        {
            assert(sscanf(cptr_p, "%d", &paper_id) == 1);
            rec.confirmed_paper_ids.push_back(paper_id);
            cptr_p = strtok(NULL, " ");
        }
        
        
        cptr_p = strtok(dp, " ");
        while (cptr_p)
        {
            assert(sscanf(cptr_p, "%d", &paper_id) == 1);
            rec.deleted_paper_ids.push_back(paper_id);
            cptr_p = strtok(NULL, " ");
        }

        train_records.push_back(rec);
    }

    return 0;


}
int load_test_csv(FILE *fptr_test, vector<TestRecord>& test_records)
{

    char line[1000000];

    assert(fptr_test != NULL);

    assert(test_records.size() == 0);

    assert(fgets(line, sizeof(line), fptr_test));
    
    while(fgets(line, sizeof(line), fptr_test))
    {

        int author_id;
        int paper_id;
        TestRecord rec;
        char *cptr = strtok(line, ",");
        assert(cptr != NULL);
        assert(sscanf(cptr, "%d", &author_id) == 1);
        cptr = strtok(NULL, ",\n");
        assert(cptr != NULL);
        cptr = strtok(cptr, " ");
        assert(cptr != NULL);
        rec.author_id = author_id;
        while (cptr)
        {
            assert(sscanf(cptr, "%d", &paper_id) == 1);
            rec.paper_ids.push_back(paper_id);
            cptr = strtok(NULL, " ");
        }

        test_records.push_back(rec);

    }
    return 0;
}

int load_groups(FILE *fptr_group, vector<Group>& groups)
{
    assert(fptr_group != NULL);

    groups.clear();

    set<int> seen;
    
    char line[100000];

    assert(fgets(line, sizeof(line), fptr_group));


    while (fgets(line, sizeof(line), fptr_group))
    {
        char *cptr;

        Group g;

        cptr = strtok(line, ",\n");

        cptr = strtok(NULL, ",\n");

        cptr = strtok(cptr, " ");
        
        while (cptr)
        {
            int author_id;
            assert(sscanf(cptr, "%d", &author_id) == 1);

            if (seen.find(author_id) != seen.end())
            {
                if (g.author_ids.size() != 0)
                {
                    fprintf(stderr, "%d\n", author_id);
                    assert(g.author_ids.size() == 0);
                }
                break;
            }
            
            g.author_ids.push_back(author_id);

            cptr = strtok(NULL, " ");
        }

        
        if (g.author_ids.size() > 1)
        {
            groups.push_back(g);
            for (vector<int>::iterator it = g.author_ids.begin(); it != g.author_ids.end(); it++)
                seen.insert(*it);
        }
    }

    return 0;
}


int load_paper_authors(FILE *fptr_paper_author, Author *authors, Paper *papers)
{
    assert(fptr_paper_author != NULL);

    char line[100000];

    assert(fgets(line, sizeof(line), fptr_paper_author));

    while (fgets(line, sizeof(line), fptr_paper_author))
    {
        char tmp[500];
        int offset = 0;
        int paper_id;
        int author_id;

        offset += get_section(line + offset, tmp, NULL);
        assert(sscanf(tmp, "%d", &paper_id) == 1);

        offset += get_section(line + offset, tmp, NULL);
        assert(sscanf(tmp, "%d", &author_id) == 1);
        
        Author *a = authors + author_id;
        Paper *p = papers + paper_id;
        
        if (a->paper_ids_set.find(paper_id) == a->paper_ids_set.end())
        {
            a->paper_ids_set.insert(paper_id);    
            a->paper_ids.push_back(paper_id);
        }

        if (p->author_ids_set.find(author_id) == p->author_ids_set.end())
        {
            p->author_ids_set.insert(author_id);
            p->author_ids.push_back(author_id);
        }
    }

    return 0;
}


int load_papers(FILE *fptr_paper, vector<Paper>& papers)
{
    assert(fptr_paper != NULL);

    papers.clear();
    char line[100000];

    assert(fgets(line, sizeof(line), fptr_paper));

    while (fgets(line, sizeof(line), fptr_paper))
    {
        char tmp[500];

        int offset = 0;

        Paper p;

        offset += get_section(line + offset, tmp, NULL);
        assert(sscanf(tmp, "%d", &p.paper_id) == 1);

        offset += get_section(line + offset, p.title, NULL);
        
        offset += get_section(line + offset, tmp, NULL);
        assert(sscanf(tmp, "%d", &p.year) == 1);

        offset += get_section(line + offset, tmp, NULL);
        assert(sscanf(tmp, "%d", &p.conference_id) == 1);

        offset += get_section(line + offset, tmp, NULL);
        assert(sscanf(tmp, "%d", &p.journal_id) == 1);

        offset += get_section(line + offset, p.keywords, NULL);

        papers.push_back(p);
    }
    return 0;
}

int load_authors(FILE *fptr_author, vector<Author>& authors, bool lower /* = true */)
{
    char line[100000];
    assert(fptr_author != NULL);

    assert(fgets(line, sizeof(line), fptr_author));
    authors.clear();
    
    Author a;

    while (fgets(line, sizeof(line), fptr_author))
    {
        char tmp[1000];
        int offset = 0;

        offset += get_section(line + offset, tmp, NULL);
        assert(sscanf(tmp, "%d", &a.author_id) == 1);

       
        offset += get_section(line + offset, a.author_name, NULL);
        if (lower)
        {
            for (int i = 0; a.author_name[i] != 0; i++)
                if (a.author_name[i] >= 'A' && a.author_name[i] <= 'Z')
#pragma GCC diagnostic ignored "-Wconversion"
                    a.author_name[i] -= 'A' - 'a';
#pragma GCC diagnostic error "-Wconversion"
        }

        offset += get_section(line + offset, a.affiliation_name, NULL);
        
        if (lower)
        {
            for (int i = 0; a.affiliation_name[i] != 0; i++)
                if (a.affiliation_name[i] >= 'A' && a.affiliation_name[i] <= 'Z')
#pragma GCC diagnostic ignored "-Wconversion"
                    a.affiliation_name[i] -= 'A' - 'a';
#pragma GCC diagnostic error "-Wconversion"

        }

        
        authors.push_back(a);
    }
    return 0;
}

inline int min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

inline int max(int a, int b)
{
    if (a > b)
        return a;
    return b;
}
int get_levenshtein_distance(const char *a, const char *b)
{
    int dp[500][500];
    int len_a = (int)strlen(a);
    int len_b = (int)strlen(b);

    assert(len_a < 500 && len_b < 500);

    for (int j = 0; j <= len_b; j++)
        dp[0][j] = j;

    for (int i = 0; i <= len_a; i++)
        dp[i][0] = i;

    for (int i = 1; i <= len_a; i++)
        for (int j = 1; j <= len_b; j++)
                dp[i][j] = min(min(dp[i-1][j] + 1, dp[i][j-1] + 1), dp[i-1][j-1] + (a[i-1] == b[j-1] ? 0 : 1));

    return dp[len_a][len_b];
}

int get_section(char *str, char *buffer, bool *skip)
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
                else if (skip == NULL || !skip[(int)str[i]])
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

int Paper::get_author_idx(int author_id)
{
    int idx = 0;
    for (vector<int>::iterator it = author_ids.begin(), end = author_ids.end(); it != end; it++, idx++)
    {
        if (*it == author_id)
            return idx;
    }
    return -1;
}

