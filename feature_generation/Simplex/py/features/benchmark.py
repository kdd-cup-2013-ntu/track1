import csv
from collections import Counter

from features import *


def benchmark(ap_pairs, paper_author_csv, paper_csv):
    author_paper_count = Counter()
    paper_author_count = Counter()
    author_conf_paper_count = Counter()
    author_jour_paper_count = Counter()
    paper_to_conf = dict()
    paper_to_jour = dict()

    with open(paper_csv, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            paper_to_conf[row['Id']] = int(row['ConferenceId'])
            paper_to_jour[row['Id']] = int(row['JournalId'])

    with open(paper_author_csv, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            author_paper_count[row['AuthorId']] += 1
            paper_author_count[row['PaperId']] += 1
            author_conf_paper_count[(row['AuthorId'], paper_to_conf.get(row['PaperId'], -1))] += 1
            author_jour_paper_count[(row['AuthorId'], paper_to_jour.get(row['PaperId'], -1))] += 1

    return [[author_paper_count[ap[0]],
             paper_author_count[ap[1]],
             author_conf_paper_count[(ap[0], paper_to_conf.get(ap[1], -1))],
             author_jour_paper_count[(ap[0], paper_to_jour.get(ap[1], -1))]
            ] for ap in ap_pairs]

