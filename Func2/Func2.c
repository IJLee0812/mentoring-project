#include "head.h"
#define MAX 1000


/*NODE && QUE*/
typedef struct NODE{
	DIR* dp;
	char name[MAX]; 
	struct NODE * next;
}NODE;

typedef struct Queue{
	struct NODE* front;
	struct NODE* rear;
	int qSize;
}QUE;



/*functions*/
_Bool isEmpty(int);
void enQue(QUE*, struct NODE*);
void deQue(QUE*);
struct NODE* createNode(DIR* dp, char* name);
void printQ(QUE*);
void dirBFS(char* toFind, char* workDir);
void dirDFS(char*, char*);
char* getDirName(char*);
void printDir(char*, char*, int);



/* isEmpty: qSize를 통해 q가 비어있는지 확인한다. (비어있으면 리턴true) */

_Bool isEmpty(int qSize){
	return (qSize==0);
}



/*
	enQue: 
   		1. q가 비어있는 경우, q의 front와 rear를 newnode로 일치시킨다.
		2. q가 비어있지 않은 경우, q의 마지막에 newnode를 배치한다.
		3. qSize를 하나 늘린다.
 */

void enQue(QUE* q, struct NODE* newnode){
	if (isEmpty(q->qSize)){
		q->front = q->rear = newnode;
	}
	else {
		q->rear->next = newnode;
		q->rear = q->rear->next;
	}
	q->qSize++;
}



/*
   deQue
   		1. q가 비어있으면 error 문구를 출력한다.
		2. q의 front를 delete한다.
		3. qSize를 하나 줄인다.
 */

void deQue(QUE* q){
	if (isEmpty(q->qSize)){
		fprintf(stderr,"delete error: QisEmpty\n");
		return;
	}
	struct NODE *tmp = q->front;
	q->front = q->front->next;
	free(tmp);
	q->qSize--;
}



/* createNode: Node를 동적할당하고, dp와 name을 init해 리턴한다. */

struct NODE* createNode(DIR* dp, char* name){
	struct NODE* newnode = (NODE*)malloc(sizeof(NODE));
	//init
	newnode->dp = dp;
	strcpy(newnode->name, name);
	return newnode;
}



/*
void printQ(QUE* Q){
	NODE* horse = Q->front;
	int i=0;
	printf("\n\n----------Q----------");
	while (horse!=NULL){
		printf("Q%d->name: %s\n", i++, horse->name);
		horse = horse->next;
	}
	printf("--------------------\n\n");
}
*/




/*
	dirBFS:
		1. bfs 탐색을 통해 toFind의 위치를 찾는다.
		2. 찾았을 경우, 그 경로를 출력한다. (못 찾으면 출력하지 않음)

 */
void dirBFS(char* toFind, char* workDir){

	//prepare
	struct dirent *dir = NULL;
	QUE q = {NULL, NULL, 0};
	struct NODE* firstNode = createNode(NULL, workDir);
	enQue(&q, firstNode);
	if ((q.front->dp = opendir(workDir))==NULL){
		perror("File Open Error\n");
		exit(1);
	}


	//bfs
	while(q.qSize!=0){
		while((dir = readdir(q.front->dp))!=NULL){
			struct stat statbuf;
			
			//check
			if (strcmp(dir->d_name, toFind)==0){
				printf("[ Path: %s/%s ]\n", q.front->name, dir->d_name);
				closedir(q.front->dp);
				break;
			}

			//pass
			if (strncmp(dir->d_name,".",1)==0){
				continue;
			}

			//enque
			char tmp[MAX];
			strcpy(tmp, q.front->name);
			strcat(tmp,"/");
			strcat(tmp, dir->d_name);
			if (lstat(tmp, &statbuf)<0){
				perror("Stat Error\n");
				exit(1);
			}
			
			if (S_ISDIR(statbuf.st_mode)){ //dir이면enque
				struct NODE* n = createNode(opendir(tmp),tmp);
				enQue(&q,n);
			}
		
		}
		deQue(&q); //q의 front를 deQ
	}
}



/*
	dirDFS:
		1. dfs 탐색을 통해 toFind의 위치를 찾는다.
		2. 찾았을 경우, 그 경로를 출력한다. (못 찾으면 출력하지 않음)
 */

void dirDFS(char* toFind, char* workDir) {

	//prepare
	DIR* dp = NULL;
	struct dirent *dir = NULL;
	struct stat statbuf;
	if ((dp = opendir(workDir))==NULL){
		perror("File Open Error\n");
		exit(1);
	}


	//dfs
	while((dir=readdir(dp))!=NULL){

		//check
		if (strcmp(dir->d_name, toFind) == 0){
			printf("[ Path: %s/%s ]\n", workDir, dir->d_name);
			closedir(dp);
			return;
		}
	
		//pass
		if ((strncmp(dir->d_name,".",1)==0)){
			continue;
		}
		
		//recurse
		char tmp[MAX];
		strcpy(tmp,workDir);
		strcat(tmp,"/");
		strcat(tmp,dir->d_name);
		lstat(tmp, &statbuf);

		if (S_ISDIR(statbuf.st_mode)){ //dir이면 recurse
			dirDFS(toFind, tmp);
		}
	}
}




/*
	getDirName:
		1. 입력받은 dir 경로에서 dir 이름을 리턴한다.
		2. dir 이름을 입력받은 경우 그대로 리턴한다.
 */

char* getDirName(char* DirPath){

	//No slashes
	if (strstr(DirPath,"/")==NULL) {
		return DirPath;
	}

	//Slashes
	char* temp = (char*)malloc(sizeof(char)*MAX);
	char* ptr = strtok(DirPath,"/");
	while (ptr!=NULL) {
		strcpy(temp, ptr);
		ptr = strtok(NULL,"/");
	}
	return temp;

}


/*
	printDir:
		1. "."을 입력받은 경우, 탐색하지 않고 바로 경로를 출력한다.
		2. "."부터 시작하는 경로를 입력받은 경우, 현재 디렉토리부터 탐색 후 경로를 출력한다.
		3. 그 외의 경우, 루트부터 탐색 후 경로를 출력한다.
 */

void printDir(char* argv, char* toFind, int BD){
	
	char workDir[MAX];
	struct passwd *pwd;
	errno = 0;
	if((pwd = getpwuid(getuid())) == NULL) { 
		if(errno == 0 || errno == ENOENT || errno == ESRCH || errno == EBADF || errno == EPERM) { 
			printf("미등록된 사용자입니다.\n"); 
		} 
		else { 
			printf("error: %s\n", strerror(errno)); 
		}
		return;	
	}

	//Branching
	if (strcmp(".", argv) == 0){ //탐색안하는경우 
		printf("왜 들어오냐\n");   // ./test 이런 친구들???
		getcwd(workDir,MAX);
		printf("[ Path: %s ]\n", workDir);
		return;
	}
	else if (strncmp("./", argv, 2) == 0){ //탐색하는경우 (.)
		getcwd(workDir, MAX);
	}
	else{ //탐색하는경우 (/)
		strcpy(workDir,pwd->pw_dir);
	}
	
	//Searching
	if (BD==0){
		dirBFS(toFind, workDir);
	}
	else{
		dirDFS(toFind,workDir);
	}

}




int main(int argc, char* argv[]){

	if(argc!=3){
		fprintf(stderr,"Usage: Program Dir File\n");
		exit(1);
	}


	int BD;
	printf("Search Option(BFS:1, DFS:2): ");
	scanf("%d",&BD);
	

	char* toFind = getDirName(argv[2]);
	switch(BD){
		case 1:
			printf("BFS Search\n");
			printDir(argv[2], toFind, 0);
			break;
		case 2:
			printf("DFS Search\n");
			printDir(argv[2], toFind, 1);
			break;
		default:
			printf("Select Again\n");
	}

	return 0;

}
