import sys
from sklearn.ensemble import  RandomForestClassifier
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.datasets import load_svmlight_file
import numpy 
import optparse, os, pickle
def main():
    if len(sys.argv)<4:
        print "- argv not enough!"
        print "- main  train.feature train.csv model_path"
        exit(0)

 
    trainName = sys.argv[1]
    model_path = sys.argv[3]
    train_method = 0

    if len(sys.argv) > 4:
        if sys.argv[4] == '-m':
            train_method = int(sys.argv[5])

    print "read train %s..." % (trainName)
    
    X , Y = load_svmlight_file(trainName)
    
    print "train..."
    
    
#    clf = RandomForestClassifier(n_estimators=50,verbose=2,n_jobs=8,min_samples_split=10,random_state=1,criterion='gini')
#    clf = RandomForestClassifier(n_estimators=800,verbose=2,n_jobs=10,min_samples_split=10,max_features=9,random_state=1,criterion='gini',compute_importances='True')

#    clf = GradientBoostingClassifier(learning_rate=0.1,n_estimators=400,max_depth=5,verbose=2,min_samples_split=10,max_features=9,random_state=1)
    if train_method == 0:
        clf = RandomForestClassifier(n_estimators=2000,verbose=2,n_jobs=10,min_samples_split=10,random_state=1,criterion='gini',compute_importances='True')
    elif train_method == 1:
        clf = GradientBoostingClassifier(learning_rate=0.08,n_estimators=300,max_depth=5,verbose=2,min_samples_split=10,max_features=9,random_state=1)
    else:
        print 'error'

    clf = clf.fit(X.todense(),Y)

    pickle.dump(clf, open(model_path, "w"))
    
    
if __name__ == "__main__":
    main()
    
    
