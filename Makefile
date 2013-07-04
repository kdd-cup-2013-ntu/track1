all:
	mkdir -p raw_data
	mkdir -p features
	mkdir -p models
	mkdir -p scores
	make -C util/ all
	make -C ensemble/ all
	make -C feature_generation/Astar/0618_2/code/ all
	make -C feature_generation/Astar/0618_2/code1/ all
clean:
	make clean -C ensemble/
	make clean -C util/
	make clean -C feature_generation/Astar/0618_2/code/
	make clean -C feature_generation/Astar/0618_2/code1/
