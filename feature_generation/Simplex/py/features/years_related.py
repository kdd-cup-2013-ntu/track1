import csv
from collections import defaultdict

from features import *
import numeric_operations as np


def years_related(ap_pairs, paper_author_csv, paper_csv):
    paper_year = dict()
    author_years = defaultdict(list)

    with open(paper_csv, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            assert len(row['Year'].strip()) > 0
            if int(row['Year'].strip()) <= 0:  # there are 0's and -1's
                continue
            paper_year[row['Id']] = int(row['Year'].strip())

    with open(paper_author_csv, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            author = row['AuthorId']
            paper = row['PaperId']
            if not paper in paper_year:
                continue
            author_years[author].append(paper_year[paper])

    return [[min(author_years[ap[0]]) if len(author_years[ap[0]]) > 0 else 2000,
             max(author_years[ap[0]]) if len(author_years[ap[0]]) > 0 else 2000,
             max(author_years[ap[0]]) - min(author_years[ap[0]]) if len(author_years[ap[0]]) > 0 else 0,
             paper_year.get(ap[1], 2000),
             abs(paper_year.get(ap[1], 2000) - (np.median(author_years[ap[0]]) if len(author_years[ap[0]]) > 0 else 2000))
            ] for ap in ap_pairs]

