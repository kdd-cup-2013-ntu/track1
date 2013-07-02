#! /usr/bin/env python

import os
import function

def concate_features():
	os.chdir('features')
	excutable = '../util/concatenate 1 0'
	arguments = [' all_test.f Kmeans_test.f Simplex_test.f Astar_test.f BinarySearch_test.f']
	for arg in arguments:
		cmd = excutable + arg
		print cmd
		os.system(cmd)

def predict(test_path):
	model_path = 'home/' + function.get_paths()['MODEL_DIR_PATH']
	os.chdir('train_and_predict')
	train_dir = os.getcwd()

	test_file = 'home/features/all_test.f'

	os.chdir('Kmeans')
	cmd = 'python train_predict.py home/features/Kmeans_train+valid.f home/features/Kmeans_test.f {0} home/scores/K.score'.format(test_path)
	print cmd
	os.system(cmd)
	os.chdir(train_dir)
	
	os.chdir('BinarySearch')
	cmd = './predict.sh {0} {1} {2}BS.model home/scores/BS.score'.format(test_file, test_path, model_path)
	print cmd
	os.system(cmd)
	os.chdir(train_dir)
	
	os.chdir('Astar')
	cmd = 'python predict.py {0} {1} {2}AS.model home/scores/AS.score'.format(test_file, test_path, model_path)
	print cmd
	os.system(cmd)
	os.chdir(train_dir)

def weighted_average():
	os.chdir('ensemble')
	cmd = 'java Ensemble {0}AS.score:1.0 {0}BS.score:1.0 {0}K.score:5.0 {0}final.score'.format('home/scores/')
	print cmd
	os.system(cmd)

def postprocess(test_path):
	submit_path = 'home/' + function.get_paths()['SUBMISSION_PATH']
	os.chdir('post_processing')
	cmd = 'sh ./post_processing.sh {0} home/features/Astar_test.f home/scores/final.score {1}'.format(test_path, submit_path)
	print cmd
	os.system(cmd)
	

def main():

	top_dir = os.getcwd()
	raw_dir = 'home/' + function.get_paths()['TRAIN_DATA_DIR_PATH']
	aux_dir = 'home/auxiliary_data/'
	test_path = 'home/' + function.get_paths()['TEST_DATA_PATH']
	function.generate_features(raw_dir, aux_dir+'dummyTrain.csv', test_path, '_null.f', '_test.f')
	os.chdir(top_dir)
	
	concate_features()
	os.chdir(top_dir)
	
	predict(test_path)
	os.chdir(top_dir)
	
	weighted_average()
	os.chdir(top_dir)

	postprocess(test_path)
	os.chdir(top_dir)
	
if __name__=="__main__":
	 main()
