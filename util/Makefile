all: RowMerger concatenate

RowMerger: RowMerger.java
	javac RowMerger.java

concatenate: concatenate.cpp
	g++ -O3 -o concatenate concatenate.cpp

clean:
	rm -f RowMerger.class
	rm -f concatenate
