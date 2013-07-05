all:
	mkdir -p raw_data
	mkdir -p features
	mkdir -p models
	mkdir -p scores
	make -C util/ all
	make -C ensemble/ all
	make -C feature_generation/Simplex/ check-dir
	mkdir -p feature_generation/BinarySearch/Feature/KDDCUP2013/data
	make -C feature_generation/Astar/0618_2/ all
	make -C train_and_predict/BinarySearch/ all
clean:
	make clean -C ensemble/
	make clean -C util/
	make clean -C feature_generation/Astar/0618_2/
