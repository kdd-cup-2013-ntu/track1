# Since the spec requires one model file, this code concatenate 10 models(in models/) to one file.

import sys

nmodel = int( open('nmodel').readline() )

fout = open( sys.argv[1], 'w' )
for i in range(nmodel):
    model = open( 'models/model'+str(i) ).read()
    fout.write( model )

fout.close()
    
    

