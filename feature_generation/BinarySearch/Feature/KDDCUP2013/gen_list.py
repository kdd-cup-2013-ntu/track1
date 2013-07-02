import csv
import sys
ftrain = open(sys.argv[2],'r')
fval = open(sys.argv[3],'r')
fpaper = open(sys.argv[1]+'/Paper.csv','r')
fpa = open(sys.argv[1]+'/PaperAuthor.csv','r')
fac_list = open('feature/list/confirmed_list.txt', 'w')
fad_list = open('feature/list/deleted_list.txt', 'w')
facd_list = open('feature/list/cd_list.txt', 'w')
fap_list = open('feature/list/val_list.txt', 'w')
fpaper_list = open('feature/list/paper_list.txt', 'w')
fall_list = open('feature/list/all_list.txt','w')
flabel0 = open('feature/list/label0','w')
flabel1 = open('feature/list/label1','w')
fqid0 = open('feature/list/qid0','w')
fqid1 = open('feature/list/qid1','w')
i = 0
print 'beginning'
for row in csv.DictReader(ftrain,['aid','cid','did']):
	if i==0:
		i = i + 1
		continue
	for pid in row['cid'].split(" "):
		print>>fac_list, row['aid'] + ' ' + pid
		print>>facd_list, row['aid'] + ' ' + pid
		print>>flabel0, '1'
		print>>fqid0, str(i)
	for pid in row['did'].split(" "):
		print>>fad_list, row['aid'] + ' ' + pid
		print>>facd_list, row['aid'] + ' ' + pid
		print>>flabel0, '0'
		print>>fqid0, str(i)
	i = i + 1
ftrain.close()
i = 0
print 'Train.csv done'
for row in csv.DictReader(fval,['aid','pid']):
	if i==0:
		i = i + 1
		continue
	for pid in row['pid'].split(" "):
		print>>fap_list, row['aid'] + ' ' + pid
		print>>flabel1, '1'
		print>>fqid1, str(i)
	i = i + 1
fval.close()
i = 0
print 'Valid.csv done'
for row in csv.DictReader(fpaper,['pid','title','year','cid','jid','key']):
	if i==0:
		i = i + 1
		continue
	print>>fpaper_list, row['pid'] + ' ' + row['year'] + ' ' + row['cid'] + ' ' + row['jid']
fpaper.close()
i = 0
print 'Paper.csv done'
for row in csv.DictReader(fpa, ['pid','aid','name','aff']):
	if i==0:
		i = i + 1
		continue
	print>>fall_list, row['aid'] + ' ' + row['pid']
fpa.close()
print 'PaperAuthor.csv done'
