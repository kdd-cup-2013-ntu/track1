#!/bin/bash

#gen lists
cd KDDCUP2013/
python gen_list.py ../$1 ../$2 ../$3
cd feature/list
g++ sort_ap.cpp -o sort
./sort
g++ sort_pa.cpp -o sort
./sort
rm -f sort
echo 'gen lists done'

#gen features
cd ../gen
#g++ gen_apy.cpp -o gen_apy
#./gen_apy
g++ gen_year_c.cpp -o gen_year_c
./gen_year_c
cd ../list
g++ fix_nan.cpp -o fix_nan
for (( cnt=0; cnt<=1; cnt=cnt+1 ))
do
	./fix_nan my_stdev_c_$cnt
	rm my_stdev_c_$cnt
	mv my_stdev_c_$cnt.tmp my_stdev_c_$cnt
	./fix_nan myc_stdev_c_$cnt
	rm myc_stdev_c_$cnt
	mv myc_stdev_c_$cnt.tmp myc_stdev_c_$cnt
	./fix_nan myj_stdev_c_$cnt
	rm myj_stdev_c_$cnt
	mv myj_stdev_c_$cnt.tmp myj_stdev_c_$cnt
	./fix_nan c_stdev_c_$cnt
	rm c_stdev_c_$cnt
	mv c_stdev_c_$cnt.tmp c_stdev_c_$cnt
	./fix_nan j_stdev_c_$cnt
	rm j_stdev_c_$cnt
	mv j_stdev_c_$cnt.tmp j_stdev_c_$cnt
done
echo 'gen features done'

cd ../gen
g++ gen_data.cpp -o gen_data
./gen_data y label qid 13 my_c_ yv ry my_mean_c_ my_stdev_c_ myc_mean_c_ myc_stdev_c_ myj_mean_c_ myj_stdev_c_ c_mean_c_ c_stdev_c_ j_mean_c_ j_stdev_c_
cd ../../data
mv y0 ../../$4
mv y1 ../../$5
echo 'gen data done'
