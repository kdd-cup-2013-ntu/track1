#!/usr/bin/python

import csv, optparse, sys, random

def main():
	parser = optparse.OptionParser()
	parser.add_option("-f",dest="feature_file",help="Specify feature file.")
	parser.add_option("-o",dest="output_file",help="Specify output file.")
	parser.add_option("-q",dest="query_file",help="Specify query file.")
	options, args = parser.parse_args(sys.argv[1:])

	fout = open(options.output_file,'w')
	fin = open(options.feature_file,'r')
	authors = 0
	with open(options.query_file, 'rb') as csvfile:
		reader = csv.reader(csvfile)
		reader.next()
		papers = 0
	        for row in reader:
			authors += 1
			author = row[0]
			Confirmed = row[1].split()
			instances = list()
			for paper in Confirmed:
				label, feature = fin.readline().rstrip().split(" ",1)
				instances.append("2 qid:%d %s\n"%(authors,feature))
			Deleted = row[2].split()
			for paper in Deleted:
				label, feature = fin.readline().rstrip().split(" ",1)
				instances.append("1 qid:%d %s\n"%(authors,feature))
			random.shuffle(instances)
			for instance in instances:
				fout.write(instance)
	fout.close()

if __name__=="__main__":
	main()
