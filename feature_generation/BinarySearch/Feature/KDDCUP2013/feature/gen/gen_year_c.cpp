#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <math.h>
#include <algorithm>

using namespace std;

#define YEAR_UB 2013.5
#define YEAR_LB 1799.5
#define Y_RANGE 50.5
#define A_NUM 2300000
#define P_NUM 2300000
#define C_NUM 6000
#define J_NUM 23000

//to change stopping criterion, change line 183

class py {
	public:
		int pid;
		double year;
		py(int,double);
};

py::py(int _pid, double _year){
	pid = _pid;
	year = _year;
}

vector<int> p2c, p2j, p2a[P_NUM], p2ry;
vector<double> p2y, _p2y, a2y_mean, _a2y_mean, a2y_stdev, a2y_min, a2y_max, c2y_mean, c2y_stdev, c2y_min, c2y_max, j2y_mean, j2y_stdev, j2y_min, j2y_max;
vector<py> a2py[A_NUM], c2py[C_NUM], j2py[J_NUM];
vector<bool> p2y_valid;
map< pair<int,int>, vector<py> > ac2py, aj2py;
map< pair<int,int>, double> ac2y_mean, ac2y_stdev, aj2y_mean, aj2y_stdev, ac2y_min, ac2y_max, aj2y_min, aj2y_max;
double e_a=10000, e_p=10000;

bool cmp(py py1, py py2){
	return py1.year<py2.year;
}

//information list
FILE *fpa = fopen("../list/pa_list.txt", "r");
FILE *fap = fopen("../list/ap_list.txt", "r");
FILE *fp = fopen("../list/paper_list.txt", "r");

void init();
void year_iter();
void gen_my(FILE *fin[2]);
void gen_my_mean(FILE *fin[2]);
void gen_my_stdev(FILE *fin[2]);
void gen_myc_mean(FILE *fin[2]);
void gen_myc_stdev(FILE *fin[2]);
void gen_myj_mean(FILE *fin[2]);
void gen_myj_stdev(FILE *fin[2]);
void gen_camycj_mean_stdev(FILE* fin[2]);
void gen_ry(FILE *fin[2]);
void gen_y_valid(FILE *fin[2]);
void gen_my_min(FILE *fin[2]);
void gen_my_max(FILE *fin[2]);
void gen_myc_min(FILE *fin[2]);
void gen_myc_max(FILE *fin[2]);
void gen_myj_min(FILE *fin[2]);
void gen_myj_max(FILE *fin[2]);
void gen_c_mean(FILE *fin[2]);
void gen_c_stdev(FILE *fin[2]);
void gen_c_min(FILE *fin[2]);
void gen_c_max(FILE *fin[2]);
void gen_j_mean(FILE *fin[2]);
void gen_j_stdev(FILE *fin[2]);
void gen_j_min(FILE *fin[2]);
void gen_j_max(FILE *fin[2]);
bool is_valid(double y);

int main(){
	int i;
	init();
	//(sub)train/val list
	//order 0,1,2,3 = train, valid, subtrain, subvalid
	FILE *fin[2];
	FILE *fpy;
	fin[0] = fopen("../list/cd_list.txt", "r");
	fin[1] = fopen("../list/val_list.txt", "r");
	fpy = fopen("../list/paper_year.txt", "w");
	for (i=0; i<P_NUM; ++i)
		fprintf(fpy,"%d %lf\n", i, p2y[i]);
	//modified year features v3
	gen_my(fin);
	gen_my_mean(fin);
	gen_my_stdev(fin);
	gen_myc_mean(fin);
	gen_myc_stdev(fin);
	gen_myj_mean(fin);
	gen_myj_stdev(fin);
	gen_ry(fin);
	gen_y_valid(fin);
	gen_my_min(fin);
	gen_my_max(fin);
	gen_myc_min(fin);
	gen_myc_max(fin);
	gen_myj_min(fin);
	gen_myj_max(fin);
	gen_c_mean(fin);
	gen_c_stdev(fin);
	gen_c_min(fin);
	gen_c_max(fin);
	gen_j_mean(fin);
	gen_j_stdev(fin);
	gen_j_min(fin);
	gen_j_max(fin);
	//gen_camycj_mean_stdev(fin);
}

void init(){
	int i, p, c, j, a, k, y, y_num=0;
	double y_mean=0.000;
	for (i=0; i<2300000; ++i){
		p2c.push_back(0);
		p2j.push_back(0);
		//no -2's in data
		p2y.push_back(-2);
		_p2y.push_back(-2);
		a2y_mean.push_back(-1);
		_a2y_mean.push_back(-1);
		a2y_stdev.push_back(0);
		p2y_valid.push_back(false);
		p2ry.push_back(-3);
		a2y_min.push_back(YEAR_UB);
		a2y_max.push_back(YEAR_LB);
	}
	for (i=0; i<C_NUM; ++i){
		c2y_mean.push_back(-1);
		c2y_stdev.push_back(0);
		c2y_min.push_back(YEAR_UB);
		c2y_max.push_back(YEAR_LB);
	}
	for (i=0; i<J_NUM; ++i){
		j2y_mean.push_back(-1);
		j2y_stdev.push_back(0);
		j2y_min.push_back(YEAR_UB);
		j2y_max.push_back(YEAR_LB);
	}
	i = 0;
	while (fscanf(fp, "%d %d %d %d", &p, &y, &c, &j) > 0){
		if (i % 100000 == 0)
			cerr << ".";
		if (c > 0)
			p2c[p] = c;
		if (j > 0)
			p2j[p] = j;
		if (is_valid(y)){
			p2y[p] = (double)y;
			y_mean += (double)y;
			++y_num;
			p2y_valid[p] = true;
		}
		else
			p2y[p] = -3;
		p2ry[p] = y;
		++i;
	}
	rewind(fp);
	i = 0;
	y_mean = y_mean/double(y_num);
	cerr << endl << "mean year: " << y_mean;
	cerr << endl << "paper to conference/journal list done" << endl;
	cerr << "reading author paper list" << endl;
	while (fscanf(fap, "%d %d", &a, &p) > 0){
		if (i % 1000000 == 0)
			cerr << ".";
		a2py[a].push_back(py(p,p2y[p]));
		p2a[p].push_back(a);
		++i;
	}
	rewind(fap);
	cerr << endl;
	i = 0;
	//get author year mean and paper year
	while (e_p>0.00001){//paper year RMSE
		cerr << "======================================" << endl;
		cerr << "iter: " << i << endl;
		year_iter();
		cerr << "======================================" << endl;
		++i;
	}
	y_num = 0;
	y_mean = 0.000;
	for (i=0; i<P_NUM; ++i)
		if (is_valid(p2y[i])){
			++y_num;
			y_mean += p2y[i];
		}
	y_mean /= double(y_num);
	cerr << "mean year:" << y_mean << endl;
	for (i=0; i<P_NUM; ++i)
		if (!is_valid(p2y[i])){
			p2y[i] = y_mean;
			for (j=0; j<p2a[i].size(); ++j)
				for (k=0; k<a2py[p2a[i][j]].size(); ++k)
					if (a2py[p2a[i][j]][k].pid == i)
						a2py[p2a[i][j]][k].year = p2y[i];
		}
	cerr << "gen ac2py and aj2py" << endl;
	for (i=0; i<A_NUM; ++i){
		if (i%100000 == 0)
			cerr << ".";
		int num=0;
		double sum=0, sqr_sum=0;
		sort(a2py[i].begin(), a2py[i].end(), cmp);
		for (j=a2py[i].size()-1; j>=0 && is_valid(a2py[i][j].year); --j){
			++num;
			sum += a2py[i][j].year;
			sqr_sum += a2py[i][j].year*a2py[i][j].year;
			if (a2py[i][j].year < a2y_min[i])
				a2y_min[i] = a2py[i][j].year;
			if (a2py[i][j].year > a2y_max[i])
				a2y_max[i] = a2py[i][j].year;
			pair<int,int> ac = make_pair(i,p2c[a2py[i][j].pid]);
			pair<int,int> aj = make_pair(i,p2j[a2py[i][j].pid]);
			if (p2c[a2py[i][j].pid] > 0){
				if (a2py[i][j].year < c2y_min[p2c[a2py[i][j].pid]])
					c2y_min[p2c[a2py[i][j].pid]] = a2py[i][j].year;
				if (a2py[i][j].year > c2y_max[p2c[a2py[i][j].pid]])
					c2y_max[p2c[a2py[i][j].pid]] = a2py[i][j].year;
				c2py[p2c[a2py[i][j].pid]].push_back(a2py[i][j]);
				if (ac2py.count(ac)>0)
					ac2py[ac].push_back(a2py[i][j]);
				else{
					ac2py.insert(make_pair(ac,vector<py>()));
					ac2py[ac].push_back(a2py[i][j]);
				}
			}
			if (p2j[a2py[i][j].pid] > 0){
				if (a2py[i][j].year < j2y_min[p2j[a2py[i][j].pid]])
					j2y_min[p2j[a2py[i][j].pid]] = a2py[i][j].year;
				if (a2py[i][j].year > j2y_max[p2j[a2py[i][j].pid]])
					j2y_max[p2j[a2py[i][j].pid]] = a2py[i][j].year;
				j2py[p2j[a2py[i][j].pid]].push_back(a2py[i][j]);
				if (aj2py.count(aj)>0)
					aj2py[aj].push_back(a2py[i][j]);
				else{
					aj2py.insert(make_pair(aj,vector<py>()));
					aj2py[aj].push_back(a2py[i][j]);
				}
			}
		}
		if (num>0){
			a2y_mean[i] = sum/double(num);
			a2y_stdev[i] = sqrt(sqr_sum/double(num) - a2y_mean[i]*a2y_mean[i]);
		}
		else{
			a2y_mean[i] = y_mean;
			a2y_stdev[i] = 0;
		}
	}
	map< pair<int,int>, vector<py> >::iterator it;
	i = 0;
	cerr << endl << "insert ac2y" << endl;
	for (it=ac2py.begin(); it!=ac2py.end(); ++it){
		if (i%1000 == 0)
			cerr << ".";
		double sum=0, sqr_sum=0, min=YEAR_UB, max=YEAR_LB;
		for (i=0; i<(it->second).size(); ++i){
			sum += (it->second)[i].year;
			sqr_sum += (it->second)[i].year*(it->second)[i].year;
			if ((it->second)[i].year < min)
				min = (it->second)[i].year;
			if ((it->second)[i].year > max)
				max = (it->second)[i].year;
		}
		double num = double((it->second).size());
		ac2y_mean.insert(make_pair(it->first,sum/num));
		ac2y_stdev.insert(make_pair(it->first,sqrt(sqr_sum/num - sum*sum/num/num)));
		ac2y_min.insert(make_pair(it->first,min));
		ac2y_max.insert(make_pair(it->first,max));
		++i;
	}
	i = 0;
	cerr << endl << "insert aj2y" << endl;
	for (it=aj2py.begin(); it!=aj2py.end(); ++it){
		if (i%1000 == 0)
			cerr << ".";
		double sum=0, sqr_sum=0, min=YEAR_UB, max=YEAR_LB;
		for (i=0; i<(it->second).size(); ++i){
			sum += (it->second)[i].year;
			sqr_sum += (it->second)[i].year*(it->second)[i].year;
			if ((it->second)[i].year < min)
				min = (it->second)[i].year;
			if ((it->second)[i].year > max)
				max = (it->second)[i].year;
		}
		double num = double((it->second).size());
		aj2y_mean.insert(make_pair(it->first,sum/num));
		aj2y_stdev.insert(make_pair(it->first,sqrt(sqr_sum/num - sum*sum/num/num)));
		aj2y_min.insert(make_pair(it->first,min));
		aj2y_max.insert(make_pair(it->first,max));
		++i;
	}
	for (i=0; i<C_NUM; ++i)
		if (c2py[i].size()>0){
			double sum=0, sqr_sum=0;
			for (j=0; j<c2py[i].size(); ++j){
				sum += c2py[i][j].year;
				sqr_sum += c2py[i][j].year*c2py[i][j].year;
			}
			c2y_mean[i] = sum/double(c2py[i].size());
			c2y_stdev[i] = sqrt(sqr_sum/double(c2py[i].size()) - c2y_mean[i]*c2y_mean[i]);
		}
	for (i=0; i<J_NUM; ++i)
		if (j2py[i].size()>0){
			double sum=0, sqr_sum=0;
			for (j=0; j<j2py[i].size(); ++j){
				sum += j2py[i][j].year;
				sqr_sum += j2py[i][j].year*j2py[i][j].year;
			}
			j2y_mean[i] = sum/double(j2py[i].size());
			j2y_stdev[i] = sqrt(sqr_sum/double(j2py[i].size()) - j2y_mean[i]*j2y_mean[i]);
		}
	cerr << endl << "init done" << endl;
}

void year_iter(){
	//calculate author's mean year
	int i, j, k;
	cerr << "cram author mean year" << endl;
	for (i=0; i<A_NUM; ++i){
		if (i % 100000 == 0)
			cerr << ".";
		sort(a2py[i].begin(), a2py[i].end(), cmp);
		int num = 0;
		double sum = 0;
		for (j=a2py[i].size()-1; j>=0 && is_valid(a2py[i][j].year); --j){
			++num;
			sum += a2py[i][j].year;
		}
		if (num > 0)
			a2y_mean[i] = sum/double(num);
	}
	cerr << endl << "cram invalid paper year" << endl;
	for (i=0; i<P_NUM; ++i){
		if (i % 100000 == 0)
			cerr << ".";
		if (!p2y_valid[i] && (p2a[i].size()>0)){
			int num = 0;
			double sum = 0;
			for (j=0; j<p2a[i].size(); ++j)
				if (is_valid(a2y_mean[p2a[i][j]])){
					++num;
					sum += a2y_mean[p2a[i][j]];
				}
			if (num > 0)
				p2y[i] = sum/double(num);
			for (j=0; j<p2a[i].size(); ++j)
				for (k=0; k<a2py[p2a[i][j]].size(); ++k)
					if (a2py[p2a[i][j]][k].pid == i)
						a2py[p2a[i][j]][k].year = p2y[i];
		}
	}

	int num=0;
	e_a=0;
	e_p=0;
	//author mean year error
	for (i=0; i<A_NUM; ++i)
		if (is_valid(a2y_mean[i])){
			e_a += (a2y_mean[i] - _a2y_mean[i])*(a2y_mean[i] - _a2y_mean[i]);
			++num;
		}
	e_a = sqrt(e_a/num);
	cerr << endl << "mean author year rmse: " << e_a << endl;
	_a2y_mean.clear();
	for (i=0; i<A_NUM; ++i)
		_a2y_mean.push_back(a2y_mean[i]);
	//paper year error
	num = 0;
	for (i=0; i<P_NUM; ++i)
		if (!p2y_valid[i] && p2a[i].size()>0 && is_valid(p2y[i])){
			e_p += (p2y[i] - _p2y[i])*(p2y[i] - _p2y[i]);
			++num;
		}
	e_p = sqrt(e_p/num);
	cerr << "paper year rmse: " << e_p << endl;
	_p2y.clear();
	for (i=0; i<P_NUM; ++i)
		_p2y.push_back(p2y[i]);
}

bool is_valid(double y){
	return (YEAR_LB<y && y<YEAR_UB);
}

//modified paper's year
void gen_my(FILE *fin[2]){
	cerr << "generate modified year per paper" << endl;
	int i, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/my_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%*d %d", &p) > 0)
			fprintf(fout[i], "%lf\n", p2y[p]);
	}
	cerr << "done" << endl;
}

//raw paper's year
void gen_ry(FILE *fin[2]){
	cerr << "generate raw year per paper" << endl;
	int i, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/ry%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%*d %d", &p) > 0)
			fprintf(fout[i], "%d\n", p2ry[p]);
	}
	cerr << "done" << endl;
}

//paper's year valid
void gen_y_valid(FILE *fin[2]){
	cerr << "generate year validity per paper" << endl;
	int i, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/yv%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%*d %d", &p) > 0){
			if (p2y_valid[p])
				fprintf(fout[i], "1\n");
			else
				fprintf(fout[i], "0\n");
		}
	}
	cerr << "done" << endl;
}

//modified year mean per author
void gen_my_mean(FILE *fin[2]){
	cerr << "generate modified year mean per author" << endl;
	int i, a;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/my_mean_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %*d", &a) > 0)
			fprintf(fout[i], "%lf\n", a2y_mean[a]);
	}
	cerr << "done" << endl;
}

//modified year min per author
void gen_my_min(FILE *fin[2]){
	cerr << "generate modified year min per author" << endl;
	int i, a;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/my_min_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %*d", &a) > 0)
			fprintf(fout[i], "%lf\n", a2y_min[a]);
	}
	cerr << "done" << endl;
}

//modified year max per author
void gen_my_max(FILE *fin[2]){
	cerr << "generate modified year max per author" << endl;
	int i, a;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/my_max_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %*d", &a) > 0)
			fprintf(fout[i], "%lf\n", a2y_max[a]);
	}
	cerr << "done" << endl;
}

//modified year stdev per author
void gen_my_stdev(FILE *fin[2]){
	cerr << "generate modified year stdev per author" << endl;
	int i, a;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/my_stdev_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %*d", &a) > 0)
			fprintf(fout[i], "%lf\n", a2y_stdev[a]);
	}
	cerr << "done" << endl;
}

//modified year mean per (author, conference)
void gen_myc_mean(FILE *fin[2]){
	cerr << "generate modified year mean per (author, conference)" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/myc_mean_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2c[p] > 0 && ac2y_mean.count(make_pair(a,p2c[p]))>0)
				fprintf(fout[i], "%lf\n", ac2y_mean[make_pair(a,p2c[p])]);
			else
				fprintf(fout[i], "%lf\n", a2y_mean[a]);
		}
	}
	cerr << "done" << endl;
}

//modified year min per (author, conference)
void gen_myc_min(FILE *fin[2]){
	cerr << "generate modified year min per (author, conference)" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/myc_min_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2c[p] > 0 && ac2y_min.count(make_pair(a,p2c[p]))>0)
				fprintf(fout[i], "%lf\n", ac2y_min[make_pair(a,p2c[p])]);
			else
				fprintf(fout[i], "%lf\n", p2y[p]);
		}
	}
	cerr << "done" << endl;
}

//modified year max per (author, conference)
void gen_myc_max(FILE *fin[2]){
	cerr << "generate modified year max per (author, conference)" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/myc_max_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2c[p] > 0 && ac2y_max.count(make_pair(a,p2c[p]))>0)
				fprintf(fout[i], "%lf\n", ac2y_max[make_pair(a,p2c[p])]);
			else
				fprintf(fout[i], "%lf\n", p2y[p]);
		}
	}
	cerr << "done" << endl;
}

//modified paper's year stdev per (author, conference)
void gen_myc_stdev(FILE *fin[2]){
	cerr << "generate modified year stdev per (author, conference)" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/myc_stdev_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2c[p] > 0 && ac2y_mean.count(make_pair(a,p2c[p]))>0)
				fprintf(fout[i], "%lf\n", ac2y_stdev[make_pair(a,p2c[p])]);
			else
				fprintf(fout[i], "0\n");
		}
	}
	cerr << "done" << endl;
}

//modified year mean per (author, journal)
void gen_myj_mean(FILE *fin[2]){
	cerr << "generate modified year mean per (author, journal)" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/myj_mean_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2j[p] > 0 && aj2y_mean.count(make_pair(a,p2j[p]))>0)
				fprintf(fout[i], "%lf\n", aj2y_mean[make_pair(a,p2j[p])]);
			else
				fprintf(fout[i], "%lf\n", a2y_mean[a]);
		}
	}
	cerr << "done" << endl;
}

//modified year min per (author, journal)
void gen_myj_min(FILE *fin[2]){
	cerr << "generate modified year min per (author, journal)" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/myj_min_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2j[p] > 0 && aj2y_min.count(make_pair(a,p2j[p]))>0)
				fprintf(fout[i], "%lf\n", aj2y_min[make_pair(a,p2j[p])]);
			else
				fprintf(fout[i], "%lf\n", p2y[p]);
		}
	}
	cerr << "done" << endl;
}

//modified year max per (author, journal)
void gen_myj_max(FILE *fin[2]){
	cerr << "generate modified year max per (author, journal)" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/myj_max_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2j[p] > 0 && aj2y_max.count(make_pair(a,p2j[p]))>0)
				fprintf(fout[i], "%lf\n", aj2y_max[make_pair(a,p2j[p])]);
			else
				fprintf(fout[i], "%lf\n", p2y[p]);
		}
	}
	cerr << "done" << endl;
}

//modified paper's year stdev per (author, journal)
void gen_myj_stdev(FILE *fin[2]){
	cerr << "generate modified year stdev per (author, journal)" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/myj_stdev_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2j[p] > 0 && aj2y_mean.count(make_pair(a,p2j[p]))>0)
				fprintf(fout[i], "%lf\n", aj2y_stdev[make_pair(a,p2j[p])]);
			else
				fprintf(fout[i], "0\n");
		}
	}
	cerr << "done" << endl;
}

//modified paper's year mean per conference
void gen_c_mean(FILE *fin[2]){
	cerr << "generate modified year mean per conference" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/c_mean_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2c[p] > 0)
				fprintf(fout[i], "%lf\n", c2y_mean[p2c[p]]);
			else
				fprintf(fout[i], "%lf\n", p2y[p]);
		}
	}
	cerr << "done" << endl;
}

//modified paper's year min per conference
void gen_c_min(FILE *fin[2]){
	cerr << "generate modified year min per conference" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/c_min_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2c[p] > 0)
				fprintf(fout[i], "%lf\n", c2y_min[p2c[p]]);
			else
				fprintf(fout[i], "%lf\n", p2y[p]);
		}
	}
	cerr << "done" << endl;
}

//modified paper's year max per conference
void gen_c_max(FILE *fin[2]){
	cerr << "generate modified year max per conference" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/c_max_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2c[p] > 0)
				fprintf(fout[i], "%lf\n", c2y_max[p2c[p]]);
			else
				fprintf(fout[i], "%lf\n", p2y[p]);
		}
	}
	cerr << "done" << endl;
}

//modified paper's year stdev per conference
void gen_c_stdev(FILE *fin[2]){
	cerr << "generate modified year stdev per conference" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/c_stdev_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2c[p] > 0)
				fprintf(fout[i], "%lf\n", c2y_stdev[p2c[p]]);
			else
				fprintf(fout[i], "0\n");
		}
	}
	cerr << "done" << endl;
}

//modified paper's year mean per journal
void gen_j_mean(FILE *fin[2]){
	cerr << "generate modified year mean per journal" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/j_mean_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2j[p] > 0)
				fprintf(fout[i], "%lf\n", j2y_mean[p2j[p]]);
			else
				fprintf(fout[i], "%lf\n", p2y[p]);
		}
	}
	cerr << "done" << endl;
}

//modified paper's year min per journal
void gen_j_min(FILE *fin[2]){
	cerr << "generate modified year min per journal" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/j_min_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2j[p] > 0)
				fprintf(fout[i], "%lf\n", j2y_min[p2j[p]]);
			else
				fprintf(fout[i], "%lf\n", p2y[p]);
		}
	}
	cerr << "done" << endl;
}

//modified paper's year max per journal
void gen_j_max(FILE *fin[2]){
	cerr << "generate modified year max per journal" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/j_max_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2j[p] > 0)
				fprintf(fout[i], "%lf\n", j2y_max[p2j[p]]);
			else
				fprintf(fout[i], "%lf\n", p2y[p]);
		}
	}
	cerr << "done" << endl;
}

//modified paper's year stdev per journal
void gen_j_stdev(FILE *fin[2]){
	cerr << "generate modified year stdev per journal" << endl;
	int i, a, p;
	FILE *fout[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/j_stdev_c_%d", i);
		fout[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2j[p] > 0)
				fprintf(fout[i], "%lf\n", j2y_stdev[p2j[p]]);
			else
				fprintf(fout[i], "0\n");
		}
	}
	cerr << "done" << endl;
}

void gen_camycj_mean_stdev(FILE *fin[2]){
	cerr << "generate co-authors' modified year mean/stdev per (author, paper)" << endl;
	int i, j, k, a, p, num, ln, y_num;
	double y_mean, y_stdev, yc_mean, yc_stdev, yj_mean, yj_stdev;
	FILE *fy_mean[2];
	FILE *fy_stdev[2];
	FILE *fyc_mean[2];
	FILE *fyc_stdev[2];
	FILE *fyj_mean[2];
	FILE *fyj_stdev[2];
	char str[1000];
	for (i=0; i<2; ++i){
		sprintf(str, "../list/camy_mean_v3_%d", i);
		fy_mean[i] = fopen(str, "w");
		sprintf(str, "../list/camy_stdev_v3_%d", i);
		fy_stdev[i] = fopen(str, "w");
		sprintf(str, "../list/camyc_mean_v3_%d", i);
		fyc_mean[i] = fopen(str, "w");
		sprintf(str, "../list/camyc_stdev_v3_%d", i);
		fyc_stdev[i] = fopen(str, "w");
		sprintf(str, "../list/camyj_mean_v3_%d", i);
		fyj_mean[i] = fopen(str, "w");
		sprintf(str, "../list/camyj_stdev_v3_%d", i);
		fyj_stdev[i] = fopen(str, "w");
		rewind(fin[i]);
		while (fscanf(fin[i], "%d %d", &a, &p) > 0){
			if (p2a[p].size()<=1){
				fprintf(fy_mean[i], "%lf\n", a2y_mean[a]);
				fprintf(fy_stdev[i], "0\n");
				if (p2c[p]>0){
					fprintf(fyc_mean[i], "%lf\n", ac2y_mean[make_pair(a,p2c[p])]);
					fprintf(fyc_stdev[i], "%lf\n", ac2y_stdev[make_pair(a,p2c[p])]);
				}
				else{
					fprintf(fyc_mean[i], "%lf\n", a2y_mean[a]);
					fprintf(fyc_stdev[i], "0\n");
				}
				if (p2j[p]>0){
					fprintf(fyj_mean[i], "%lf\n", aj2y_mean[make_pair(a,p2j[p])]);
					fprintf(fyj_stdev[i], "%lf\n", aj2y_stdev[make_pair(a,p2j[p])]);
				}
				else{
					fprintf(fyj_mean[i], "%lf\n", a2y_mean[a]);
					fprintf(fyj_stdev[i], "0\n");
				}
				continue;
			}
			y_mean = 0.000;
			y_stdev = 0.000;
			yc_mean = 0.000;
			yc_stdev = 0.000;
			yj_mean = 0.000;
			yj_stdev = 0.000;
			y_num = 0;
			for (j=0; j<p2a[p].size(); ++j){
				if (a==p2a[p][j])
					continue;
				y_mean += a2y_mean[p2a[p][j]];
				y_stdev += a2y_mean[p2a[p][j]]*a2y_mean[p2a[p][j]];
				++y_num;
			}
			if (y_num==0){
				y_mean = a2y_mean[a];
				y_stdev = 0.000;
			}
			else{
				y_mean = y_mean/double(y_num);
				y_stdev = sqrt(y_stdev/double(y_num)-y_mean*y_mean);
			}
			fprintf(fy_mean[i], "%lf\n", y_mean);
			fprintf(fy_stdev[i], "%lf\n", y_stdev);
			if (p2c[p]>0){
				y_num = 0;
				for (j=0; j<p2a[p].size(); ++j){
					if (a==p2a[p][j])
						continue;
					yc_mean += ac2y_mean[make_pair(p2a[p][j],p2c[p])];
					yc_stdev += ac2y_mean[make_pair(p2a[p][j],p2c[p])]*ac2y_mean[make_pair(p2a[p][j],p2c[p])];
					++y_num;
				}
				if (y_num==0){
					fprintf(fyc_mean[i], "%lf\n", y_mean);
					fprintf(fyc_stdev[i], "%lf\n", y_stdev);
				}
				else{
					yc_mean = yc_mean/double(y_num);
					yc_stdev = sqrt(yc_stdev/double(y_num)-yc_mean*yc_mean);
					fprintf(fyc_mean[i], "%lf\n", yc_mean);
					fprintf(fyc_stdev[i], "%lf\n", yc_stdev);
				}
			}
			else{
				fprintf(fyc_mean[i], "%lf\n", y_mean);
				fprintf(fyc_stdev[i], "%lf\n", y_stdev);
			}
			if (p2j[p]>0){
				y_num = 0;
				for (j=0; j<p2a[p].size(); ++j){
					if (a==p2a[p][j])
						continue;
					yj_mean += aj2y_mean[make_pair(p2a[p][j],p2j[p])];
					yj_stdev += aj2y_mean[make_pair(p2a[p][j],p2j[p])]*aj2y_mean[make_pair(p2a[p][j],p2j[p])];
					++y_num;
				}
				if (y_num==0){
					fprintf(fyj_mean[i], "%lf\n", y_mean);
					fprintf(fyj_stdev[i], "%lf\n", y_stdev);
				}
				else{
					yj_mean = yj_mean/double(y_num);
					yj_stdev = sqrt(yj_stdev/double(y_num)-yj_mean*yj_mean);
					fprintf(fyj_mean[i], "%lf\n", yj_mean);
					fprintf(fyj_stdev[i], "%lf\n", yj_stdev);
				}
			}
			else{
				fprintf(fyj_mean[i], "%lf\n", y_mean);
				fprintf(fyj_stdev[i], "%lf\n", y_stdev);
			}
		}
	}
	cerr << "done" << endl;
}

