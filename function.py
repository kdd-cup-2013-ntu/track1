import os
import json

def get_paths():
	paths = json.loads(open('SETTINGS.json').read())
	for key in paths:
		paths[key] = os.path.expandvars(paths[key])
	return paths

def generate_features(raw_dir, train, valid, train_f, valid_f):
	features_dir = 'home/features/'
	print 'Start generating features.'
	os.chdir('feature_generation')
	feature_gen_dir = os.getcwd()

	os.chdir('Simplex')
	cmd = 'make clean'
	os.system(cmd)
	os.chdir(feature_gen_dir)
	
	py_gen = 'python ./generate_feature.py'
	teams = [('Kmeans', 'Kmeans/Feature/20130616_6', py_gen),
		('Simplex', 'Simplex', py_gen), 
		('BinarySearch', 'BinarySearch/Feature', './generate_feature.sh'),
		('Astar', 'Astar/0618_2', py_gen)]
	for tup in teams:
		team = tup[0]
		os.chdir(tup[1])
		excutable = tup[2]
		cmd = '{5} {0} {1} {2} {3} {4}'.format(raw_dir, train, valid, features_dir+team+train_f, features_dir+team+valid_f, excutable)
		print cmd
		os.system(cmd)
		os.chdir(feature_gen_dir)

