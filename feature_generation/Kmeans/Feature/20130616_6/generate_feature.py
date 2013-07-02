#!/usr/bin/python

import csv, sys, os, jellyfish, re, difflib, nltk
from multiprocessing import Pool
from collections import defaultdict

def latin_filter(name):
#	This function convert latin character into ascii character
#	to A 
	name = re.sub(r'\xc3[\x80-\x85]', r'A', name)
	name = re.sub(r'\xc4[\x80\x82\x84]', r'A', name)
	name = re.sub(r'\xc8\xa6', r'A', name)
#	to AE 
	name = re.sub(r'\xc3\x86', r'AE', name)
#	to C 
	name = re.sub(r'\xc3\x87', r'C', name)
	name = re.sub(r'\xc4[\x86\x88\x8a\x8c]', r'C', name)
#	to D 
	name = re.sub(r'\xc3\x90', r'D', name)
	name = re.sub(r'\xc4[\x8e\x90]', r'D', name)
#	to E 
	name = re.sub(r'\xc3[\x88-\x8b]', r'E', name)
	name = re.sub(r'\xc4[\x92\x94\x96\x98\x9a]', r'E', name)
#	to G
	name = re.sub(r'\xc4[\x9c\x9e\xa0\xa2]', r'G', name)
#	to I 
	name = re.sub(r'\xc3[\x8c-\x8f]', r'I', name)
	name = re.sub(r'\xc4[\xa8\xaa\xac\xae\axb0]', r'I', name)
#	to J
	name = re.sub(r'\xc4\xb4', r'J', name)
#	to K
	name = re.sub(r'\xc4\xb6', r'K', name)
#	to L
	name = re.sub(r'\xc4\xbd', r'L', name)
#	to N 
	name = re.sub(r'\xc3\x91|\xc5[\x83\x87]', r'N', name)
#	to O 
	name = re.sub(r'\xc3[\x92-\x96\x98]', r'O', name)
	name = re.sub(r'\xc5[\x8c\x8e\x90]', r'O', name)
	name = re.sub(r'\xc6\x9f', r'O', name)
#	to OE 
	name = re.sub(r'\xc5\x92', r'OE', name)
#	to R 
	name = re.sub(r'\xc5[\x94\x98]', r'R', name)
#	to S
	name = re.sub(r'\xc5[\x9a\x9c\x9e\xa0]', r'S', name)
#	to T 
	name = re.sub(r'\xc5[\xa2\xa4]', r'T', name)
#	to U 
	name = re.sub(r'\xc3[\x99-\x9c]', r'U', name)
	name = re.sub(r'\xc5[\xa8\xaa\xac\xae\xb0\xb2]', r'U', name)
	name = re.sub(r'\xc6\xaf', r'U', name)
#	to Y 
	name = re.sub(r'\xc3\x9d', r'Y', name)
#	to Z 
	name = re.sub(r'\xc5[\xb9\xbb\xbd]', r'Z', name)
#	to Th 
	name = re.sub(r'\xc3\x9e', r'Th', name)
#	to s 
	name = re.sub(r'\xc3\x9f', r's', name)
#	to a 
	name = re.sub(r'\xc3[\xa0-\xa5]', r'a', name)
	name = re.sub(r'\xc4[\x81\x83\x85]', r'a', name)
	name = re.sub(r'\xc7\x8e', r'a', name)
	name = re.sub(r'\xc8[\x83\xa7]', r'n', name)
#	to ae 
	name = re.sub(r'\xc3\xa6', r'ae', name)
#	to c 
	name = re.sub(r'\xc3\xa7', r'c', name)
	name = re.sub(r'\xc4[\x87\x89\x8b\x8d]', r'c', name)
#	to d
	name = re.sub(r'\xc3\xb0', r'd', name)
	name = re.sub(r'\xc4[\x8f\x91]', r'd', name)
#	to e 
	name = re.sub(r'\xc3[\xa8-\xab]', r'e', name)
	name = re.sub(r'\xc4[\x93\x95\x97\x99\x9b]', r'e', name)
	name = re.sub(r'\xc8\xa9', r'e', name)
	name = re.sub(r'\xc9[\x99\x9b]', r'e', name)
#	to g
	name = re.sub(r'\xc4[\x9d\x9f\xa1\xa3]', r'g', name)
	name = re.sub(r'\xc7\xa7', r'g', name)
#	to h
	name = re.sub(r'\xc4[\xa5\xa7]', r'h', name)
#	to i 
	name = re.sub(r'\xc3[\xac-\xaf]', r'i', name)
	name = re.sub(r'\xc4[\xa9\xab\xad\xaf\xb0\xb1]', r'i', name)
	name = re.sub(r'\xc7\x90', r'i', name)
#	to ij
	name = re.sub(r'\xc4\xb3', r'ij', name)
#	to j
	name = re.sub(r'\xc4\xb5', r'j', name)
#	to k
	name = re.sub(r'\xc4\xb7', r'k', name)
#	to l
	name = re.sub(r'\xc4[\xba\xbe]|\xc5\x80', r'l', name)
#	to n 
	name = re.sub(r'\xc3\xb1|\xc5[\x84\x86\x88]', r'n', name)
	name = re.sub(r'\xc7\xb9', r'n', name)
#	to eng 
	name = re.sub(r'\xc5\x8b', r'eng', name)
#	to oe
	name = re.sub(r'\xc5\x93', r'oe', name)
#	to o 
	name = re.sub(r'\xc3[\xb2-\xb6\xb8]', r'o', name)
	name = re.sub(r'\xc5[\x8d\x8f\x91]', r'o', name)
	name = re.sub(r'\xc7\xab', r'o', name)
#	to r
	name = re.sub(r'\xc5[\x95\x99]', r'r', name)
	name = re.sub(r'\xc9\xb9', r'r', name)
#	to t
	name = re.sub(r'\xc5[\xa3\xa5]', r't', name)
#	to s
	name = re.sub(r'\xc5[\x9b\x9d\x9f\xa1]', r's', name)
#	to u 
	name = re.sub(r'\xc3[\xb9-\xbc]', r'u', name)
	name = re.sub(r'\xc5[\xa9\xab\xad\xaf\xb1\xb3]', r'u', name)
#	to y 
	name = re.sub(r'\xc3[\xbd\xbf]', r'y', name)
#	to z 
	name = re.sub(r'\xc5[\xba\xbc\xbe]', r'z', name)
#	to th
	name = re.sub(r'\xc3\xbe', r'th', name)

#	to L
	name = re.sub(r'\xc5\x81', r'L', name)
#	to l
	name = re.sub(r'\xc5\x82', r'l', name)
#	haven't done '\xce\xbe', '\xce\xbf', '\xcf\x80', '\xcf\x9a'
	name = re.sub(r'[\xc0-\xdf].', '', name)
	name = re.sub(r'[\xe0-\xef]..', '', name)
	name = re.sub(r'[\xf0-\xf7]...', '', name)

	return name

def parse_id(string):
	return string.rstrip().split()

def readinput(input_path):
	table = list()
	with open(input_path, "r") as f:
		reader = csv.reader(f)
		for row in reader:
			table.append(row)
	attr = table.pop(0)
	return attr, table

def get_invalid_word_set(input_path):
#	prepare cleaned version of training data which removes character in non-ascii encoding
	targetfiles = ["Author.csv", "Paper.csv", "PaperAuthor.csv"]
	if not os.path.isdir("Cleaned"):
		os.mkdir("Cleaned")
	for target in targetfiles:
		print target
		with open(os.path.join(input_path,target)) as fin, open(os.path.join("Cleaned",target), 'w') as fout:
			i = 0
			print '%9d' % (i),
			for line in fin:
				line = latin_filter(line)
				fout.write(line)
				i += 1
				if i % 10000 == 0:
					print '\b'*10 + '%9d' %(i),

def jacard(set1, set2):
#	compute jacard distance
	union = len(set1 | set2)
	intersect = len(set1 & set2)
	return float(intersect)/float(union)

class DataCollection:
	def __init__(self):
		self.papersauthors = dict()	# data from PaperAuthor.csv that need preprocess
		self.authorspapers = dict()
		self.authorconference = dict()
		self.authorjournal = dict()

		self.paperconference = dict()	# data from Paper.csv and Author.csv
		self.paperjournal = dict()
		self.paperyear = dict()
		self.paperkeywords = dict()
		self.papertitle = dict()

		self.authorname = dict()	# data from Author.csv
		self.authorsplittedname = dict()
		self.authorabbrevname = dict()
		self.authoraffiliation = dict()

		self.paperauthornamelist = dict()	# author data from PaperAuthor.csv
		self.paperauthorsplittednamelist = dict()
		self.paperauthorabbrevnamelist = dict()
		self.paperauthoraffiliationlist = dict()
		self.paperauthorlist = dict()
		self.name = dict()

		self.authoraffiliationlist = dict()

		self.avgaffiliationlength = 0.0
		self.avgauthornamelength = 0.0
		self.avgtitlelength = 0.0

	def readPaperAuthor(self,inputdir):
		inputpath = os.path.join(inputdir,"PaperAuthor.csv")
		#stoplist = nltk.corpus.stopwords.words('english')
		stoplist = ['i', 'me', 'my', 'myself', 'we', 'our', 'ours', 'ourselves', 'you', 'your', 'yours', 'yourself', 'yourselves', 'he', 'him', 'his', 'himself', 'she', 'her', 'hers', 'herself', 'it', 'its', 'itself', 'they', 'them', 'their', 'theirs', 'themselves', 'what', 'which', 'who', 'whom', 'this', 'that', 'these', 'those', 'am', 'is', 'are', 'was', 'were', 'be', 'been', 'being', 'have', 'has', 'had', 'having', 'do', 'does', 'did', 'doing', 'a', 'an', 'the', 'and', 'but', 'if', 'or', 'because', 'as', 'until', 'while', 'of', 'at', 'by', 'for', 'with', 'about', 'against', 'between', 'into', 'through', 'during', 'before', 'after', 'above', 'below', 'to', 'from', 'up', 'down', 'in', 'out', 'on', 'off', 'over', 'under', 'again', 'further', 'then', 'once', 'here', 'there', 'when', 'where', 'why', 'how', 'all', 'any', 'both', 'each', 'few', 'more', 'most', 'other', 'some', 'such', 'no', 'nor', 'not', 'only', 'own', 'same', 'so', 'than', 'too', 'very', 's', 't', 'can', 'will', 'just', 'don', 'should', 'now']
		distinctaffiliation = 0
		distinctauthorname = 0
		with open(inputpath, 'rb') as csvfile:
			reader = csv.reader(csvfile)
			reader.next() # remove first line
		        for row in reader:
				PaperId = row[0]
				AuthorId = row[1]
				Name = row[2]
				Affiliation = row[3]

				if not self.papersauthors.has_key(PaperId):
					self.papersauthors[PaperId] = set()
				self.papersauthors[PaperId].add(AuthorId)
				if not self.authorspapers.has_key(AuthorId):
					self.authorspapers[AuthorId] = set()
				self.authorspapers[AuthorId].add(PaperId)

				if len(Affiliation) > 0:
					if Affiliation[0] == '"' and Affiliation[-1] == '"':
						Affiliation = Affiliation[1:-1]
					Affiliation = " ".join( [w for w in Affiliation.replace(",",' ').replace("|",' ').replace(";",' ').replace("(",' ').replace(")",' ').replace("<SUP>","").replace("</SUP>","").split() if not w in stoplist])
				if not self.authoraffiliationlist.has_key(AuthorId):
					self.authoraffiliationlist[AuthorId] = set()
				if self.paperyear.has_key(PaperId):
					year = self.paperyear[PaperId]
				else:
					year = -1
				self.authoraffiliationlist[AuthorId].add( (Affiliation.lower(),year) )
				if not self.paperauthoraffiliationlist.has_key(PaperId):
					self.paperauthoraffiliationlist[PaperId] = dict()
				if not self.paperauthoraffiliationlist[PaperId].has_key(AuthorId):
					self.paperauthoraffiliationlist[PaperId][AuthorId] = list()
				self.paperauthoraffiliationlist[PaperId][AuthorId].append( Affiliation.lower() )

				if len(Affiliation) > 0:
					self.avgaffiliationlength += float(len(Affiliation))
					distinctaffiliation += 1

				if not self.paperauthornamelist.has_key(PaperId):
					self.paperauthornamelist[PaperId] = dict()
					self.paperauthorabbrevnamelist[PaperId] = dict()
					self.paperauthorsplittednamelist[PaperId] = dict()
				if not self.paperauthornamelist[PaperId].has_key(AuthorId):
					self.paperauthornamelist[PaperId][AuthorId] = list()
					self.paperauthorabbrevnamelist[PaperId][AuthorId] = list()
					self.paperauthorsplittednamelist[PaperId][AuthorId] = list()
				name = Name.lower()
				self.paperauthornamelist[PaperId][AuthorId].append( Name.lower() )

				if not self.paperauthorlist.has_key(PaperId):
					self.paperauthorlist[PaperId] = dict()
				if not self.paperauthorlist[PaperId].has_key(AuthorId):
					self.paperauthorlist[PaperId][AuthorId] = list()
				self.paperauthorlist[PaperId][AuthorId].append((Name.lower(),Affiliation.lower()))
				
				cleaned_name = re.sub(r'\W',' ',Name).lower().split()
				if len(cleaned_name) > 0:
					abbrevname = [ letter[0] for letter in cleaned_name[:-1]]
					abbrevname.append(cleaned_name[-1])
				else:
					abbrevname = ['']
				self.paperauthorabbrevnamelist[PaperId][AuthorId].append( " ".join(abbrevname) )

				if len(cleaned_name) > 0:
					entry = dict()
					entry["lastname"] = cleaned_name[-1]
					entry["firstname"] = cleaned_name[0]
					if len(cleaned_name) > 2:
						entry["middlename"] = " ".join(cleaned_name[1:-1])
					self.paperauthorsplittednamelist[PaperId][AuthorId].append(entry)

				if not self.name.has_key(name):
					self.name[name] = set()
				self.name[name].add(AuthorId)
				if len(name) > 0:
					self.avgauthornamelength += float(len(name))
					distinctauthorname += 1
		self.avgaffiliationlength /= float(distinctaffiliation)
		self.avgauthornamelength /= float(distinctauthorname)

		for author in self.authorspapers.keys():
			self.authorconference[author] = dict()
			self.authorjournal[author] = dict()
			for paper in self.authorspapers[author]:
				if self.paperconference.has_key(paper):
					conferenceId = self.paperconference[paper]
					if not self.authorconference[author].has_key(conferenceId):
						self.authorconference[author][conferenceId] = 0
					self.authorconference[author][conferenceId] += 1
				elif self.paperjournal.has_key(paper):
					journalId = self.paperjournal[paper]
					if not self.authorjournal[author].has_key(journalId):
						self.authorjournal[author][journalId] = 0
					self.authorjournal[author][journalId] += 1

	def readAuthor(self,inputdir):
		inputpath = os.path.join(inputdir,"Author.csv")
		#stoplist = nltk.corpus.stopwords.words('english')
		stoplist = ['i', 'me', 'my', 'myself', 'we', 'our', 'ours', 'ourselves', 'you', 'your', 'yours', 'yourself', 'yourselves', 'he', 'him', 'his', 'himself', 'she', 'her', 'hers', 'herself', 'it', 'its', 'itself', 'they', 'them', 'their', 'theirs', 'themselves', 'what', 'which', 'who', 'whom', 'this', 'that', 'these', 'those', 'am', 'is', 'are', 'was', 'were', 'be', 'been', 'being', 'have', 'has', 'had', 'having', 'do', 'does', 'did', 'doing', 'a', 'an', 'the', 'and', 'but', 'if', 'or', 'because', 'as', 'until', 'while', 'of', 'at', 'by', 'for', 'with', 'about', 'against', 'between', 'into', 'through', 'during', 'before', 'after', 'above', 'below', 'to', 'from', 'up', 'down', 'in', 'out', 'on', 'off', 'over', 'under', 'again', 'further', 'then', 'once', 'here', 'there', 'when', 'where', 'why', 'how', 'all', 'any', 'both', 'each', 'few', 'more', 'most', 'other', 'some', 'such', 'no', 'nor', 'not', 'only', 'own', 'same', 'so', 'than', 'too', 'very', 's', 't', 'can', 'will', 'just', 'don', 'should', 'now']
		with open(inputpath, 'rb') as csvfile:
			reader = csv.reader(csvfile)
			reader.next()
			for row in reader:
				AuthorId = row[0]
				Name = row[1]
				Affiliation = row[2]	# information in Author.csv: name, affiliation

				if len(Affiliation) > 0:
					if Affiliation[0] == '"' and Affiliation[-1] == '"':
						Affiliation = Affiliation[1:-1]
					Affiliation = " ".join( [w for w in Affiliation.replace(",",' ').replace("|",' ').replace(";",' ').replace("(",' ').replace(")",' ').replace("<SUP>","").replace("</SUP>","").split() if not w in stoplist])
				self.authorname[AuthorId] = Name.lower()
				self.authoraffiliation[AuthorId] = Affiliation.lower()

				cleaned_name = re.sub(r'\W',' ',Name).lower().split()
				if len(cleaned_name) > 0:
					abbrevname = [ letter[0] for letter in cleaned_name[:-1]]
					abbrevname.append(cleaned_name[-1])
				else:
					abbrevname = ['']
				self.authorabbrevname[AuthorId] = " ".join(abbrevname)

				if len(cleaned_name) > 0:
					self.authorsplittedname[AuthorId] = dict()
					self.authorsplittedname[AuthorId]["lastname"] = cleaned_name[-1]
					self.authorsplittedname[AuthorId]["firstname"] = cleaned_name[0]
					if len(cleaned_name) > 2:
						self.authorsplittedname[AuthorId]["middlename"] = " ".join(cleaned_name[1:-1])

	def readPaper(self,inputdir):
		inputpath = os.path.join(inputdir,"Paper.csv")
		#stoplist = nltk.corpus.stopwords.words('english')
		stoplist = ['i', 'me', 'my', 'myself', 'we', 'our', 'ours', 'ourselves', 'you', 'your', 'yours', 'yourself', 'yourselves', 'he', 'him', 'his', 'himself', 'she', 'her', 'hers', 'herself', 'it', 'its', 'itself', 'they', 'them', 'their', 'theirs', 'themselves', 'what', 'which', 'who', 'whom', 'this', 'that', 'these', 'those', 'am', 'is', 'are', 'was', 'were', 'be', 'been', 'being', 'have', 'has', 'had', 'having', 'do', 'does', 'did', 'doing', 'a', 'an', 'the', 'and', 'but', 'if', 'or', 'because', 'as', 'until', 'while', 'of', 'at', 'by', 'for', 'with', 'about', 'against', 'between', 'into', 'through', 'during', 'before', 'after', 'above', 'below', 'to', 'from', 'up', 'down', 'in', 'out', 'on', 'off', 'over', 'under', 'again', 'further', 'then', 'once', 'here', 'there', 'when', 'where', 'why', 'how', 'all', 'any', 'both', 'each', 'few', 'more', 'most', 'other', 'some', 'such', 'no', 'nor', 'not', 'only', 'own', 'same', 'so', 'than', 'too', 'very', 's', 't', 'can', 'will', 'just', 'don', 'should', 'now']
		with open(inputpath, 'rb') as csvfile:
			reader = csv.reader(csvfile)
			reader.next()
			distincttitlecount = 0
			for row in reader:
				PaperId = row[0]
				Title = row[1]
				Year = int(row[2])
				ConferenceId = int(row[3])
				JournalId = int(row[4])
				Keywords = row[5]
				if Year > 1900 and Year < 2013:
					self.paperyear[PaperId] = Year
				if ConferenceId > 0:
					self.paperconference[PaperId] = ConferenceId
				elif JournalId > 0:
					self.paperconference[PaperId] = JournalId
				keywords = [w for w in Keywords.lower().replace('"',' ').replace("|",' ').replace(",",' ').split() if not w in stoplist]
				if len(Title) > 0:
					if Title[0] == '"' and Title[-1] == '"':
						Title = Title[1:-1]
					Title = " ".join([w for w in Title.split() if not w in stoplist])
				self.paperkeywords[PaperId] = set(keywords)
				self.papertitle[PaperId] = Title.lower()
				if len(Title) > 0:
					distincttitlecount += 1
					self.avgtitlelength += float(len(Title))
			self.avgtitlelength /= float(distincttitlecount)

	def writeentry(self,paper,author,Papers,fout):
		if self.paperjournal.has_key(paper): # Number of publication for the author in the journal
			if self.authorjournal.has_key(author) and self.authorjournal[author].has_key(self.paperjournal[paper]):
				fout.write("1:%d "%( self.authorjournal[author][self.paperjournal[paper]] ) )
			journalcount = 0
			for journal in self.authorjournal[author].keys():
				journalcount += self.authorjournal[author][journal]
			fout.write("2:%d "%(journalcount))
		if self.authorspapers.has_key(author): # Number of publication of the author
			fout.write("3:%d "%( len(self.authorspapers[author])))

		if self.paperauthornamelist.has_key(paper) and self.paperauthornamelist[paper].has_key(author) and self.authorname.has_key(author):
			jarodistance = list()
			levenshteindistance = list()
			ratio = list()
			for authorname in self.paperauthornamelist[paper][author]:
				if len(authorname) == 0 and len(self.authorname[author]) == 0:
					ratio.append(0.5)
				else:
					seq = difflib.SequenceMatcher(None,authorname,self.authorname[author])
					ratio.append(seq.ratio())
			fout.write("4:%f "%( min(ratio)) )
		if self.paperauthoraffiliationlist.has_key(paper) and self.paperauthoraffiliationlist[paper].has_key(author) and self.authoraffiliation.has_key(author):
			levenshteindistance = list()
			ratio = list()
			for authoraffiliation in self.paperauthoraffiliationlist[paper][author]:
				if len(authoraffiliation) == 0 and len(self.authoraffiliation[author]) == 0:
					ratio.append(0.5)
					levenshteindistance.append(self.avgaffiliationlength)
				else:
					seq = difflib.SequenceMatcher(None,authoraffiliation,self.authoraffiliation[author])
					ratio.append(seq.ratio())
					levenshteindistance.append( jellyfish.levenshtein_distance(authoraffiliation,self.authoraffiliation[author]) )
			fout.write("5:%d "%( max(levenshteindistance)) )
			fout.write("6:%f "%( min(ratio)) )
		if self.papersauthors.has_key(paper):
			paperswithcoauthor = 0
			coauthors = set()
			for otherpaper in Papers:
				if otherpaper == paper:
					continue
				if self.papersauthors.has_key(otherpaper):
					coauthors |= self.papersauthors[otherpaper]
					if len(self.papersauthors[paper] & self.papersauthors[otherpaper]) > 1:
						paperswithcoauthor += 1
			coauthors.discard(author)
			fout.write("7:%f "%(float(paperswithcoauthor)/float(len(Papers))))
			fout.write("8:%d "%( len(coauthors) ))
			overlap = list()
			intersects = list()
			for coauthor in self.papersauthors[paper]:
				if coauthor == author:
					continue
				union = len(self.authorspapers[author] | self.authorspapers[coauthor])
				intersect = len(self.authorspapers[author] & self.authorspapers[coauthor])
				intersects.append(intersect)
				overlap.append( float(intersect)/float(union) )
			if len(overlap) > 0:
				fout.write("9:%f "%(max(overlap)))
				fout.write("10:%d "%(max(intersects)))
		if self.paperyear.has_key(paper) and len(Papers)>1:
			farestpaperyear = 0
			for otherpaper in Papers:
				if otherpaper == paper:
					continue
				if self.paperyear.has_key(otherpaper):
					if abs(self.paperyear[paper]-self.paperyear[otherpaper]) > farestpaperyear:
						farestpaperyear = self.paperyear[paper] - self.paperyear[otherpaper]
			fout.write("12:%f "%(float(farestpaperyear)/113.0))
		if self.papertitle.has_key(paper) and len(self.papertitle[paper]) > 0 and len(Papers) > 1:
			jarodistance = list()
			levenshteindistance = list()
			ratio = list()
			for otherpaper in Papers:
				if otherpaper == paper:
					continue
				if self.papertitle.has_key(otherpaper):
					if len(self.papertitle[paper]) == 0 and len(self.papertitle[otherpaper]) == 0:
						jarodistance.append(0.5)
						levenshteindistance.append(self.avgtitlelength/2)
					else:
						jarodistance.append( jellyfish.jaro_distance(self.papertitle[paper],self.papertitle[otherpaper]) )
						levenshteindistance.append( jellyfish.levenshtein_distance(self.papertitle[paper],self.papertitle[otherpaper]) )
			if len(jarodistance) > 0:
				fout.write("13:%f "%(max(jarodistance)))
				fout.write("14:%f "%(min(levenshteindistance)))
		if self.paperkeywords.has_key(paper) and len(Papers) > 1:
			intersects = list()
			for otherpaper in Papers:
				if otherpaper == paper:
					continue
				if self.paperkeywords.has_key(otherpaper):
					intersect = len(self.paperkeywords[paper] & self.paperkeywords[otherpaper])
					union = len(self.paperkeywords[paper] | self.paperkeywords[otherpaper])
					if union > 0:
						intersects.append( float(intersect)/float(union) )
			if len(intersects) > 0:
				fout.write("15:%f "%(max(intersects)))
		if self.papersauthors.has_key(paper):
			fout.write("16:%d "%(len(self.papersauthors[paper])))
			affiliationjarodistance = list()
			affiliationlevenshteindistance = list()
			if self.authoraffiliation.has_key(author) and len(self.papersauthors[paper])>1:
				for otherauthor in self.papersauthors[paper]:
					if author == otherauthor:
						continue
					if self.authoraffiliation.has_key(otherauthor):
						if len(self.authoraffiliation[author]) == 0 and len(self.authoraffiliation[otherauthor]) == 0:
							affiliationjarodistance.append(0.5)
							affiliationlevenshteindistance.append(self.avgaffiliationlength/2)
						else:
							affiliationjarodistance.append(jellyfish.jaro_distance(self.authoraffiliation[author],self.authoraffiliation[otherauthor]))
							affiliationlevenshteindistance.append(jellyfish.levenshtein_distance(self.authoraffiliation[author],self.authoraffiliation[otherauthor]))
				if len(affiliationjarodistance) > 0:
					fout.write("17:%f "%(max(affiliationjarodistance)))
					fout.write("18:%d "%(max(affiliationlevenshteindistance)))
		if self.paperauthornamelist.has_key(paper):
			jarodistance = list()
			levenshteindistance = list()
			samenameauthor = list()
			for name in self.paperauthornamelist[paper][author]:
				samenameauthor.append(len(self.name[name]))
			if self.authorname.has_key(author):
				samenameauthor.append(len(self.name[self.authorname[author]]))
			for otherauthor in self.papersauthors[paper]:
				if otherauthor == author:
					continue
				if self.authoraffiliation.has_key(otherauthor) and len(self.authoraffiliation[author]) > 0 and self.authoraffiliation[author] == self.authoraffiliation[otherauthor]:
					continue
				for name in self.paperauthornamelist[paper][author]:
					for otherauthor_name in self.paperauthornamelist[paper][otherauthor]:
						if len(name) == 0 and len(otherauthor_name) == 0:
							jarodistance.append(0.5)
							levenshteindistance.append(self.avgauthornamelength/2)
						else:
							jarodistance.append( jellyfish.jaro_distance(name,otherauthor_name))
							levenshteindistance.append( jellyfish.levenshtein_distance(name,otherauthor_name))
			if len(jarodistance) > 0:
				fout.write("19:%f "%( max(jarodistance)) )
				fout.write("20:%d "%( min(levenshteindistance)) )
			if len(samenameauthor) > 0:
				fout.write("21:%d "%(max(samenameauthor)))
		if self.paperauthoraffiliationlist.has_key(paper):
			jarodistance = list()
			levenshteindistance = list()
			for otherauthor in self.papersauthors[paper]:
				if otherauthor == author:
					continue
				for otherauthor_affiliation in self.paperauthoraffiliationlist[paper][otherauthor]:
					for affiliation in self.paperauthoraffiliationlist[paper][author]:
						if len(affiliation) == 0 and  len(otherauthor_affiliation) == 0:
							jarodistance.append(0.5)
							levenshteindistance.append(self.avgaffiliationlength/2)
						else:
							jarodistance.append( jellyfish.jaro_distance(affiliation,otherauthor_affiliation))
							levenshteindistance.append( jellyfish.levenshtein_distance(affiliation,otherauthor_affiliation))
			if len(jarodistance) > 0:
				fout.write("22:%f "%( max(jarodistance)) )
				fout.write("23:%d "%( min(levenshteindistance)) )
		if self.paperconference.has_key(paper): # Number of publication for the author in the conference
			if self.authorconference.has_key(author) and self.authorconference[author].has_key(self.paperconference[paper]):
				fout.write("24:%d "%( self.authorconference[author][self.paperconference[paper]] ) )
			conferencecount = 0
			for conference in self.authorconference[author].keys():
				conferencecount += self.authorconference[author][conference]
			fout.write("25:%d "%(conferencecount))

		if self.paperauthorabbrevnamelist.has_key(paper) and self.paperauthorabbrevnamelist[paper].has_key(author) and self.authorabbrevname.has_key(author):
			ratio = list()
			for authorname in self.paperauthorabbrevnamelist[paper][author]:
				seq = difflib.SequenceMatcher(None,authorname,self.authorabbrevname[author])
				ratio.append(seq.ratio())
			fout.write("26:%f "%( min(ratio)) )
		if self.paperauthorabbrevnamelist.has_key(paper):
			jarodistance = list()
			levenshteindistance = list()
			for otherauthor in self.papersauthors[paper]:
				if otherauthor == author:
					continue
				if self.authoraffiliation.has_key(otherauthor) and len(self.authoraffiliation[author]) > 0 and self.authoraffiliation[author] == self.authoraffiliation[otherauthor]:
					continue
				for name in self.paperauthorabbrevnamelist[paper][author]:
					for otherauthor_name in self.paperauthorabbrevnamelist[paper][otherauthor]:
						if len(name) == 0 and len(otherauthor_name) == 0:
							jarodistance.append(0.5)
							levenshteindistance.append(self.avgauthornamelength/4)
						else:
							jarodistance.append( jellyfish.jaro_distance(name,otherauthor_name))
							levenshteindistance.append( jellyfish.levenshtein_distance(name,otherauthor_name))
			if len(jarodistance) > 0:
				fout.write("27:%f "%( max(jarodistance)) )
				fout.write("28:%d "%( min(levenshteindistance)) )
		if self.papersauthors.has_key(paper):
			affiliationjarodistance = list()
			affiliationlevenshteindistance = list()
			affiliationjacarddistance = list()
			sameaffiliation_authors = 0
			if self.authoraffiliationlist.has_key(author) and len(self.papersauthors[paper])>1:
				for otherauthor in self.papersauthors[paper]:
					if author == otherauthor:
						continue
					if self.authoraffiliationlist.has_key(otherauthor):
						sameaffiliation = False
						for affiliation1 in self.authoraffiliationlist[author]:
							for affiliation2 in self.authoraffiliationlist[otherauthor]:
								if affiliation1 == affiliation2:
									sameaffiliation = True
								if len(affiliation1[0]) == 0 and len(affiliation2[0]) == 0:
									affiliationjarodistance.append(0.5)
									affiliationlevenshteindistance.append(self.avgaffiliationlength/2)
								else:
									affiliationjarodistance.append(jellyfish.jaro_distance(affiliation1[0],affiliation2[0]))
									affiliationlevenshteindistance.append(jellyfish.levenshtein_distance(affiliation1[0],affiliation2[0]))
								if len(affiliation1[0]) > 0 or len(affiliation2[0])> 0:
									affiliationjacarddistance.append( jacard(set(affiliation1[0].split()),set(affiliation2[0].split())) )
						if sameaffiliation:
							sameaffiliation_authors += 1
				if len(affiliationjarodistance) > 0:
					fout.write("29:%f "%(max(affiliationjarodistance)))
					fout.write("30:%d "%(max(affiliationlevenshteindistance)))
				if len(affiliationjacarddistance)>0:
					fout.write("31:%f "%(max(affiliationjacarddistance)))
				fout.write("32:%d "%(sameaffiliation_authors))

		if self.paperauthorsplittednamelist.has_key(paper) and self.paperauthorsplittednamelist[paper].has_key(author) and self.authorsplittedname.has_key(author):
			ratiolastname = list()
			ratiofirstname = list()
			ratiomiddlename = list()
			ratioreversename = list()
			for authorname in self.paperauthorsplittednamelist[paper][author]:
				seq = difflib.SequenceMatcher(None,authorname['lastname'],self.authorsplittedname[author]['lastname'])
				ratiolastname.append(seq.ratio())
				seq = difflib.SequenceMatcher(None,authorname['firstname'],self.authorsplittedname[author]['firstname'])
				ratiofirstname.append(seq.ratio())
				if authorname.has_key('middlename') and self.authorsplittedname[author].has_key('middlename'):
					seq = difflib.SequenceMatcher(None,authorname['middlename'],self.authorsplittedname[author]['middlename'])
					ratiomiddlename.append(seq.ratio())
				seq1 = difflib.SequenceMatcher(None,authorname['lastname'],self.authorsplittedname[author]['firstname'])
				seq2 = difflib.SequenceMatcher(None,authorname['firstname'],self.authorsplittedname[author]['lastname'])
				ratioreversename.append(seq1.ratio()+seq2.ratio())
			if len(ratiolastname) > 0:
				fout.write("33:%f "%( min(ratiolastname)) )
				fout.write("34:%f "%( min(ratiofirstname)) )
				fout.write("35:%f "%( min(ratioreversename)) )
			if len(ratiomiddlename) > 0:
				fout.write("36:%f "%( min(ratiomiddlename)) )

		if self.paperauthorsplittednamelist.has_key(paper):
			lastnamejarodistance = list()
			firstnamejarodistance = list()
			reversenamejarodistance = list()
			reverseabbreviatenamejarodistance_wrong = list()
			reverseabbreviatenamejarodistance_correct = list()

			lastnamelevenshteindistance = list()
			firstnamelevenshteindistance = list()
			reversenamelevenshteindistance = list()
			reverseabbreviatenamelevenshteindistance_wrong = list()
			reverseabbreviatenamelevenshteindistance_correct = list()
			for otherauthor in self.papersauthors[paper]:
				if otherauthor == author:
					continue
				if self.authoraffiliation.has_key(otherauthor) and len(self.authoraffiliation[author]) > 0 and self.authoraffiliation[author] == self.authoraffiliation[otherauthor]:
					continue
				if self.paperauthorsplittednamelist[paper].has_key(author) and self.paperauthorsplittednamelist[paper].has_key(otherauthor):
					for name in self.paperauthorsplittednamelist[paper][author]:
						for otherauthor_name in self.paperauthorsplittednamelist[paper][otherauthor]:
							if len(name) == 0 and len(otherauthor_name) == 0:
								continue
							lastnamejarodistance.append( jellyfish.jaro_distance(name['lastname'],otherauthor_name['lastname']))
							lastnamelevenshteindistance.append( jellyfish.levenshtein_distance(name['lastname'],otherauthor_name['lastname']))

							firstnamejarodistance.append( jellyfish.jaro_distance(name['firstname'],otherauthor_name['firstname']))
							firstnamelevenshteindistance.append( jellyfish.levenshtein_distance(name['firstname'],otherauthor_name['firstname']))

							reversenamejarodistance.append( jellyfish.jaro_distance(name['lastname'],otherauthor_name['firstname']) + jellyfish.jaro_distance(name['firstname'],otherauthor_name['lastname']) )
							reversenamelevenshteindistance.append( jellyfish.levenshtein_distance(name['lastname'],otherauthor_name['firstname']) + jellyfish.levenshtein_distance(name['firstname'],otherauthor_name['lastname']) )

							reverseabbreviatenamejarodistance_wrong.append( jellyfish.jaro_distance(name['lastname'][0],otherauthor_name['firstname'][0]) + jellyfish.jaro_distance(name['firstname'],otherauthor_name['lastname']) )
							reverseabbreviatenamejarodistance_correct.append( jellyfish.jaro_distance(name['lastname'],otherauthor_name['firstname']) + jellyfish.jaro_distance(name['firstname'][0],otherauthor_name['lastname'][0]) )

							reverseabbreviatenamelevenshteindistance_wrong.append( jellyfish.levenshtein_distance(name['lastname'][0],otherauthor_name['firstname'][0]) + jellyfish.levenshtein_distance(name['firstname'],otherauthor_name['lastname']) )
							reverseabbreviatenamelevenshteindistance_correct.append( jellyfish.levenshtein_distance(name['lastname'],otherauthor_name['firstname']) + jellyfish.levenshtein_distance(name['firstname'][0],otherauthor_name['lastname'][0]) )

			if len(lastnamejarodistance) > 0:
				fout.write("37:%f "%( max(lastnamejarodistance)) )
				fout.write("38:%f "%( max(firstnamejarodistance)) )
				fout.write("39:%f "%( max(reversenamejarodistance)) )

				fout.write("40:%f "%( max(reverseabbreviatenamejarodistance_wrong)) )
				fout.write("41:%f "%( max(reverseabbreviatenamejarodistance_correct)) )

				fout.write("42:%d "%( min(lastnamelevenshteindistance)) )
				fout.write("43:%d "%( min(firstnamelevenshteindistance)) )
				fout.write("44:%d "%( min(reversenamelevenshteindistance)) )

				fout.write("45:%d "%( min(reverseabbreviatenamelevenshteindistance_wrong)) )
				fout.write("46:%d "%( min(reverseabbreviatenamelevenshteindistance_correct)) )
		if self.paperauthornamelist.has_key(paper) and self.paperauthornamelist[paper].has_key(author):
			fout.write("47:%d "%(len(self.paperauthornamelist[paper][author])))
		if self.paperconference.has_key(paper): # Number of publication for the author in the conference
			conferencecount = 0
			for conference in self.authorconference[author].keys():
				conferencecount += self.authorconference[author][conference]
			fout.write("48:%f "%(float(conferencecount)/float(len(self.authorspapers[author]))))
		if self.paperjournal.has_key(paper): # Number of publication for the author in the journal
			journalcount = 0
			for journal in self.authorjournal[author].keys():
				journalcount += self.authorjournal[author][journal]
			fout.write("49:%f "%(float(journalcount)/float(len(self.authorspapers[author]))))
		if self.paperauthorlist.has_key(paper) and self.paperauthorlist[paper].has_key(author):
			distance1 = list()
			distance2 = list()
			for otherauthor in self.papersauthors[paper]:
				if otherauthor == author:
					continue
				if self.paperauthorlist[paper].has_key(otherauthor):
					for (name1,affiliation1) in self.paperauthorlist[paper][author]:
						for (name2,affiliation2) in self.paperauthorlist[paper][otherauthor]:
							if len(name1) == 0 and len(name2) == 0:
								jaroname = 0.5
								levenshteinname = self.avgauthornamelength/2.0
							else:
								jaroname = jellyfish.jaro_distance(name1,name2)
								levenshteinname = jellyfish.levenshtein_distance(name1,name2)
							if len(affiliation1) == 0 and len(affiliation2) == 0:
								levenshteinaffiliation = self.avgaffiliationlength/2.0
								jaroaffiliation = 0.5
							else:
								levenshteinaffiliation = jellyfish.levenshtein_distance(affiliation1,affiliation2)
								jaroaffiliation = jellyfish.jaro_distance(affiliation1,affiliation2)
							distance1.append( jaroname*levenshteinaffiliation )
							distance2.append( jaroaffiliation*levenshteinname )
			if len(distance1) > 0:
				fout.write("50:%f "%(max(distance2)))
		if self.paperauthornamelist.has_key(paper) and self.authorname.has_key(author):
			jarodistance = list()
			levenshteindistance = list()
			name = self.authorname[author]
			for otherauthor in self.papersauthors[paper]:
				if otherauthor == author:
					continue
				for otherauthor_name in self.paperauthornamelist[paper][otherauthor]:
					if len(name) == 0 and len(otherauthor_name) == 0:
						jarodistance.append(0.5)
						levenshteindistance.append(self.avgauthornamelength/2)
					else:
						jarodistance.append( jellyfish.jaro_distance(name,otherauthor_name))
						levenshteindistance.append( jellyfish.levenshtein_distance(name,otherauthor_name))
			if len(jarodistance) > 0:
				fout.write("51:%f "%( max(jarodistance)) )
				fout.write("52:%d "%( min(levenshteindistance)) )
		if self.paperauthornamelist.has_key(paper):
			duplicateAID = 0
			for otherauthor in self.papersauthors[paper]:
				if self.paperauthornamelist[paper].has_key(otherauthor) and  len(self.paperauthornamelist[paper][otherauthor]) > 1:
					duplicateAID += 1
			fout.write("53:%d "%(duplicateAID))
		if len(self.papersauthors[paper]) == 1:
			fout.write("54:1 ")
		if self.paperauthoraffiliationlist.has_key(paper):
			emptyaffiliation = 0
			count = 0
			for otherauthor in self.papersauthors[paper]:
				if otherauthor == author:
					count += 1
					continue
				for otherauthor_affiliation in self.paperauthoraffiliationlist[paper][otherauthor]:
					if len(otherauthor_affiliation) == 0:
						emptyaffiliation += 1
					count += 1
			fout.write("55:%d "%(emptyaffiliation))
			fout.write("56:%f "%(float(emptyaffiliation)/float(count)))
		fout.write("\n")

def write((author,publications)):
	global data
	fout = open(os.path.join("Feature",author+".feature"),'w')
	for (label,paper,Papers) in publications:
		fout.write("%s "%(label))
		data.writeentry(paper,author,Papers,fout)
	fout.close()

def main():
	global data
	data_dir = sys.argv[1]
	train_input = sys.argv[2]
	valid_input = sys.argv[3]
	train_output = sys.argv[4]
	valid_output = sys.argv[5]

	get_invalid_word_set(data_dir)

	data = DataCollection()
	data.readPaper("Cleaned")
	print("Read Paper.csv finish.")
	data.readAuthor("Cleaned")
	print("Read Author.csv finish.")
	data.readPaperAuthor("Cleaned")
	print("Read PaperAuthor.csv finish.")

	if not os.path.isdir("Feature"):
		os.mkdir("Feature")

	training = dict()
	with open(train_input, 'rb') as csvfile:
		reader = csv.reader(csvfile)
		reader.next()
		for row in reader:
			author = row[0]
			Confirmed = row[1].split()
			Deleted = row[2].split()
			training[author] = list()
			if data.authorspapers.has_key(author):
				Papers = data.authorspapers[author]
			else:
				Papers = Confirmed + Deleted
			for paper in Confirmed:
				training[author].append(("1",paper,Papers))
			for paper in Deleted:
				training[author].append(("-1",paper,Papers))
	pool = Pool(processes=6)
        pool.map(write,training.iteritems())
        pool.close()
        pool.join()

	testing = dict()
	with open(valid_input, 'rb') as csvfile:
		reader = csv.reader(csvfile)
		reader.next()
		for row in reader:
			author = row[0]
			Query = row[1].split()
			testing[author] = list()
			if data.authorspapers.has_key(author):
				Papers = data.authorspapers[author]
			else:
				Papers = Query
			for paper in Query:
				testing[author].append(("1",paper,Papers))
	pool = Pool(processes=10)
        pool.map(write,testing.iteritems())
        pool.close()
        pool.join()

	with open(train_input, 'rb') as csvfile, open(train_output,'w') as fout:
		reader = csv.reader(csvfile)
		reader.next()
	        for row in reader:
			author = row[0]
			Confirmed = row[1].split()
			Deleted = row[2].split()
			Papers = Confirmed + Deleted
			fin = open(os.path.join("Feature",author+".feature"),"r")
			for paper, feature in zip(Papers,fin):
				fout.write(feature)
			fin.close()

	with open(valid_input, 'rb') as csvfile, open(valid_output,'w') as fout:
		reader = csv.reader(csvfile)
		reader.next()
	        for row in reader:
			author = row[0]
			Papers = row[1].split()
			fin = open(os.path.join("Feature",author+".feature"),"r")
			for paper, feature in zip(Papers,fin):
				fout.write(feature)
			fin.close()

if __name__=="__main__":
	main()
