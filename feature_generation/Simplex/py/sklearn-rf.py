#!/usr/bin/python
from sklearn.ensemble import RandomForestClassifier
import csv, sys
import numpy as np
from array import array
import math

SPLIT_CRITERION = 'entropy'
MIN_SAMPLE_SPLIT = 10
NUM_THREAD = 10

if len(sys.argv) != 6:
    print sys.argv[0], "train_file test_file ntree mtry output_file"
    exit(-1)

train_file_path = sys.argv[1]
test_file_path = sys.argv[2]
num_tree = int(sys.argv[3])
output_file_path = sys.argv[5]

importance_file_path = "./.imp"

train_x = []
train_y = []
test_x = []

with open(train_file_path, 'r') as train_file:
    train_data = csv.reader(train_file, delimiter=' ')
    for row in train_data:
        if str(row[-1]).__len__() == 0:
            train_x.append(row[1:-1])
        else:
            train_x.append(row[1:])
        train_y.append(row[0])

if sys.argv[4] == 'sqrt':
    mtry = math.sqrt(len(train_x[0]))
else:
    mtry = int(sys.argv[4])
"""
print train_x[0]
for row in train_x:
    for idx in xrange(0, len(row)):
        row[idx] = float(row[idx])
"""
print train_x[0]
#print train_y[0][0]
with open(test_file_path , 'r') as test_file:
    test_data = csv.reader(test_file, delimiter=' ')
    for row in test_data:
        if str(row[-1]).__len__() == 0:
            test_x.append(row[1:-1])
        else:            
            test_x.append(row[1:])

print "Data loaded!!"
forest = RandomForestClassifier(n_estimators=num_tree, criterion=SPLIT_CRITERION, 
        min_samples_split=MIN_SAMPLE_SPLIT, 
        n_jobs = NUM_THREAD,
        max_features=mtry,
#        oob_score = True,
        verbose = 5,
        random_state = 514,
        compute_importances = True
        );

forest.fit(train_x, train_y)
#train_acc = forest.score(train_x, train_y)
#print "Training Acc:", train_acc
importance = forest.feature_importances_ 
test_prob = forest.predict_proba(test_x)

with open(output_file_path, "w+") as output_file:
    for row in test_prob:
        output_file.write(str(row[0]) + '\n')



with open(importance_file_path, "w+") as importance_file:
    for row in importance:
        importance_file.write(str(row) + '\n')

