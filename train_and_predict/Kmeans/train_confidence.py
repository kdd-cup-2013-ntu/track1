#!/usr/bin/python

import sys, numpy, os
from sklearn.ensemble import RandomForestClassifier
from sklearn.datasets import load_svmlight_file

def main():
	input_data = sys.argv[1]
	output_dir = sys.argv[2]
	test_data = sys.argv[3]

	features, target = load_svmlight_file(input_data)
	test, targettest = load_svmlight_file(test_data)
	print("Getting features from file")

	for seed in xrange(10):
		classifier = RandomForestClassifier(n_estimators=12000, verbose=1, n_jobs=10, min_samples_split=10, random_state=seed)
		classifier.fit(features.todense(), target)

	 	predictions = classifier.predict_proba(test.todense())
		predictions = iter(predictions)

		fout = open(os.path.join(output_dir,"s%d.out"%(seed)),'w')
		for prob in predictions:
			fout.write("%f\n"%(prob[1]))
		fout.close()
		print("Seed: %d"%(seed))

if __name__=="__main__":
	main()
