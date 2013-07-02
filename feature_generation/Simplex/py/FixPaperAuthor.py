#!/usr/bin/env python2.7
import csv
import sys

def clean(string):
    return string.replace('\n', ' ').replace('\r', ' ').replace('\t', ' ').replace(',', ' ').replace('"', '')

def main(argv):
    if len(argv) != 1 + 2:
        print >> sys.stderr, 'Usage : %s PaperAuthor.csv output' % (argv[0])
        return -1
	
    with open(argv[1], 'r') as f, open(argv[2], 'w+') as output:
        output.write("PaperId,AuthorId,Name,Affiliation\n")
        reader = csv.DictReader(f)
        for row in reader:
            output.write('%s,%s,%s,%s\n' % (clean(row['PaperId']), clean(row['AuthorId']), clean(row['Name']), clean(row['Affiliation'])))

            
    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))

