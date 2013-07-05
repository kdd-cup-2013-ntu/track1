import csv
from collections import defaultdict

from features import *


def num_of_papers_with_coauthors(author, paper, author_papers, paper_authors):
    counter = 0

    for p in author_papers[author]:
        if len(paper_authors[paper] & paper_authors[p]) > 1:  # 1: only this author himself
            counter += 1

    return counter

def num_of_authors_coworked_at_least_one_other_paper(author, paper, author_papers, paper_authors):
    counter = 0

    for a in paper_authors[paper]:
        if len(author_papers[author] & author_papers[a]) > 1:  # 1: only this coworked paper
            counter += 1

    return counter


def coauthor_related(ap_pairs, paper_author_csv):
    author_papers = defaultdict(set)
    paper_authors = defaultdict(set)

    with open(paper_author_csv, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            author = row['AuthorId']
            paper = row['PaperId']
            author_papers[author].add(paper)
            paper_authors[paper].add(author)

    return [[num_of_papers_with_coauthors(ap[0], ap[1], author_papers, paper_authors),
             num_of_authors_coworked_at_least_one_other_paper(ap[0], ap[1], author_papers, paper_authors)
            ] for ap in ap_pairs]

