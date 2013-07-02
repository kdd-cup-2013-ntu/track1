#!/usr/local/bin/python

import sys, numpy, os, csv
from sklearn.ensemble import RandomForestClassifier
from sklearn.datasets import load_svmlight_file

def main():
	input_data = sys.argv[1]
	test_data = sys.argv[2]
	test_csv = sys.argv[3]
	predict = sys.argv[4]

	features, target = load_svmlight_file(input_data)
	test, targettest = load_svmlight_file(test_data)
	print("Getting features from file")

	if not os.path.isdir("T12000"):
		os.mkdir("T12000")

	for seed in xrange(10):
		classifier = RandomForestClassifier(n_estimators=12000, verbose=1, n_jobs=10, min_samples_split=10, random_state=seed)
		classifier.fit(features.todense(), target)

	 	predictions = classifier.predict_proba(test.todense())
		predictions = iter(predictions)

		fout = open(os.path.join("T12000","s%d.out"%(seed)),'w')
		for prob in predictions:
			fout.write("%f\n"%(prob[1]))
		fout.close()
		print("Seed: %d"%(seed))

	ranking = dict()
	for seed in range(10):
		with open(test_csv, 'rb') as csvfile, open(os.path.join("T12000","s%d.out"%(seed)),'r') as fin:
			reader = csv.reader(csvfile)
			reader.next()
			for row in reader:
				author = row[0]
				Papers = row[1].split()
				if not ranking.has_key(author):
					ranking[author] = dict()
				for paper in Papers:
					confidence = float(fin.readline().rstrip())
					if not ranking[author].has_key(paper):
						ranking[author][paper] = 0.0
					ranking[author][paper] += confidence


	with open(test_csv,'rb') as csvfile, open(predict,'w') as fout:
		reader = csv.reader(csvfile)
		reader.next()
		for row in reader:
			author = row[0]
			Papers = row[1].split()
			for paper in Papers:
				fout.write("%f\n"%(ranking[author][paper]))

if __name__=="__main__":
	main()
