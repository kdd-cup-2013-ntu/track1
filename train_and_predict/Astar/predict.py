import sys
from sklearn.ensemble import  RandomForestClassifier
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.datasets import load_svmlight_file
import numpy 
import optparse, os,pickle
def main():
    if len(sys.argv)<5:
        print "- argv not enough!"
        print "- main valid_data valid.csv model valid_pred"
        exit(0)


    validName = sys.argv[1]
    model_path = sys.argv[3]
    validOut = sys.argv[4]

    print "Load model %s..." % (model_path)

    clf = pickle.load(open(model_path))
    
    print "read valid %s..." % (validName)
    
    TX , TY = load_svmlight_file(validName)
    
    print "predict valid..."
    
    result = clf.predict_proba(TX.todense())[:,1]
    
    print "write valid pred %s..." % (validOut)
    
    f = open(validOut,"w")
    
    for r in result:
        f.write("%f\n" % (r))
    f.close()
    
if __name__ == "__main__":
    main()
    
    
