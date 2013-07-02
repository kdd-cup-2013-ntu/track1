CXX ?= g++
CFLAGS = -Wall -Wconversion -O3

all: make_feature make_feature1 trans_feature1 trans_feature3 trans_feature4 trans_feature6 trans_feature7

make_feature: make_feature.cpp
	$(CXX) $(CFLAGS) make_feature.cpp -o make_feature
make_feature1: make_feature1.cpp
	$(CXX) $(CFLAGS) make_feature1.cpp -o make_feature1
trans_feature1: trans_feature1.cpp 
	$(CXX) $(CFLAGS) trans_feature1.cpp -o trans_feature1
trans_feature3: trans_feature3.cpp 
	$(CXX) $(CFLAGS) trans_feature3.cpp -o trans_feature3
trans_feature4: trans_feature4.cpp 
	$(CXX) $(CFLAGS) trans_feature4.cpp -o trans_feature4
trans_feature6: trans_feature6.cpp 
	$(CXX) $(CFLAGS) trans_feature6.cpp -o trans_feature6
trans_feature7: trans_feature7.cpp 
	$(CXX) $(CFLAGS) trans_feature7.cpp -o trans_feature7
clean:
	rm -f *~ make_feature make_feature1 trans_feature1 trans_feature3 trans_feature4 trans_feature6 trans_feature7
