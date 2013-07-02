#! /usr/bin/env python

import sys
import csv

official_train_file = open(sys.argv[1], 'r')
official_valid_file = open(sys.argv[2], 'r')
official_train_valid_file = open(sys.argv[3], 'r')
train_valid_file = open(sys.argv[4], 'w')
		
reader = csv.reader(official_train_valid_file)
header = reader.next()
confirmed = {}
for row in reader:
	aid = int(row[0])
	cpids = row[1].split()
	cpids = map(int, cpids)
	confirmed[aid] = cpids
official_train_valid_file.close()

for line in official_train_file:
	train_valid_file.write(line)
official_train_file.close()

reader = csv.reader(official_valid_file)
header = reader.next()
for row in reader:
	aid = int(row[0])
	pids = row[1].split()
	pids = map(int, pids)
	cpids = confirmed[aid]
	for cpid in cpids:
		pids.remove(cpid)
	train_valid_file.write('{0},{1}'.format(aid, cpids[0]))	
	for i in range(1, len(cpids)):
		train_valid_file.write(' {0}'.format(cpids[i]))	
	train_valid_file.write(',{0}'.format(pids[0]))	
	for i in range(1, len(pids)):
		train_valid_file.write(' {0}'.format(pids[i]))	
	train_valid_file.write('\n')	
official_train_valid_file.close()
train_valid_file.close()
