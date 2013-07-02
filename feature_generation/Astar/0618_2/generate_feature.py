import sys
import subprocess
import os

def TransCSV():
	cmd1 = 'python ./csv_trans.py ' + sys.argv[1]
	print cmd1
	os.system(cmd1)

def TransFeature():
	cmd = 'sh ./transFeature.sh ' + '../' + sys.argv[1]
	print cmd
	os.system(cmd)

def MakeFeature(TrainData,ValidData):
	cmd = 'sh ./makeFeature.sh ' + TrainData + ' ' + ValidData + ' ' + sys.argv[4] + ' ' + sys.argv[5]
	print cmd
	os.system(cmd)

def DenseToLibSVM(TrainData,ValidData):
	
	rd1_path = sys.argv[4] + '.dense' 
	rd2_path = sys.argv[5] + '.dense'
	wr1_fp = file(sys.argv[4],'w')
	wr2_fp = file(sys.argv[5],'w')
	rd1_fp = file(rd1_path,'r')
	rd2_fp = file(rd2_path,'r')
	
	for line in rd1_fp:
		token = line.strip().split(' ')
		label = token[0]
		feature = token[1:]
		wr1_fp.write('{0}'.format(label))
		for x in range(0, len(feature)):
			wr1_fp.write(' {0}:{1}'.format(x+1, feature[x]))
		wr1_fp.write('\n')

	for line in rd2_fp:
		token = line.strip().split(' ')
		label = token[0]
		feature = token[1:]
		wr2_fp.write('{0}'.format(label))
		for x in range(0, len(feature)):
			wr2_fp.write(' {0}:{1}'.format(x+1, feature[x]))
		wr2_fp.write('\n')

def main():
	
	TrainData = "../" + sys.argv[2]
	ValidData = "../" + sys.argv[3]
	TrainFeature = sys.argv[1] + sys.argv[4]
	ValidFeature = sys.argv[1] + sys.argv[5]
	TransCSV()
	TransFeature()
	MakeFeature(TrainData,ValidData)
	DenseToLibSVM(TrainData,ValidData)

if __name__ == '__main__':
	if len(sys.argv) < 6:
		sys.stderr.write("./generate_feature.py raw_data_directory train.csv valid.csv train.feature valid.feature\n")
		exit()
	main()

