#! /usr/bin/env python

import os
import function

def concate_features():
	os.chdir('features')
	excutable = '../util/concatenate 1 0'
	arguments = [' all_train.f Kmeans_train.f Simplex_train.f Astar_train.f BinarySearch_train.f',
		     ' all_valid.f Kmeans_valid.f Simplex_valid.f Astar_valid.f BinarySearch_valid.f']
	for arg in arguments:
		cmd = excutable + arg
		print cmd
		os.system(cmd)

def merge_train_and_valid(set_list, raw_dir):
	os.chdir('util')
	features_dir = 'home/features/'
	for name in set_list:
		cmd = 'java RowMerger {0} {1} {2} {3} {4}'.format(features_dir+name+'_train.f', features_dir+name+'_valid.f', raw_dir+'Valid.csv', raw_dir+'ValidSolution.csv', features_dir+name+'_train+valid.f')
		print cmd
		os.system(cmd)

def train():
	model_path = 'home/' + function.get_paths()['MODEL_DIR_PATH']
	os.chdir('train_and_predict')
	train_dir = os.getcwd()

	train_file = 'home/features/all_train+valid.f'
	ref_file = 'home/auxiliary_data/Train+Valid.csv'

	os.chdir('BinarySearch')
	cmd = './train.sh {0} {1} {2}BS.model'.format(train_file, ref_file, model_path)
	print cmd
	os.system(cmd)
	os.chdir(train_dir)
	
	os.chdir('Astar')
	cmd = 'python train.py {0} {1} {2}AS.model -m 1'.format(train_file, ref_file, model_path)
	print cmd
	os.system(cmd)
	os.chdir(train_dir)

def main():
	top_dir = os.getcwd()

	raw_dir = 'home/' + function.get_paths()['TRAIN_DATA_DIR_PATH']
	cmd = 'python util/gen_Train+Valid.py {0}Train.csv {0}Valid.csv {0}ValidSolution.csv auxiliary_data/Train+Valid.csv'.format(function.get_paths()['TRAIN_DATA_DIR_PATH'])
	print cmd
	os.system(cmd)
	
	function.generate_features(raw_dir, raw_dir+'Train.csv', raw_dir+'Valid.csv', '_train.f', '_valid.f')
	os.chdir(top_dir)
	
	concate_features()
	os.chdir(top_dir)
	
	set_list = ['all', 'Kmeans']
	merge_train_and_valid(set_list, raw_dir)
	os.chdir(top_dir)

	train()
	os.chdir(top_dir)
	
if __name__=="__main__":
	 main()
