# Spilt the concatenated file to original 10 model files.

import sys
import re

nmodel = int( open('nmodel').readline() )
f = open( sys.argv[1] )
models = re.findall( r'<Ensemble>.*?</Ensemble>', f.read(), re.DOTALL )
f.close()

for i in range(nmodel):
    fout = open( 'models/model'+str(i), 'w' )
    fout.write( models[i] )
    fout.close()
    
    

