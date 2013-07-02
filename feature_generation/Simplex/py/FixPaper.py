#!/usr/bin/env python2.7
import csv
import sys

def clean(string):
    return string.replace('\n', ' ').replace('\r', ' ').replace('\t', ' ').replace(',', ' ').replace('"', '')

def main(argv):
    if len(argv) != 1 + 2:
        print >> sys.stderr, 'Usage : %s Paper.csv Output' % (argv[0]);
        return -1
    
    with open(argv[1], 'r') as f, open(argv[2], 'w+') as output:
        reader = csv.DictReader(f)
        
        output.write('Id,Title,Year,ConferenceId,JournalId,Keyword\n')

        for row in reader:
            output.write('%s,%s,%s,%s,%s,%s\n' % (clean(row['Id']), clean(row['Title']), clean(row['Year']), clean(row['ConferenceId']), clean(row['JournalId']), clean(row['Keyword'])))

    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))


