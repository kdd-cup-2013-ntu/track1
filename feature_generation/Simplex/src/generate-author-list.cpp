#include <cstdio>
#include <cstdlib>
#include <set>
#include <vector>
#include "kdd2013.h"
#include <cassert>
using namespace std;

vector<int> author_ids[MAX_PAPER_ID_IN_PAPER_AUHTOR + 1];
//set<int> author_ids_seen[MAX_PAPER_ID_IN_PAPER_AUHTOR + 1];
int main(int argc, char *argv[])
{
	if (argc != 1 + 2)
	{
		fprintf(stderr, "Usage : %s PaperAuthor.csv Author-List-Output\n", argv[0]);
		return 0;
	}

	FILE *fptr_paper_author;
	fptr_paper_author = fopen(argv[1], "r");
	assert(fptr_paper_author != NULL);

	FILE *fptr_author_list;
	fptr_author_list = fopen(argv[2], "w+");
	assert(fptr_author_list != NULL);
	
	char line[1000000];

	assert(fgets(line, sizeof(line), fptr_paper_author));

	while (fgets(line, sizeof(line), fptr_paper_author))
	{
		int paper_id, author_id;

		assert(sscanf(line, "%d,%d", &paper_id, &author_id) == 2);

//        if (author_ids_seen[paper_id].find(author_id) == author_ids_seen[paper_id].end())
        {
            author_ids[paper_id].push_back(author_id);
  //          author_ids_seen[paper_id].insert(author_id);
        }
	}

	fprintf(fptr_author_list, "%d\n", MAX_PAPER_ID_IN_PAPER_AUHTOR);

	for (int i = 0; i <= MAX_PAPER_ID_IN_PAPER_AUHTOR; i++)
	{
		fprintf(fptr_author_list, "%d\n", (int)author_ids[i].size());
		for (int j = 0, d = (int)author_ids[i].size(); j < d; j++)
			fprintf(fptr_author_list, "%d\n", author_ids[i][j]);
	}
	
	fclose(fptr_paper_author);
	fclose(fptr_author_list);
	return 0;
}
