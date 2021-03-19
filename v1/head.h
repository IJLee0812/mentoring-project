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
#define Q_MAX 1000



/*NODE && QUE*/
typedef struct NODE{
	DIR *dp;
	char Nname[MAX];
	struct NODE* next;
}NODE;

typedef struct{
	NODE* top;
	int size;
}Stack;

typedef struct{
	char (*pathptr)[STR_MAX];
	int front;
	int rear;
	int capacity;
}Queue;

typedef struct Queue{
	struct NODE* front;
	struct NODE* rear;
	int qSize;
}QUE;



/*functions*/
//익준
void init(char *);
void selectmod(char *); 
void bfs(char *, char *); 
int checkdir(char *); 
void dfs(char *, char *); 
void dfs_print(int, char *); 
char *strrev(char *);
//준호
char* absolute(char*);
void dfs_or_bfs(char*,int);
int dfs_for_dirsize(char*);
void push(Stack* , NODE*);
NODE* initNODE(DIR*, char*, NODE*);
void pop(Stack*);
int bfs_for_dirsize(char*);
void enqueue(Queue* , char*);
void dequeue(Queue*);
_Bool isFull(Queue*);
void expandCapacity(Queue*);
//지우
//BFS
_Bool isEmpty(int); //큐가 비어있는지 아닌지 확인
void enQue(QUE*, struct NODE*); //큐의 끝에 노드를 in
void deQue(QUE*); //큐의 front를 out
struct NODE* create_Node(DIR* dp, char* Nname); //노드를 만들고 데이터 초기화
void BFS_for_Path(char* toFind, char* workDir); //BFS로 toFind의 위치를 찾아 경로 출력
//DFS
void DFS_for_Path(char*, char*); //DFS로 toFind의 위치를 찾아 경로 출력
//CO
char* extract_Filename(char*); //argv[2]에서 파일명/디렉토리명만 추출
char* set_Parentdir_Path(char*); //현재 디렉토리를 기준으로 부모 디렉토리의 경로를 설정
void before_Search(char*, char*, int); //탐색 전, 일정 기준(탐색o/x, 상대/절대경로, BFS/DFS)별로 탐색 방법을 결정
