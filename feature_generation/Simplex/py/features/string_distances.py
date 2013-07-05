import csv
import difflib
import math
import sys
from collections import defaultdict

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


def jaro_winkler_distance(s, t):
    pass


def longest_common_subsequence(s, t):
    pass


def ratcliff_obershelp_python_builtin(s, t):
    return difflib.SequenceMatcher(None, s, t).ratio()


def string_distances(ap_pairs, paper_author_csv, author_csv):
    author_csv_name = dict()
    author_csv_affiliation = dict()
    ap_pairs_set = set(ap_pairs)
    name_leve_dist = defaultdict(list)
    affi_leve_dist = defaultdict(list)
    name_ropb_simi = defaultdict(list)
    affi_ropb_simi = defaultdict(list)

    with open(author_csv, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            author_csv_name[row['Id']] = row['Name'].strip()
            author_csv_affiliation[row['Id']] = row['Affiliation'].strip()

    with open(paper_author_csv, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            curr_ap = (row['AuthorId'], row['PaperId'])
            name = row['Name'].strip()
            affi = row['Affiliation'].strip()

            if len(name) > 0 and row['AuthorId'] in author_csv_name:
                if curr_ap in ap_pairs_set:
                    name_leve_dist[curr_ap].append(levenshtein_distance(author_csv_name[row['AuthorId']], name) * 1.0)
                    name_ropb_simi[curr_ap].append(ratcliff_obershelp_python_builtin(author_csv_name[row['AuthorId']], name))

            if len(affi) > 0 and row['AuthorId'] in author_csv_affiliation:
                if curr_ap in ap_pairs_set:
                    affi_leve_dist[curr_ap].append(levenshtein_distance(author_csv_affiliation[row['AuthorId']], affi) * 1.0)
                    affi_ropb_simi[curr_ap].append(ratcliff_obershelp_python_builtin(author_csv_affiliation[row['AuthorId']], affi))

    return [[max(name_leve_dist[ap]) if ap in name_leve_dist else 0.5,
             max(affi_leve_dist[ap]) if ap in affi_leve_dist else 0.5,
             # min(name_leve_dist[ap]) if ap in name_leve_dist else 0.0,
             # min(affi_leve_dist[ap]) if ap in affi_leve_dist else 0.0,
             # name_ropb_simi[ap] if ap in name_ropb_simi else 0.9,
             # affi_ropb_simi[ap] if ap in affi_ropb_simi else 0.9
            ] for ap in ap_pairs]

