#include<cstdio>
#include<iostream>
#include<cstring>
#include<string>
#include<cstdlib>
#include<map>
#define L 4096 

using namespace std ;

int main( int argc, char *argv[] )
{
	if( argc<5 )
	{
		fprintf( stderr, "%s label_1 label_2 output features \n", argv[0] ) ;
		return 0 ;
	}
	int N = argc-4 ;
	
	FILE *fp[N], *f_w ;
	int d[N] ;
	char s[L], *p ;
	map<string, int> label_map ;

	for( int i = 0 ; i<N ; i++ )
		fp[i] = fopen( argv[i+4], "r" ) ;
	f_w = fopen( argv[3], "w" ) ;
	
	//find d
	int l_count = 0 ;
	d[ N-1 ] = 0 ;
	for( int i=0 ; i<N-1 ; i++ )
	{
		d[i] = 0 ;
		while( fgets( s, L, fp[i] ) != NULL )
		{
			p = strtok( s, " :\n" ) ;
			if( i == 0 && l_count < 2)
				if( label_map.find( string(p) ) == label_map.end() )
				{
					l_count++ ;
					label_map[ string(p) ] = l_count ;
				}
			p = strtok(NULL, " :\n") ;
			while( p != NULL )
			{
				if( atoi(p) > d[i] )
					d[i] = atoi(p) ;
				p = strtok(NULL, " :\n") ;
				p = strtok(NULL, " :\n") ;
			}
		}
	}
	//print
	for( int i=0 ; i<N ; i++ )
		fseek( fp[i], 0, SEEK_SET ) ;
	int cum ;
	int flag = 0 ;
	while( fgets( s, L, fp[0] ) != NULL )
	{
		if( flag == 1 )
			fprintf( f_w, "\n" ) ;
		flag = 1 ;
		p = s ;
		s[ strlen(s)-1 ] = 0 ;
		while( *p != ' ' )
			p++ ;
		*p = 0 ;
		p++ ;
		fprintf( f_w, "%s %s", argv[ label_map[string(s)] ], p ) ;
		cum = d[0] ;
		for( int i=1 ; i<N ; i++ )
		{
			fgets( s, L, fp[i] ) ;
			p = strtok( s, " :\n" ) ;
			p = strtok(NULL, " :\n") ;
			int idx ;	
			while( p != NULL )
			{
				idx = atoi(p) + cum ;
				p = strtok(NULL, " :\n") ;
				fprintf( f_w, " %d:%s", idx, p ) ;
				p = strtok(NULL, " :\n") ;
			}
			cum += d[i] ;
		}
	}
	fclose( f_w ) ;
	for( int i=0 ; i<N ; i++ )
		fclose( fp[i] ) ;
}


