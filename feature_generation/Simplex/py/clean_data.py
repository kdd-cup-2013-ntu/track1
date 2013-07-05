#!/usr/bin/env python2.7
import csv, re, pickle, sys
from collections import defaultdict
from filter_new import latin_filter

def parse_id(string):
    return string.rstrip().split()

def readinput(input_path):
    table = list()
    with open(input_path, "r") as f:
        reader = csv.reader(f)
        for row in reader:
            table.append(row)
    attr = table.pop(0)
    return attr, table

def get_invalid_word_set(path):
    with open(path[0]) as fin, open(path[1], 'w') as fout:
        i = 0
        print '%9d' % (i),
        for line in fin:
            line = latin_filter(line)
            fout.write(line)
            i += 1
            if i % 10000 == 0:
                print '\b'*10 + '%9d' %(i),
            
            

def main(argv):
    if len(argv) != 1 + 2:
        print >> sys.stderr, 'Usage : %s Target.csv Clean.csv' % (argv[0],)
        return -1
    
    map(get_invalid_word_set, [(argv[1], argv[2])])

    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
