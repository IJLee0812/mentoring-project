#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <errno.h>

#define MAX 1000

typedef struct NODE{
	DIR *dp;
	char Nname[MAX];
	struct NODE* next;
}NODE;

typedef struct{
	char (*pathptr)[STR_MAX];
	int front;
	int rear;
	int capacity;
}Queue;

typedef struct{
	Node* top;
	int size;
}Stack;

typedef struct Queue{
	struct NODE* front;
	struct NODE* rear;
}QUE;

void init(char *);
void selectmod(char *); 
void bfs(char *, char *); 
int checkdir(char *); 
void dfs(char *, char *); 
void dfs_print(int, char *); 
char *strrev(char *);

char* absolute(char*);
void dfs_or_bfs(char*,int);
int fileSize_dfs(char*);
void push(Stack* , NODE*);
NODE* initNODE(DIR*, char*, NODE*);
void pop(Stack*);
int fileSize_bfs(char*);
void enqueue(Queue* , char*);
void dequeue(Queue*);
_Bool isEmpty(Queue*);
_Bool isFull(Queue*);
void expandCapacity(Queue*);

_Bool isEmpty(int);
void enQue(QUE*, struct NODE*);
void deQue(QUE*);
struct NODE* createNode(DIR* dp, char* name);
void printQ(QUE*);
void dirBFS(char* toFind, char* workDir);
//DFS
void dirDFS(char*, char*);
//CO
char* getDirName(char*);
char* parents(char*);
void printDir(char*, char*, int);
