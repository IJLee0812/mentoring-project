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

typedef struct{
	DIR *dp;
	char Nname[MAX];
	struct Node* next;
}Node;

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

void fileSize_dfs(int argc, char* argv[]);
void push(Stack* , Node*);
Node* initNode(DIR*, char*, Node*);
void pop(Stack*);
void fileSize_bfs(int argc, char* argv[]);
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
