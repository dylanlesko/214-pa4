#ifndef INVERTED_LIST_H
#define INVERTED_LIST_H

typedef char* dirStr;
typedef char* tokStr;
typedef char* stringStructPtr;

typedef struct pathList
{
	dirStr path;
	int count;
	struct pathList *next;

} pathList;

typedef struct treeNode
{
  tokStr token;
  pathList *pathRecords;
  struct treeNode *left;
  struct treeNode *right;
} treeNode;

typedef struct stringList
{
	tokStr token;
	struct stringList *next;
} stringList;


treeNode** tree_init ( );
stringStructPtr string_init ( char* string );
int insertNode(tokStr token, dirStr path, treeNode **fileHead);
int insertRecord(dirStr path, pathList **root);

void printTree(FILE *path, treeNode *root);
void printRecords(FILE *path, pathList *root);

int output(dirStr path, treeNode **root);
	
	
void mergeSort(pathList **root);


pathList* sortedMerge(pathList *a, pathList *b);
void traverse(treeNode *root);
void traverseRecords(pathList *root);

void mergeSplit(pathList *head, pathList **back, pathList **front);
	void freeStringList(stringList *list);
void freePathList(pathList *head);



#endif
