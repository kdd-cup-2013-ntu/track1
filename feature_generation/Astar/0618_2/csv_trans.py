import pandas as pd
import sys
import string

def MakePaperAuthor(path):
	sys.stderr.write('make PaperAuthor.my')
	data = pd.read_csv(path)
	f = open('mydata/PaperAuthor.my','w')
	cnt = 0
	for (a,b) in zip(data['PaperId'],data['AuthorId']):
		if (cnt % 500000 == 0): sys.stderr.write('.')
		f.write("%s %s\n" % (a,b))
		cnt += 1
	f.close()

def MakePaper(path):
	sys.stderr.write('\nmake Paper.my')
	data = pd.read_csv(path)
	f = open('mydata/Paper.my','w')
	cnt = 0
	for (a,b,c) in zip(data['Id'],data['ConferenceId'],data['JournalId']):
		if ( cnt % 500000 == 0): sys.stderr.write('.')
		f.write("%s %s %s\n" % (a,b,c))
		cnt += 1
	f.close()

def MakePaperYear(path):
	sys.stderr.write('\nmake PaperYear.my')
	data = pd.read_csv(path)
	f = open('mydata/PaperYear.my','w')
	cnt = 0
	for (a,b) in zip(data['Id'],data['Year']):
		if ( cnt % 500000 == 0): sys.stderr.write('.')
		f.write("%s %s\n" % (a,b))
		cnt += 1
	f.close()

def ReadPaperAuthor(path):
	sys.stderr.write('\nmake PaperAuthor.my, PaperAuthor.Name under code1/mydata')
	f = open('code1/mydata/PaperAuthor.my','w')	
	f1 = open('code1/mydata/PaperAuthor.Name','w')
	data = pd.read_csv(path)
	L = len(data['PaperId'])
	for i in range(L):
		if (i % 500000 == 0) : sys.stderr.write('.')
		Name = str(data['Name'][i])
		Name = string.replace(Name,'\r','')
		Name = string.replace(Name,'\n','')
		f.write('{0} {1} {2}\n'.format(data['PaperId'][i], data['AuthorId'][i], i+1)) 
		f1.write('{0}\n'.format(Name))

def ReadAuthor(path):
	sys.stderr.write('\nmake Author.my, Author.Name under code1/mydata')
	f = open('code1/mydata/Author.my','w');
	f1 = open('code1/mydata/Author.Name','w');
	data = pd.read_csv(path)
	L = len(data['Id'])
	for i in range(L):
		if (i % 50000 == 0) : sys.stderr.write('.')
		Name = str(data['Name'][i])
		Name = string.replace(Name,'\r','')
		Name = string.replace(Name,'\n','')
		f.write('{0} {1}\n'.format(data['Id'][i], i+1))
		f1.write('{0}\n'.format(Name))

def main():	
	
	path1 = sys.argv[1] + '/PaperAuthor.csv'
	MakePaperAuthor(path1)
	path2 = sys.argv[1] + '/Paper.csv'
	MakePaper(path2)
	MakePaperYear(path2)

	ReadPaperAuthor(path1)
	path3 = sys.argv[1] + '/Author.csv'
	ReadAuthor(path3)

	sys.stderr.write('\nDone!\n')

if __name__ == '__main__':
	if len(sys.argv) < 2:
		sys.stderr.write('./csv_trans.py raw_data_directory\n')
		exit()
	main()
