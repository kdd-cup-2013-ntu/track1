# Add query id to the feature file

import sys
import csv

fcsv = open( sys.argv[1] )

fin = open( sys.argv[2] )
fout = open( sys.argv[3], 'w')

i = 0
fcsv.next()
for row in csv.reader( fcsv ):
    pids = ' '.join( row[1:] )
    
    for j in range( len( pids.split() ) ):
        label, fea = fin.readline().split(None,1)
        if label == '+1':
            label = '1'
        if label != '1':
            label = '0'
        fout.write( '%s qid:%d %s' % (label, i, fea) )

    i += 1
