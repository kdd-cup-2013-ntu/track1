#!/usr/bin/env python2.7

import sys
import python_util.loader as loader
from python_util.config import *


def get_internal_author_ids(internal_train_csv):
    author_ids = dict()
    for rec in internal_train_csv:
        author_ids[rec['author_id']] = 1
    return author_ids


if __name__ == '__main__':
    if len(sys.argv) != 1 + 3:
        print >> sys.stderr, "Usage : %s feature_file internal_train_output internal_valid_output" % (sys.argv[0],)
        sys.exit(-1)

    train_csv = loader.load_train_csv(DATA['Train'])
    internal_train_csv = loader.load_train_csv(DATA['internal_train'])
    internal_valid_csv = loader.load_test_csv(DATA['internal_valid'])


    author_papers = dict()

    with open(sys.argv[1], 'r') as f:
        lines = f.read().split('\n')
        pair_count = 0
        for rec in train_csv:
            author_id = rec['author_id']
            paper_ids = rec['confirmed_paper_ids'] + rec['deleted_paper_ids']
            paper_id_count = len(paper_ids)
            author_papers[author_id] = dict()
            
            for idx, paper_id in enumerate(paper_ids):
                if paper_id not in author_papers[author_id]:
                    author_papers[author_id][paper_id] = lines[pair_count + idx]

            pair_count += paper_id_count

    with open(sys.argv[2], 'w+') as f:
        for rec in internal_train_csv:
            author_id = rec['author_id']
            for paper_id in rec['confirmed_paper_ids'] + rec['deleted_paper_ids']:
                f.write(author_papers[author_id][paper_id] + '\n')

    with open(sys.argv[3], 'w+') as f:
        for rec in internal_valid_csv:
            author_id = rec['author_id']
            for paper_id in rec['paper_ids']:
                f.write(author_papers[author_id][paper_id] + '\n')


            


    
    sys.exit(0)

