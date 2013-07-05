CXX ?= g++
CFLAGS = -Wall -Wconversion -O3

all: check_dir trans_feature1 trans_feature2

check_dir:
	mkdir -p mydata

trans_feature1: trans_feature1.cpp strcmp95.o
	$(CXX) $(CFLAGS) trans_feature1.cpp strcmp95.o -o trans_feature1
trans_feature2: trans_feature2.cpp strcmp95.o
	$(CXX) $(CFLAGS) trans_feature2.cpp strcmp95.o -o trans_feature2

clean:
	rm -f *~ trans_feature1 trans_feature2 
