cd ./code
rm -f *tmp*
echo "Make Astar Basic Feature"
echo ./make_feature $1 ./Train.tmp $2 ./Valid.tmp
./make_feature $1 ./Train.tmp $2 ./Valid.tmp 
echo ./make_feature1 $1 ./Train.tmp1 $2 ./Valid.tmp1 
./make_feature1 $1 ./Train.tmp1 $2 ./Valid.tmp1 
paste -d ' ' Train.tmp Train.tmp1 > Train.tmp2
paste -d ' ' Valid.tmp Valid.tmp1 > Valid.tmp2

cd ../code1
echo "Make dupPidCnt, LastName/Name JaroDis Feature"
rm -f *tmp* 
echo ./trans_feature1 $1 $2 Train.tmp Valid.tmp
./trans_feature1 $1 $2 Train.tmp Valid.tmp
paste -d ' ' ../code/Train.tmp2 Train.tmp > Train.tmp1
paste -d ' ' ../code/Valid.tmp2 Valid.tmp > Valid.tmp1

echo "MAKE dupPaper cnt per aid Feature"
echo ./trans_feature2 $1 $2 Train.tmp Valid.tmp 
./trans_feature2 $1 $2 Train.tmp2 Valid.tmp2
paste -d ' ' Train.tmp1 Train.tmp2 > ../$3.dense
paste -d ' ' Valid.tmp1 Valid.tmp2 > ../$4.dense

cd ../
