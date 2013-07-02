#!/usr/bin/python

import sys, pickle, csv
from sklearn.ensemble import RandomForestClassifier
from sklearn.datasets import load_svmlight_file

def main():
	test_data = sys.argv[1]
	test_csv = sys.argv[2]
	model = sys.argv[3]
	predict = sys.argv[4]

	features, target = load_svmlight_file(test_data)
	print("Loading the classifier")
	classifier = pickle.load(open(model,'r'))

	print("Making predictions")
	predictions = classifier.predict_proba(features.todense())
	predictions = iter(predictions)

	fout = open(predict,'w')
	with open(test_csv, 'rb') as csvfile:
		reader = csv.reader(csvfile)
		reader.next()
	        for row in reader:
			author = row[0]
			Papers = row[1].split()
			for paper in Papers:
				prob = prediction.next()[1]
				fout.write("%f\n"%(prob))
	fout.close()
if __name__=="__main__":
	main()
