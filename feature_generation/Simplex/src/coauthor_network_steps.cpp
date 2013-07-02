#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>
#include <cassert>
using namespace std;


const int BUFFER_SIZE = 1024000;


int maxPaperId;
vector<vector<int> > paperToAuthor;
vector<vector<int> > authorToPaper;

int maxAuthorId;
int V, N;
long long E;

map<int, int> ways;


void DFS(int x, int d) {
    if (d == 0) {
        if (ways.find(x) == ways.end()) {
            ways[x] = 1;
        } else {
            ways[x] += 1;
        }
        return;
    }

    for (vector<int>::iterator it = authorToPaper[x].begin(); it != authorToPaper[x].end(); it++) {
        int p = *it;
        for (vector<int>::iterator it2 = paperToAuthor[p].begin(); it2 != paperToAuthor[p].end(); it2++) {
            int a = *it2;
            if (a == x) continue;
//            if (ways[x] == 100)continue;
            DFS(a, d - 1);
        }
    }
}


int main(int argc, char *argv[]) {
    if (argc != 1 + 2) {
        fprintf(stderr, "usage: %s author-list-file step-count\n\n", argv[0]);
        return -1;
    }

    int STEP_COUNT;
    sscanf(argv[2], "%d", &STEP_COUNT);

    static char buf[BUFFER_SIZE];
    FILE *inFile = fopen(argv[1], "r");

    assert(fscanf(inFile, "%d", &maxPaperId) == 1);
    paperToAuthor.resize(maxPaperId);
    authorToPaper.resize(maxPaperId * 2);
    maxAuthorId = 0;
    for (int i = 0; i <= maxPaperId; i++) {
        int len;

        if (fscanf(inFile, "%d", &len) != 1) {
            fprintf(stderr, "format error. (at paper-id = %d)\n\n", i);
            return -1;
        }
        for (int j = 0; j < len; j++) {
            int authorId;

            assert(fscanf(inFile, "%d", &authorId) == 1);
            paperToAuthor[i].push_back(authorId);
            authorToPaper[authorId].push_back(i);

            if (authorId > maxAuthorId) {
                maxAuthorId = authorId;
            }
        }
    }

    if (fscanf(inFile, "%s", buf) != EOF) {
        fprintf(stderr, "format error. (too many lines, maybe wrong maxPaperId)\n\n");
        return -1;
    }

    fclose(inFile);
    fprintf(stderr, "finish reading the input\n");

    V = maxAuthorId + 1;  //including "node 0"

    for (int i = 0; i < V; i++) {
        ways.clear();
        DFS(i, STEP_COUNT);

        printf("%d", i);
        for (map<int, int>::iterator it = ways.begin(); it != ways.end(); it++) {
            printf(" %d:%d", it->first, it->second);
        }
        putchar('\n');
    }

    return 0;
}
