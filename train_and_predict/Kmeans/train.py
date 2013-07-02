#!/usr/bin/python

import sys, os, pickle
from sklearn.ensemble import RandomForestClassifier
from sklearn.datasets import load_svmlight_file

def main():
	input_data = sys.argv[1]
	train_csv = sys.argv[2]
	model = sys.argv[3]

	features, target = load_svmlight_file(input_data)
	print("Getting features from file")

	classifier = RandomForestClassifier(n_estimators=12000, verbose=1, n_jobs=10, min_samples_split=10, random_state=1)
	classifier.fit(features.todense(), target)

	pickle.dump(classifier,model)

if __name__=="__main__":
	main()

