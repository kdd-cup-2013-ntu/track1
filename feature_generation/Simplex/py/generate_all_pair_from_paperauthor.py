#!/usr/bin/python2.7
import sys
import python_util.loader as loader

def make_test_pair(paper_author_pairs):
    pairs = dict()
    for pair in paper_author_pairs:
        author_id = paper_author_pairs['author_id']
        if paper_author_pairs['author_id'] not in pairs:
            pairs[author_id] = []

        pairs[authpr_id].append(paper_author_pairs['paper_id'])
    
    return pairs

def generate_pairs(author_info, paper_info, author_paper_pairs):
    authors = dict()
    for pair in author_paper_pairs:
        author_id = pair['author_id']
        paper_id = pair['paper_id']
        if author_id in author_info and paper_id in paper_info:
            if author_id not in authors:
                authors[author_id] = list()

            authors[author_id].append(paper_id)
    return authors

def make_test_file(authors, test_file_path):
    with open(test_file_path, 'w+') as f:
        f.write("AuthorId,PaperIds\n")
        for author_id in authors:
            f.write(str(author_id) + ',')
            f.write(' '.join([str(x) for x in authors[author_id]]))
            f.write('\n')



if __name__ == '__main__':
    if len(sys.argv) != 1 + 4:
        print >> sys.stderr, "Usage : %s Author.csv Paper.csv PaperAuthor.csv OutputFile" % (sys.argv[0],)
        sys.exit(-1)

    authors = loader.load_author_csv(sys.argv[1])
    papers = loader.load_paper_csv(sys.argv[2])
    paper_author_pairs = loader.load_paper_author_csv(sys.argv[3])

    print 'Data loaded'
    print 'Generating'
    test_pairs = generate_pairs(authors, papers, paper_author_pairs)
    print 'Saving'
    make_test_file(test_pairs, sys.argv[4])




