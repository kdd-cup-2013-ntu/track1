#!/bin/bash

cd KDDCUP2013/feature/gen
rm -f gen_year_c
rm -f gen_data

cd ../list
for (( cnt=0; cnt<=1; cnt=cnt+1))
do
	rm -f *$cnt
done
rm *txt
rm -f fix_nan

cd ../../data
rm -f *

echo 'done'
