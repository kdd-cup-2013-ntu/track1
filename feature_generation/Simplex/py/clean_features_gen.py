#!/usr/bin/env python3

import csv
import sys

import features

DATA = {}

def train_feature_gen():
    ap_pairs = []
    labels = []
    print (DATA['Train'])
    with open(DATA['Train'], 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            confirmed_paper_ids = [paper_id for paper_id in row['ConfirmedPaperIds'].strip().split(' ') if paper_id.__len__() > 0]
            deleted_paper_ids = [paper_id for paper_id in row['DeletedPaperIds'].strip().split(' ') if paper_id.__len__() > 0]
            ap_pairs += [(row['AuthorId'], pid) for pid in confirmed_paper_ids]
            labels += [1] * len(confirmed_paper_ids)
            ap_pairs += [(row['AuthorId'], pid) for pid in deleted_paper_ids]
            labels += [-1] * len(deleted_paper_ids)
        

#    for pair in ap_pairs:
#        print (pair[0], pair[1])

#    print (len(ap_pairs))
#    sys.exit(0)

    f_bcmk = features.benchmark(ap_pairs=ap_pairs, paper_author_csv=DATA['PaperAuthor'], paper_csv=DATA['Paper'])
    f_yrre = features.years_related(ap_pairs=ap_pairs, paper_author_csv=DATA['PaperAuthor'], paper_csv=DATA['Paper'])
    f_strdist = features.string_distances(ap_pairs=ap_pairs, paper_author_csv=DATA['PaperAuthor'], author_csv=DATA['Author'])
    f_coauth = features.coauthor_related(ap_pairs=ap_pairs, paper_author_csv=DATA['PaperAuthor'])

    with open(sys.argv[1], 'w') as g:
        for i in range(len(labels)):
            print(labels[i], end='', file=g)
            print('', *f_bcmk[i], sep=' ', end='', file=g)
            print('', *f_yrre[i], sep=' ', end='', file=g)
            print('', *f_strdist[i], sep=' ', end='', file=g)
            print('', *f_coauth[i], sep=' ', end='', file=g)
            print('', file=g)


def valid_feature_gen():
    ap_pairs = []
    labels = []
    with open(DATA['Valid'], 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            paper_ids = row['PaperIds'].strip().split(' ')
            ap_pairs += [(row['AuthorId'], pid) for pid in paper_ids]
            labels += [0] * len(paper_ids)

    f_bcmk = features.benchmark(ap_pairs=ap_pairs, paper_author_csv=DATA['PaperAuthor'], paper_csv=DATA['Paper'])
    f_yrre = features.years_related(ap_pairs=ap_pairs, paper_author_csv=DATA['PaperAuthor'], paper_csv=DATA['Paper'])
    f_strdist = features.string_distances(ap_pairs=ap_pairs, paper_author_csv=DATA['PaperAuthor'], author_csv=DATA['Author'])
    f_coauth = features.coauthor_related(ap_pairs=ap_pairs, paper_author_csv=DATA['PaperAuthor'])

    with open(sys.argv[2], 'w') as g:
        for i in range(len(labels)):
            print(labels[i], end='', file=g)
            print('', *f_bcmk[i], sep=' ', end='', file=g)
            print('', *f_yrre[i], sep=' ', end='', file=g)
            print('', *f_strdist[i], sep=' ', end='', file=g)
            print('', *f_coauth[i], sep=' ', end='', file=g)
            print('', file=g)


if __name__ == '__main__':
    if len(sys.argv) != 1 + 5:
        print('usage: %s train-features valid-features data Train.csv CleanTest.csv' % (sys.argv[0],), file=sys.stderr)
        exit(-1)

    argv = sys.argv
    DATA['Train'] = argv[4]
    DATA['Valid'] = argv[5]
    DATA['PaperAuthor'] = './output/PaperAuthor.csv.final'
    DATA['Author'] = './output/Author.csv.final'
    DATA['Paper'] = './output/Paper.csv.final'



    train_feature_gen()
    valid_feature_gen()

