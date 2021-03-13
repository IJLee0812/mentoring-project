#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <errno.h>

#define PathMax 255
#define MAX 1000

typedef struct Node{
	DIR *dirp;			
	char name[MAX];		
	struct Node* next;
}Node;

typedef struct Stack{
	Node* top;
	int size;
}Stack;

char* getDirName(char*);
void printDir(char*, char*);
void fileSize(int, char**);
void init(char *);
void selectmod(char*);

void bfs(char *, char *);
void dfs(char *, char *); 
void dfs_print(int, char *); 
char *strrev(char *);
void fileSize(int argc, char* argv[]);
void push(Stack* , Node*);
Node* initNode(DIR*, char*, Node*);
void pop(Stack*);
