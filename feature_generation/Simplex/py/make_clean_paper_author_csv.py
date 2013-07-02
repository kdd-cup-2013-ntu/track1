#!/usr/bin/python2.7

import sys
import python_util.loader as loader

def filter_paper_author_csv(paper_author_csv, confirmed_pairs):
    pair_idxs = []
    for idx, pair in enumerate(paper_author_csv):
        author_id = pair['author_id']
        paper_id = pair['paper_id']
        if author_id in confirmed_pairs:
            if paper_id in confirmed_pairs[author_id]:
                pair_idxs.append(idx)
        else:
            pair_idxs.append(idx)

    return pair_idxs

def get_confirmed_pairs(author_in_train, train_csv, test_csv, predicted_labels):
    pairs = dict()
    pair_count = 0
    for author in test_csv:
        author_id = author['author_id']
        paper_id_count = len(author['paper_ids'])

        pairs[author_id] = dict()
        if author_id in author_in_train:
            for paper_id in train_csv[author_id]['confirmed_paper_ids']:
                pairs[author_id][paper_id] = 1
        else:
            for paper_id, label in zip(author['paper_ids'], predicted_labels[pair_count:pair_count + paper_id_count]):
                if label == 1:
                    pairs[author_id][paper_id] = 1

        pair_count += paper_id_count
            
    return pairs

def get_author_in_train(train_csv):
    authors = dict()
    for author in train_csv:
        authors[author['author_id']] = 1

    return authors

def load_labels(label_file_path):
    with open(label_file_path, 'r') as f:
        return [int(x) for x in f.read().split('\n') if x.__len__() > 0]
def make_train_csv_dict(train_csv):
    train_dict = dict()
    for author in train_csv:
        train_dict[author['author_id']] = author

    return train_dict

def make_clean_paper_author(confirmed_pairs, paper_author_pairs, output_file_path):
    with open(output_file_path, 'w+') as f:
        print >> f, 'PaperId,AuthorId,Name,Affiliation'
        for idx in confirmed_pairs:
            print >> f, '%d,%d,%s,%s' % (paper_author_pairs[idx]['paper_id'], paper_author_pairs[idx]['author_id'], paper_author_pairs[idx]['author_name'], paper_author_pairs[idx]['author_affiliation'])

if __name__ == '__main__':
    if len(sys.argv) != 1 + 5:
        print >> sys.stderr, "Usage : %s Train.csv PaperAuthor.csv Test.csv label output_file" % (sys.argv[0], )
        sys.exit(-1)

    train_csv = loader.load_train_csv(sys.argv[1])
    train_csv_dict = make_train_csv_dict(train_csv)
    paper_author_csv = loader.load_paper_author_csv(sys.argv[2])
    test_csv = loader.load_test_csv(sys.argv[3])
    labels = load_labels(sys.argv[4])
    print 'Data loaded!'
    confirmed_pairs = get_confirmed_pairs(get_author_in_train(train_csv),
                                            train_csv_dict, test_csv, labels)
    pair_idxs = filter_paper_author_csv(paper_author_csv, confirmed_pairs)
    make_clean_paper_author(pair_idxs, paper_author_csv, sys.argv[5])
    sys.exit(0)

