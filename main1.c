#include "headers.h"
#include <time.h>

int main ( int argc, char *argv[ ] ) 
{
	clock_t begin, end;
	double time_spent;

begin = clock();
	system("clear");
	if(argc != 3)
	{
		printf(	"\nIncorrect Number of Arguments.\n./indexer <outputfile> <dir or path>\n\n");
		return 1;
	}
	
	///	[MALLOC]
	dirStr inputPath = string_init( argv[ 2 ] );
	///	[MALLOC]
	dirStr outputPath = string_init( argv[ 1 ] );
	
	treeNode **tokenRecords = tree_init();
	
	indexDir(tokenRecords, inputPath);

	
	
	//printTree(*tokenRecords);
	
	
	output(outputPath, tokenRecords);
	
	
	end = clock();
	time_spent = (double)(end - begin) / (double)CLOCKS_PER_SEC;
	printf("\ntime: %f seconds", time_spent );
	
	
	printf("\n\n");
	return 0;	
}
