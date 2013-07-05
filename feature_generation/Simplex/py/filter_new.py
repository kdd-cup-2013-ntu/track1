#!/usr/bin/env python2.7
import re, sys

def author_name_filter(name):

#	()
	name = re.sub(r'\(.*\)', r'', name)
#	-
	name = re.sub(r'-', r' ', name)
#	@
	name = re.sub(r'@.*$', r'', name)
#	?
	name = re.sub(r'\?', r'', name)
#	'
	name = re.sub(r'\'', r'', name)

#	deal with latin characters
	name = latin_filter(name)

#	else
	name = re.sub(r'[^A-Za-z()\s\?\-\.,]', r'', name)

	name = re.sub(r'(?i)^(Dr|Mr|Mrs|Ms)\. ', '', name)


#	name = re.sub(' (([Ss][Tt]|[Vv][Aa][Nn]|[Dd][Ee]|[Dd][Ee][Ll]|[Dd][Aa]|[Dd][Oo]|[Ee][Ll]|[Ll][Aa]|[Dd][Ii]|[Vv][Oo][Nn]|[Dd][Ee][Rr])\.? )+', ' ', name)
#	remove suffix and split AH to A H
	split = name.split()
	new_split = list()
	for part in split:
		if (not is_chinese_name(name)) and re.search(r'^(junior|jr|i|ii|iii|iv)\.?$', part.lower()):
			continue
		elif (not is_chinese_name(name)) and re.search(r'^(st|van|de|del|da|do||el|la|di|von|der)\.?$', part.lower()):
			continue
		elif re.search(r'^[A-Z][A-Z]$', part):
			new_split.extend([part[0], part[1]])
#		elif part[-1] == '.':
#			new_split.append(part[0])
		else:
			new_split.append(part)
		name = ' '.join(new_split)

	name = name.lower()
	name = split_chinese_concat_name(name)
#	.
	name = re.sub(r'\.', r' ', name)

#	\s+
	name = re.sub(r'\s+', r' ', name)
	
#	rstrip
	name = name.rstrip()

	return name

def author_affiliation_filter(affi):
	affi = latin_filter(affi)
#	<a></a>
	affi = re.sub(r'<.*?>', '', affi)	
#	?
	affi = re.sub(r'\?', '', affi)
#	|-,.:;*()[]/<>
	affi = re.sub(r'[|\-,.:;\*\(\)\[\]/<>]', ' ', affi)
#	&
#	TODO:&
#	others
	affi = re.sub(r'[^A-Za-z\s]', '', affi);
	
	affi = re.sub(r'\s+', ' ', affi)
	return affi

def paper_title_filter(title):
	title = latin_filter(title)
#	<a></a>
	title = re.sub(r'<.*?>', '', title)	
	title = re.sub(r'\W', '', title);
	return title.lower()


def latin_filter(name):
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
	#name = re.sub(r'[\xe0-\xef]..', '', name)
	#name = re.sub(r'[\xf0-\xf7]...', '', name)

	return name

if __name__=='__main__':
	print 'please type: '
	s = sys.stdin.readline()
	print author_name_filter(s.rstrip())
