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
#define STR_MAX 255


/*NODE && QUE*/
typedef struct NODE{
	DIR *dp;
	char Nname[MAX];
	struct NODE* next;
}NODE;

typedef struct{ //argv[1]
	NODE* top;
	int size;
}Stack;

typedef struct{ //argv[1]
	char (*pathptr)[STR_MAX];
	int front;
	int rear;
	int capacity;
}Queue;

typedef struct Queue{ //argv[2]
	struct NODE* front;
	struct NODE* rear;
	int qSize;
}QUE;



/*functions*/

//익준
//CO
void init(char *);
void selectmod(char *, int);
char *strrev(char *);
//BFS
void Bfs_for_SearchTree(char *, char *); 
int havedir(char *); 
//DFS
void Dfs_for_SearchTree(char *, char *); 
void Dfs_for_PrintTree(int, char *); 

//준호
//CO
char* absolute(char*);
void bfs_or_dfs(char*,int);
//BFS
int Bfs_for_Size(char*);
void enqueue_for_Size(Queue* , char*);
void dequeue_for_Size(Queue*);
_Bool isFull(Queue*);
void expand_Capacity(Queue*);
//DFS
int Dfs_for_Size(char*);
void push(Stack* , NODE*);
NODE* initNODE(DIR*, char*, NODE*);
void pop(Stack*);

//지우
//CO
char* extract_Filename(char*);
char* set_Parentdir_Path(char*);
void before_Search(char*, char*, int);
//BFS
_Bool isEmpty(int);
void enQue_for_Path(QUE*, struct NODE*);
void deQue_for_Path(QUE*);
struct NODE* create_NODE(DIR* dp, char* Nname);
void Bfs_for_Path(char* toFind, char* workDir);
//DFS
void Dfs_for_Path(char*, char*);
