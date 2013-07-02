#!/usr/bin/python

import csv, optparse, sys, os
import numpy as np

def apk(actual, predicted, k):
	if len(predicted)>k:
		predicted = predicted[:k]

	score = 0.0
	num_hits = 0.0

	actualset = set(actual)
	for i,p in enumerate(predicted):
		if p in actualset and p not in predicted[:i]:
			num_hits += 1.0
			score += num_hits / (i+1.0)
	if not actual:
		return 1.0
	return score / min(len(actual), k)

def main():
	predict = sys.argv[1]
	query_file = sys.argv[2]
	groundtruth = sys.argv[3]
	seednumber = int(sys.argv[4])

	ranking = dict()
	for i in range(seednumber):
		with open(query_file, 'rb') as csvfile, open(os.path.join(predict,"s%d.out"%(i)),'r') as fin:
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

	fout = open("Prediction.csv",'w')
	fout.write("AuthorId,PaperIds\n")

	ap = list()
	with open(groundtruth,'rb') as groundtruthfile:
		groundtruthreader = csv.reader(groundtruthfile)
		groundtruthreader.next()
		for row in groundtruthreader:
			author = row[0]
			result = sorted(ranking[author],key=ranking[author].get,reverse=True)
			ap.append(apk(row[1].split(),result,10000))
			fout.write("%s,"%(author))
			for paper in result:
				fout.write("%s "%(paper))
			fout.write("\n")
	print np.mean(ap)

if __name__=="__main__":
	main()
