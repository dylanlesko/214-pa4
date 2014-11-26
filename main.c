#include "headers.h"
#include <time.h>

void sa(treeNode *root, stringList *tokens);
void so(treeNode *root, stringList *tokens);
void sx(treeNode *root, stringList *tokens);
void saTokens(treeNode *root, tokStr term, pathList **list);
void saRecords(pathList *root, pathList **list);
void soSearch(treeNode *root, tokStr term);
void readIndex(dirStr path, treeNode** head);
void saPrint(pathList *root, int count);
void sxPrint(pathList *root, int count);
void new(stringList *args);
void changeIndex( treeNode **root, stringList *args );
void dump( treeNode *root );
void help();

//reads an inverted index into memory
void readIndex(dirStr path, treeNode** head)
{
	FILE *fp;
	fp = fopen(path, "r");
	
	//char test[1028];
	int count;
	
	//tries to open the file
	if( fp != NULL)
	{
		char line[2048];
		//reads the inverted index into our binary tree struct
		while (fgets(line, sizeof(line), fp) != NULL)
		{				
			char test[2048];
			char token[2048];
			if( strncmp("<list>", line, 6) == 0)
			{
				fgets(line, sizeof(line), fp);
				strcpy(test, line);
				int hacky = 0;
				while( strncmp("</list>", line, 7) != 0)
				{
					fgets(line, sizeof(line), fp);
					if(hacky%2 == 0)
					{
						if( strncmp("</list>", line, 7) != 0 )
						{
							strcpy(token, line);
							
							if( token[strlen(line) - 1] == '\n')
							{
								token[strlen(line) - 1] = '\0';
							}
						}
					}
					else
					{
						int i = atoi(line);
						while(i > 0)
						{
							i--;
							insertNode(test, token, head);
						}
					}
					hacky++;
				}
			}
		}
		//close that bitch down
		fclose(fp);
	}
	//if the file was not able to be opened end the program
	else
	{
		fprintf(stderr, "Could not open file:\n %s.\n\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	
}

//loops command options to the user
int main ( int argc, char *argv[ ] ) 
{
	system("clear");
	
	if(argc != 2)
	{
		printf(	"\nIncorrect Number of Arguments.\n./search <index file>\n\n");
		return 1;
	}
	

	dirStr inputPath = string_init( argv[ 1 ] );
	treeNode **tokenRecords = tree_init();

	readIndex(inputPath, tokenRecords);
		
	int switcher = 0;
	char* input;
	char* command;
	char* terms;
	
	stringList *searchTerms;
	
	while(1)
	{
		searchTerms = NULL;
		
		printf(MAKE_BLUE"\nEnter a command:");
		printf("\n\tsa <term(S)>");
		printf("\n\tso <term(s)>");
		printf("\n\tsx <term(s)>");
		printf("\n\tnw <output> <input>");
		printf("\n\tld <new index file>");
		printf("\n\tdp");
		printf("\n\thelp");
		printf("\n\texit");
		printf("\n"RESET_FORMAT);

		input = malloc(4096);
		fgets(input, 4096, stdin);
		
		terms = malloc(strlen(input) + 1);
		char *tok = strtok(input, " \n");
		
		if(tok != NULL)
		{
			command = malloc(strlen(tok) + 1);
			strcpy(command, tok);
			tolower(command);

			tok = strtok(NULL, " \n");
			
			while(tok != NULL)
			{
				if(searchTerms == NULL)
				{
					searchTerms = malloc(sizeof(stringList));
					searchTerms->token = string_init(tok);
					searchTerms->next = NULL;
				}
				else
				{
					stringList *newNode = malloc(sizeof(newNode));
					(*newNode) = (*searchTerms);
					(searchTerms->next) = (newNode);
					searchTerms->token = string_init(tok);
				}
				tok = strtok(NULL, " \n");
			}
			
			stringList *iter = searchTerms;
			
			while(iter != NULL)
			{
				tolower(iter->token);
				iter = iter->next;
			}

			if( strcmp(command, "sa") == 0 )
				switcher = 1;
			else if( strcmp(command, "so") == 0 )
				switcher = 2;
			else if( strcmp(command, "sx") == 0 )
				switcher = 3;
			else if( strcmp(command, "nw") == 0 )
				switcher = 4;
			else if( strcmp(command, "ld") == 0 )
				switcher = 5;
			else if( strcmp(command, "dp") == 0 )
				switcher = 6;
			else if( strcmp(command, "help") == 0 )
				switcher = 7;
			else if( strcmp(command, "exit") == 0 || strcmp(command, "q") == 0 )
				exit(0);
			else
				switcher = 0;

			switch(switcher)
			{
				case 1:
					sa(*tokenRecords, searchTerms);
					break;
				case 2:
					so(*tokenRecords, searchTerms);
					break;
				case 3:
					sx(*tokenRecords, searchTerms);
					break;
				case 4:
					new(searchTerms);
					break;
				case 5:
					changeIndex(tokenRecords, searchTerms);
					break;
				case 6:
					dump(*tokenRecords);
					break;
				case 7:
					help();
					break;
					
				default:
					printf("\nCommand not found.\n\n");
					break;
			}

			FREE(input);
			FREE(terms);
			FREE(command);
			freeStringList(searchTerms);
		}
		else
		{
			printf("\ninvalid command.\n\n");
		}

	}
	
	//printf("\n\n");
	return 0;	
}

//help explains the commands further
void help()
{
	printf(MAKE_GREEN"\nsa"RESET_FORMAT);
	printf(MAKE_GREEN"\n\tString AND\n\tThis command gives a list of path files that contain all the terms(arguments) in the sa command."RESET_FORMAT);
	
	printf(MAKE_GREEN"\nso"RESET_FORMAT);
	printf(MAKE_GREEN"\n\tString OR\n\tThis command gives a list of path files that contain any of the terms(arguments) specirfied for the so command."RESET_FORMAT);

	printf(MAKE_GREEN"\nsx"RESET_FORMAT);
	printf(MAKE_GREEN"\n\tString XOR\n\tList of file paths that only contain one of the input terms."RESET_FORMAT);
	
	printf(MAKE_GREEN"\nnw"RESET_FORMAT);
	printf(MAKE_GREEN"\n\tNew Inverted-File\n\tUses the inverter file to create a new inverted text based on the arguments that are passed to it."RESET_FORMAT);

	printf(MAKE_GREEN"\nld"RESET_FORMAT);
	printf(MAKE_GREEN"\n\tLoad Inverted-File\n\tgives the ability to re-input an inverted text file."RESET_FORMAT);
	
	printf(MAKE_GREEN"\ndp"RESET_FORMAT);
	printf(MAKE_GREEN"\n\tdumps all the tokens to the console."RESET_FORMAT);
	
	printf("\n\n");
	
}

//dump outputs everything in the tree
void dump( treeNode *root )
{
	printf(MAKE_RED"\n\tTokens:"RESET_FORMAT);
	traverse(root);
}

//allows the user to choose a new inverted index
void changeIndex( treeNode **root, stringList *args )
{
	if(args == NULL)
	{
		printf("\nNo file selected.");
		return;
	}
	if(args->token == NULL)
	{
		printf("\nInvalid index file.");
		return;
	}
	freeTree(*root);
	readIndex(args->token, root);
}

//alows the user to create a new inverted index
void new(stringList *args)
{
	if(args == NULL)
	{
		printf("\nNo output file selected.");
		return;
	}
	if(args->token == NULL)
	{
		printf("\nInvalid output file.");
		return;
	}
	if(args->next == NULL)
	{
		printf("\nNo input file selected.");
		return;
	}
	if(args->next->token == NULL)
	{
		printf("\nInvalid input file.");
		return;
	}
	char newIndexer[2048];
	strcpy(newIndexer, "./indexer");
	strcat(newIndexer, " ");

	strcat(newIndexer, args->next->token);
		strcat(newIndexer, " ");

		strcat(newIndexer, args->token);
	system("make indexer");
	system(newIndexer);
	return;
}

//exclusive or search
void sx(treeNode *root, stringList *tokens)
{
	printf(MAKE_RED"\nTokens:"RESET_FORMAT);
	stringList *iter = tokens;
	pathList *records = NULL;
	int count = 0;
	

	while (iter != NULL)
	{
		printf(MAKE_RED"\n\t%s"RESET_FORMAT, iter->token);
		saTokens(root, iter->token, &records);
		iter = iter->next;
		count++;
	}
	
	sxPrint(records, count);
	
	freePathList(records);
}


void sxPrint(pathList *root, int count)
{
	pathList *iter = root;
	
	printf(MAKE_RED"\n\tfound in:"RESET_FORMAT);
	while(iter != NULL)
	{
		if( iter->count == 1)
			printf("\n\t%s", iter->path);
		iter = iter->next;
	}
	printf("\n\n");
}

//or search
void so(treeNode *root, stringList *tokens)
{
	
	stringList *iter = tokens;
	
	printf(MAKE_RED"\nTokens:"RESET_FORMAT);
	while (iter != NULL)
	{
		printf(MAKE_RED"\n\t%s\n\tfound in:\n"RESET_FORMAT, iter->token);
		soSearch(root, iter->token);
		iter = iter->next;
	}

}
void soSearch(treeNode *root, tokStr term)
{
	if( root == NULL)
	{
		return;
	}
	if( strncmp(root->token, term, strlen(term)) == 0 )
	{
		traverseRecords(root->pathRecords);
		return;
	}
	else
	{
		soSearch(root->left, term);
		soSearch(root->right, term);
	}
}

//and search
void sa(treeNode *root, stringList *tokens)
{
	stringList *iter = tokens;
	pathList *records = NULL;
	int count = 0;
	
	printf(MAKE_RED"\nTokens:\n"RESET_FORMAT);

	while( iter != NULL)
	{
		printf(MAKE_RED"\t%s\n"RESET_FORMAT, iter->token);
		saTokens(root, iter->token, &records);

		iter = iter->next;
		count++;
	}
	printf(MAKE_RED"\tfound in:"RESET_FORMAT);

	saPrint(records, count);
	
	freePathList(records);

}
void saTokens(treeNode *root, tokStr term, pathList **list)
{
	if( root == NULL)
	{
		return;
	}
	
	if( strncmp(root->token, term, strlen(term) ) == 0 )
	{
		saRecords(root->pathRecords, list);
		return;
	}
	else
	{
		saTokens(root->left, term, list);
		saTokens(root->right, term, list);
	}
}
void saRecords(pathList *root, pathList **list)
{
	if( root == NULL)
	{
		return;
	}
	
	insertRecord(root->path, list);
	saRecords(root->next, list);
}
void saPrint(pathList *root, int count)
{
	pathList *iter = root;
	
	while(iter != NULL)
	{
		if( iter->count == count)
		{
			printf("\n\t%s", iter->path);
		}
		iter = iter->next;
	}
	printf("\n\n");
}
