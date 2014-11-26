#include "headers.h"

stringStructPtr string_init ( char* string )
{
		
	if(string == NULL)
	{
		//printf("\nInvalid String.\n\n");
		//exit(1);
		return NULL;
	}
	
	stringStructPtr newStruct = (stringStructPtr)malloc(strlen(string) + 1);
		strcpy(newStruct, string);
		(newStruct)[strlen(string)] = '\0';
		
	return newStruct;
}

treeNode** tree_init ( )
{
	treeNode **newNode = (treeNode**)malloc(sizeof(treeNode));

	return newNode;
}


void traverse(treeNode *root)
{
	if(root == NULL)
	{
		return;
	}

	printf(MAKE_RED"\ttoken: %s"RESET_FORMAT, root->token);
	
	mergeSort( &(root->pathRecords) );

	traverseRecords(root->pathRecords);
	traverse(root->left);
	traverse(root->right);
}

void traverseRecords(pathList *root)
{
	if(root == NULL)
	{
		return;
	}
	
	printf("\tpath: %s\n\tcount: %d\n\n", root->path, root->count);
	traverseRecords(root->next);
}



void printTree(FILE *fp, treeNode *root)
{
	if(root == NULL)
	{
		return;
	}

	//printf("\n\n%-30s", root->token);
	fprintf(fp, "<list>\n");
	fprintf(fp, "%s\n", root->token);
	
	mergeSort( &(root->pathRecords) );

	printRecords(fp, root->pathRecords);
	fprintf(fp, "</list>\n");
	printTree(fp, root->left);
	printTree(fp, root->right);
	

}
void printRecords(FILE *fp, pathList *root)
{
	if(root == NULL)
	{
		return;
	}
	
	fprintf(fp, "%s\n%d\n", root->path, root->count);
	//printf("\n%-50s\t\t%d", root->path, root->count);
	printRecords(fp, root->next);
}

int insertNode(tokStr token, dirStr path, treeNode **root)
{		
	if( (*root) == NULL)
	{
		(*root) = (treeNode*)malloc(sizeof(treeNode));
		(*root)->token = string_init(token);
		(*root)->left = NULL;
		(*root)->right = NULL;
		(*root)->pathRecords = NULL;
	}
	else
	{
		int cmpVar;
		cmpVar = strcmp(token, (*root)->token);

		if(cmpVar < 0)
		{
			insertNode(token, path, &((*root)->left));		return 0;
		}
		if(cmpVar > 0)
		{
			insertNode(token, path, &((*root)->right));		return 0;
		}
		

	}

	insertRecord(path, &((*root)->pathRecords));

	return 0;
}

int insertRecord(dirStr path, pathList **root)
{
	if( (*root) == NULL)
	{
		(*root) = malloc(sizeof(pathList));
		(*root)->path = string_init(path);
		(*root)->count = 1;
		(*root)->next = NULL;
	}
	else
	{
		int cmpPath = strcmp( path, (*root)->path );
		
		if(cmpPath == 0)
		{
			(*root)->count += 1;
		}
		else
		{
			insertRecord(path, &((*root)->next) );
		}

	}
	return 0;
}





int output(dirStr path, treeNode **root)
{
	
	if( access (path, F_OK) != -1)
	{
		printf("\nFile Already Exists. Overwrite? (Y/N)?: \n");
		
		if(tolower(getchar()) == 'y')
		{
			remove(path);
		}
		else
		{
			printf("\nProgram Terminated.\n");
			return 1;
		}
		
	}
	
	//strim(path);
	
	
	FILE *fp;
	fp = fopen(path, "ab+");
	
	if(!fp)
	{
		printf("\ncould not open output file.\n\n");
		exit(1);
	}
	
	printTree(fp, *root);
	
	fclose(fp);
	
	return 0;
}



void mergeSort(pathList **root)
{
	pathList *head = *root;
	pathList *a;
	pathList *b;
	
	if ( (head == NULL) || (head->next == NULL) )
	{
		return;
	}
	
	mergeSplit(head, &a, &b);
	mergeSort(&a);
	mergeSort(&b);
	(*root) = sortedMerge(a, b);
}


pathList* sortedMerge(pathList *a, pathList *b)
{
	pathList *result = NULL;
	
	if (a == NULL)
		return b;
	else if (b == NULL)
		return a;
		
		if ( (a->count > b->count) )
		{
			result = a;
			result->next = sortedMerge(a->next, b);
		}
		else
		{
			result = b;
			result->next = sortedMerge(a, b->next);
		}
	return result;

}

void mergeSplit(pathList *head, pathList **back, pathList **front)
{
	pathList *fast;
	pathList *slow;
	
	if ( (head== NULL) || (head->next == NULL) )
	{
		*front = head;
		back = NULL;
	}
	else
	{
		slow = head;
		fast = head->next;
		
		while(fast != NULL)
		{
			fast = fast->next;
			if(fast != NULL)
			{
				slow = slow->next;
				fast = fast->next;
			}
		}
		
		*front = head;
		*back = slow->next;
		slow->next = NULL;
		
	}
	
}





void freeStringList(stringList *head)
{
	if(head == NULL)
		return;
	
	stringList *iter = head;
	stringList *temp;
	
	while (iter != NULL)
	{
		temp = iter;
		iter = iter->next;
		FREE(temp->token);
		FREE(temp);
	}
	
	head = NULL;

}


void freePathList(pathList *head)
{
	if(head == NULL)
		return;
		
	pathList *iter = head;
	pathList *temp;
	
	while (iter != NULL)
	{
		temp = iter;
		iter = iter->next;
		FREE(temp->path);
		FREE(temp);
	}
	
	head = NULL;
}

void freeTree(treeNode *head)
{
	if(head == NULL)
	{
		return;
	}
	
	freeTree(head->left);
	freeTree(head->right);
	
	freePathList(head->pathRecords);
	FREE(head->token);
	FREE(head);
}





