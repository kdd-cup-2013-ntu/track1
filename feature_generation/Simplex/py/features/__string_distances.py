import csv
import math
import sys

from features import *


def levenshtein_distance(s, t):
    if len(t) < len(s):
        return levenshtein_distance(t, s)
    if len(t) == 0:
        return len(s)
    
    previous_row = [x for x in range(len(t) + 1)]
    for i, c1 in enumerate(s):
        current_row = [i + 1]
        for j, c2 in enumerate(t):
            insertion = previous_row[j + 1] + 1
            deletion = current_row[j] + 1
            substitution = previous_row[j] + (c1 != c2)
            current_row.append(min(insertion, deletion, substitution))
        previous_row = current_row

    return previous_row[-1]


def string_distances(ap_pairs, paper_author_csv, author_csv):
    author_csv_affiliation = dict()
    ap_pairs_set = set(ap_pairs)
    lev_dist = dict()
    author_lev_dist_sum = dict()

    with open(author_csv, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            author_csv_affiliation[row['Id']] = row['Affiliation'].strip()

    with open(paper_author_csv, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            aff = row['Affiliation'].strip()
            if len(aff) == 0 or (not row['AuthorId'] in author_csv_affiliation):
                continue
            curr_ap = (row['AuthorId'], row['PaperId'])
            if curr_ap in ap_pairs_set:
                '''
                if curr_ap in lev_dist:
                    print(curr_ap, lev_dist[curr_ap], levenshtein_distance(author_csv_affiliation[row['AuthorId']], aff) * 1.0, aff, '|', author_csv_affiliation[row['AuthorId']], file=sys.stderr)
                '''
                lev_dist[curr_ap] = max(lev_dist.get(curr_ap, 0.0), levenshtein_distance(author_csv_affiliation[row['AuthorId']], aff) * 1.0)

    lev_dist_avg = math.fsum(lev_dist.values()) / len(lev_dist)

    return [lev_dist[ap] if ap in lev_dist else lev_dist_avg for ap in ap_pairs]

