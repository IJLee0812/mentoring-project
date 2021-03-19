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
void init(char *); //use: main //init workdir
void selectmod(char *, int); //use: main
char *strrev(char *); //reverse str
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
char* extract_Filename(char*); //argv[2]에서 파일명/디렉토리명만 추출
char* set_Parentdir_Path(char*); //현재 디렉토리를 기준으로 부모 디렉토리의 경로를 설정
void before_Search(char*, char*, int); //탐색 전, 일정 기준(탐색o/x, 상대/절대경로, BFS/DFS)별로 탐색 방법을 결정
//BFS
_Bool isEmpty(int); //큐가 비어있는지 아닌지 확인
void enQue_for_Path(QUE*, struct NODE*); //큐의 끝에 노드를 in
void deQue_for_Path(QUE*); //큐의 front를 out
struct NODE* create_NODE(DIR* dp, char* Nname); //노드를 만들고 데이터 초기화
void Bfs_for_Path(char* toFind, char* workDir); //BFS로 toFind의 위치를 찾아 경로 출력
//DFS
void Dfs_for_Path(char*, char*); //DFS로 toFind의 위치를 찾아 경로 출력
