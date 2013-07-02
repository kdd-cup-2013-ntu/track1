#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string.h>
#include <utility>
#include <algorithm>

using namespace std;

bool ap_comp(pair<int,int> p1, pair<int,int> p2){
	if (p1.first != p2.first)
		return p1.first < p2.first;
	return p1.second < p2.second;
}

int main(){
	FILE *fin, *fout;
	int aid, pid, i=0;
	vector< pair<int,int> > all_v;
	fin = fopen("all_list.txt", "r");
	fout = fopen("ap_list.txt", "w");
	while(fscanf(fin, "%d %d", &aid, &pid) != EOF){
		all_v.push_back(make_pair(aid, pid));
		i = i+1;
	}
	cout << i << endl;
	sort(all_v.begin(), all_v.end(), ap_comp);
	for (i=0; i<all_v.size(); ++i)
		fprintf(fout, "%d %d\n", all_v[i].first, all_v[i].second);
	return 0;
}
