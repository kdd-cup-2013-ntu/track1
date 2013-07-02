# Average confidence scores from 10 models.

import sys

nmodel = int( open('nmodel').readline() )

p = []
for i in range(nmodel):
    f = open( 'predictions/predictions'+str(i)+'.txt' )
    p.append( [ float(row.strip('\n')) for row in f.readlines() ] )


psum = [0]*len(p[0])
for predict in p:
    psum = [ psum[j]+predict[j] for j in range(len(predict)) ]


fout = open( sys.argv[1], 'w')
for i in range(len(psum)):
    fout.write( str(psum[i]/nmodel)+'\n' )

fout.close()

