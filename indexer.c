#include "headers.h"




int indexDir(treeNode **root, dirStr directory)
{
	//printf("\npath: %s", directory);
	if(directory == NULL)
	{
		return 0;
	}
	
	if(strlen(directory) > 2047)
	{
		printf("\nFile Path Length is Too Long.\n\n");
		exit(1);
		return 1;
	}
	
	DIR *dir;
	struct dirent *entry;

	//file
	if ( !( dir = opendir( directory ) ) )
	{
		if ( (access ( directory, F_OK ) == -1) || (access ( directory, R_OK ) == -1))
		{
			printf("\nUnable to Open File: %s\n\n", directory);
			closedir( dir );
			exit(1);
		}
		else
		{
			readFile(root, directory);
		}
		
		return 0;
	}
	
	//error
	if ( !(entry = readdir( dir ) ) )
	{
		return 1;
	}
	
	
	//recursive retrieval
	do
	{
		if ( (access ( directory, F_OK ) == -1) || (access ( directory, R_OK ) == -1))
		{
			closedir( dir );
			exit(1);
			return 1;
		}
		//dir
		if ( entry->d_type == DT_DIR )
		{
			//ignore ".." and "." listings
			if( ( strncmp( entry->d_name, ".", 1 ) == 0 ) || ( strncmp( entry->d_name, "..", 2 ) ) == 0 )
			{
				continue;
			}
			
			//make string, then recreate it from stack to heap
			/// [MALLOC]
			char newString[ 1024 ];
			int len = snprintf ( newString, sizeof(newString) - 1, "%s/%s", directory, entry->d_name );
			newString[len] = '\0';
			
			//should add current directory to the dirHead
			
			//pushStack(dirHead,newString);
			//printf("\nindexer: %s", (dirStr)(*dirHead)->node);

			indexDir(root, newString);

		}
		//file
		else
		{
			//ignore temp files
			if( ( entry->d_name[ strlen( entry->d_name ) - 1 ] != '~' ) )
			{
				/// [MALLOC]
				char *newString = ( char* )malloc( strlen( directory ) + strlen( entry->d_name ) + 2 );
					strcpy( newString, directory );
					strcat( newString, "/" );
					strcat( newString, entry->d_name );
					newString[ strlen( newString ) ] = '\0';
				//printf("\nnewstring: %s", newString);
				//pushStack(fileHead, newString);
				//printf("\nindexer: %s", (dirStr)(*fileHead)->node);
				readFile(root, newString);
				free(newString);
			}
		}
	}
	while ( ( entry = readdir( dir ) ) );
	closedir( dir );
	
	return 0;
}

int readFile(treeNode **fileHead, dirStr file)
{
	FILE *fp;
	long fileSize = 0;
	
	fp = fopen(file, "r");
	
	if(!fp)
	{
		fprintf(stderr, "Could not open file:\n %s.\n\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
		
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	if(fileSize == 0)
	{
		return;
	}
	
	//char *contentString = (char*)malloc(fileSize + 1);
	//char *contentString = calloc(fileSize, 1);
	char *contentString = malloc(fileSize + 1);
	fread(contentString, fileSize, 1, fp);
		stringNonAlphaToSpace(contentString);
		stringLowerCase(contentString);
	char *tempToken;
	tempToken = strtok(contentString, " ");
	
	while(tempToken != NULL)
	{
		insertNode(tempToken, file, fileHead);
		//printf("\ndir: %s\ntoken: %s\n", file, tempToken);
		tempToken = strtok(NULL, " ");
	}
	
	
	
	return 0;
}
