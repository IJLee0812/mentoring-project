#include "head.h"

char wd[MAX];

/*익준*/

void init(char *name){ // 탐색할 디렉터리 이름(또는 경로), 탐색 시작 절대경로(프로그램 위치)출력
	printf("탐색할 디렉터리 이름(경로) : %s\n", name);
	getcwd(wd, BUFSIZ); // 프로그램 위치를 불러오기 위해 사용, wd에 저장
	printf("프로그램 위치(절대경로) : %s\n", wd); 
	printf("\n");
	return;
}

void selectmod(char *argv){
	int mod; printf("모드 선택 (0 : bfs, 1 : dfs) : "); scanf("%d", &mod); // bfs, dfs 모드 선택
	
	if (mod == 0){
		if (strncmp("/", argv, 1) == 0) // 절대 경로로 입력했을 경우
			bfs(argv, "..");
		else if (strncmp("..", argv, 2) == 0) // ../dirname의 경우
			bfs(argv, "..");
		else if (strcmp(".", argv) == 0) // . 입력했을 경우(프로그램 위치한 디렉터리) 탐색X, 출력
			dfs_print(0, ".");
		else // dirname 또는 상대경로로 입력했을 경우
			bfs(argv, ".");
	}
	
	else if (mod == 1){
		if (strncmp("/", argv, 1) == 0) // 절대 경로로 입력했을 경우
			dfs(argv, "..");
		else if (strncmp("..", argv, 2) == 0) // ../dirname의 경우
			dfs(argv, "..");
		else if (strcmp(".", argv) == 0) // . 입력했을 경우(프로그램 위치한 디렉터리) 탐색X, 출력
			dfs_print(0, ".");
		else // dirname 또는 상대경로로 입력했을 경우
			dfs(argv, ".");
	}
}

void bfs(char *name, char *wd){ 
	struct dirent *entry; struct stat buf; DIR *dp;
	Node queue[MAX]; int front, rear; front = rear = -1; // 큐 생성
	
	if (chdir(wd) < 0){ // 디렉터리 이동, 실패 시 프로그램 종료
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}
	
	if ((dp = opendir(".")) == NULL){ // 디렉터리 열기, 실패 시 프로그램 종료
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}

	while (1){ // BFS 알고리즘
		
		while ((entry = readdir(dp)) != NULL){ // 현재 디렉터리 내용을 모두 읽었을 때 탈출
			
			lstat(entry->d_name, &buf);
			
			if (S_ISDIR(buf.st_mode)){ // 하위 디렉터리일 경우
				
				char path[MAX];
				
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) // ".", ".."은 고려하지 않음
					continue;
				
				printf("탐색된 디렉터리 : %s\n", entry->d_name); 
				
				if (strncmp(strrev(name), strrev(entry->d_name), strlen(entry->d_name)) == 0){ // <입력한 이름(경로) == 탐색한 디렉터리명>인 경우					
					strrev(name); strrev(entry->d_name);   
					printf("\n%s----\n", entry->d_name); // 입력한(경로의) 디렉터리명 출력
					dfs_print(0, entry->d_name); // dfs_print함수 호출
					closedir(dp); return; // 반복문 탈출, bfs함수 종료
				}
				
				else{
				strrev(name); strrev(entry->d_name);
				}
				
				getcwd(path, BUFSIZ);
				strcat(path, "/");
				strcat(path, entry->d_name);
				
				if (checkdir(path)){ // 탐색 중인 디렉터리가 하위 디렉터리를 보유한 경우
					rear++; // 저장할 공간 확보
					strcpy(queue[rear].Nname, path);// 대상 디렉터리 경로 저장
					printf("Queue에 %s 디렉터리 정보(경로) 저장\n", entry->d_name);
				}
				
				chdir("..");
			}
		}
		// 같은 깊이의 모든 노드들의 탐색이 끝난 경우
		front++;
		printf("%s 디렉터리로 이동\n", queue[front].Nname); // 선입선출, 비었으면 함수 종료
		closedir(dp); chdir(queue[front].Nname);
		
		if ((dp = opendir(queue[front].Nname)) == NULL){ // Dequeue한 디렉터리로 이동
			printf("Dequeue 오류 발생. 프로그램 종료\n");
			exit(1);
		}
	}
	return;
}

int checkdir(char *path){ // 하위 디렉터리 보유 여부 체크 함수
	struct dirent *entry; struct stat buf; DIR *dp;
	
	chdir(path); dp = opendir(path);
	
	while ((entry = readdir(dp)) != NULL){
		
		lstat(entry->d_name, &buf);
		
		if (S_ISREG(buf.st_mode)) continue; 
		
		else if (S_ISDIR(buf.st_mode)){ // 하위 디렉터리가 있을 경우
			
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;
			printf("탐색된 디렉터리는 하위 디렉터리 존재\n");
			closedir(dp);
			return 1; // 1 반환, if문 True
		
		}
	}
	
	printf("탐색된 디렉터리는 하위 디렉터리 존재 X\n");
	closedir(dp);
	return 0; // 0 반환, if문 False
}

void dfs(char *name, char *wd){ // 입력한 디렉터리를 찾는 함수, dfs 알고리즘
	struct dirent *entry; struct stat buf; DIR *dp; 
	
	if (chdir(wd) < 0){ // 디렉터리 이동, 실패 시 프로그램 종료
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}
	
	if ((dp = opendir(".")) == NULL){ // 디렉터리 열기, 실패 시 프로그램 종료
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}
	
	while ((entry = readdir(dp)) != NULL){ // 현재 디렉터리 내용을 모두 읽었을 때 탈출
		
		lstat(entry->d_name, &buf);
		
		if (S_ISDIR(buf.st_mode)){ // 하위 디렉터리일 경우
			
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) // ".", ".."은 고려하지 않음
				continue;
			
			if (strncmp(strrev(name), strrev(entry->d_name), strlen(entry->d_name)) == 0){ // <입력한 이름(경로) == 탐색한 디렉터리명>인 경우
				strrev(name); strrev(entry->d_name); // 비교 위해 뒤집은 문자열 원상태로 복구
				printf("%s----\n", entry->d_name); // 입력한(경로의) 디렉터리명 출력
				dfs_print(0, entry->d_name); // dfs_print함수 호출
				chdir(".."); closedir(dp); return; // dfs 함수 종료
			}
			
			else{
				strrev(name); strrev(entry->d_name); // 비교 위해 뒤집은 문자열 원상태로 복구
			}
			
			dfs(name, entry->d_name); // 없을 경우, 재귀 호출(더 하위 디렉터리로 이동)
		}
	} 
	
	chdir(".."); closedir(dp); // 백트래킹(부모 디렉터리로 올라감)
}

void dfs_print(int tmp, char *wd){ // 디렉터리를 트리구조로 출력하는 함수, dfs 알고리즘
	struct dirent *entry; struct stat buf; DIR *dp; int count = 0;
	
	count = tmp; // \t 횟수 구분(깊이 동일한 노드들 같은 열에 출력하기 위함)
	
	if (chdir(wd) < 0){ // dfs()와 동일
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}
	
	if ((dp = opendir(".")) == NULL){
		printf("오류 발생! 프로그램 종료.\n");
	}  
	
	while ((entry = readdir(dp)) != NULL){ // dfs()와 동일
		
		lstat(entry->d_name, &buf);
		
		if (S_ISREG(buf.st_mode)){ // 탐색한 것이 파일일 경우
			
			for (int i = 0 ; i <= count ; i++)
				printf("\t");
			printf("-%s\n", entry->d_name);
		
		}
		
		else if (S_ISDIR(buf.st_mode)){ // 탐색한 것이 디렉터리일 경우
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) // dfs()와 동일
				continue;
			
			for (int i = 0 ; i < 3 ; i++){ // 칸을 벌려서 겹치는 경우 예방하기 위함
				for (int j = 0 ; j <= count ; j++)
					printf("\t");
				printf("|\n");
			}
			
			for (int i = 0 ; i <= count ; i++) printf("\t");
			printf("-%s----\n", entry->d_name); // (하위)디렉터리명 출력
			tmp = count + 1;
			dfs_print(tmp, entry->d_name); // 재귀 호출 (더 하위 디렉터리로 이동)
			
			for (int i = 0 ; i < 3 ; i++){ // 위와 동일
				for (int j = 0 ; j <= count ; j++)
					printf("\t");
				printf("|\n");
			}
		
		}
	}
	
	chdir(".."); closedir(dp); // 백트래킹(부모 디렉터리로 올라감)
}

char *strrev(char *str){ // 문자열 뒤집는 함수(string.h_strrev는 리눅스 사용 불가)
	char *p1, *p2;
	if (!str || !*str)
		return str;
	for (p1 = str, p2 = str + strlen(str)-1; p2 > p1 ; ++p1, --p2){
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	return str;
}



/*준호*/
typedef struct Queue{		//방향 front->rear
	char (*pathptr)[STR_MAX];
	int front;
	int rear;
	int capacity;
}Queue;

typedef struct Node{
	DIR *dirp;			//검색 진행상황 저장
	char name[MAX];		//현재 경로를 위해
	struct Node* next;
}Node;

typedef struct Stack{
	Node* top;
	int size;
}Stack;

/*
	#함수 설명 : 크기를 구하고 싶은 디렉토리를 argv를 통해 받아 출력한다.
	#변수 : int argc, char* argv[] - main의 argc,argv
	#리턴값 : void
*/
void fileSize_dfs(int argc, char* argv[]){
	struct stat stbuf;
	int totalSize = 0;
	int intbuf;

	Stack st={NULL,0};
	struct dirent *dp;
	char absPath[MAX]={};
	char strbuf[MAX]={};

	if(argc<=1){
		printf("Usage : %s [dirname] [filename]\n",argv[0]);
		exit(1);
   	}

	/*argv[1] 문자열 처리*/
	if(argv[1][0]=='/')							//argv[1]==절대경로
		strcpy(absPath,argv[1]);
	else{										//argv[1]==상대경로
		getcwd(absPath,MAX);
		if(strcmp(argv[1],".")){
			for(int i=0; argv[1][i]!='\0'; ){
				if(!strncmp(argv[1]+i,"../",3)){
					//tmp에서 가장 뒤에 있는 '/'찾기
					//리턴값 : 위치(max로 준다는 생각)
					//받은 리턴값에 '\0'대입
					for(int j=0; absPath[j]!='\0';j++)
						if(absPath[j]=='/')
							intbuf = j;
					absPath[intbuf] = '\0';
					i+=3;
				}
				else if(!strncmp(argv[1]+i,"./",2)){
					i+=2;
				}
				else{
					strncat(strbuf,argv[1]+i,1);
					i++;
				}
			}
			strcat(absPath,"/");
			strcat(absPath,strbuf);
		}
	}

	/*stack에 argv[1] dirp 추가 */
	push(&st,initNode(NULL,"",NULL));

	if((st.top->dirp=opendir(absPath))==NULL){
		printf("Error : fail on open directory!\n");
		exit(1);
	}
   	strcpy(st.top->name,absPath);


	/*전체 탐색 알고리즘*/
	while(st.size){										//모든 디렉토리가 스택에서 pop되면 종료
		if((dp=readdir(st.top->dirp))!=NULL){
			if(strcmp(dp->d_name,".")!=0&&strcmp(dp->d_name,"..")!=0){

			/*탐색 디렉토리 문자열 처리*/
            strcpy(absPath, st.top->name);
			if(absPath[strlen(absPath)-1]!='/')
	            strcat(absPath,"/");
            strcat(absPath,dp->d_name);

            stat(absPath,&stbuf);	//탐색 디렉토리 정보 불러오기

            if((stbuf.st_mode&0xF000)==0x8000){		//파일일 경우 : S_ISDIR(buf.st_mode)
				totalSize += stbuf.st_size;
   				printf("%-8ld  %s\n",stbuf.st_size,absPath);
             }
            else									//디렉토리일 경우 top에 추가후 위의 과정 반복
               push(&st,initNode(opendir(absPath),absPath,NULL));
			}

		}
      else		//readdir(st.top->dir)==NULL : 현재 디렉토리 안의 모든 dir 확인 시
         pop(&st);
	}

   printf("<<total : %d>>\n",totalSize);
}

/*
	#함수 설명 : 스택의 top에 노드를 push한다.
	#변수 : Stack *s - 노드를 추가할 스텍의 포인터, Node* n - 추가할 노드의 포인터
	#리턴값 : void
*/
void push(Stack *s, Node* n){
	Node* tmp;

	tmp = s->top;
	s->top = n;
    s->top->next = tmp;
	s->size++;
}

/*
	#함수 설명 : 노드를 생성한다.
	#변수 : DIR* newDirp, char* newName, Node* newNext -새로운 노드의 정보들
	#리턴값 : 새로 동적할당해 값을 대입한 Node*
*/
Node* initNode(DIR* newDirp, char* newName, Node* newNext){
	Node* new = (Node*)malloc(sizeof(Node));

	new->dirp = newDirp;
	strcpy(new->name, newName);
	new->next = newNext;

	return new;
}

/*
	#함수 설명 : 스택의 top 노드를 pop한다.
	#변수 : Stack *s - top을 내보낼 스텍의 포인터
	#리턴값 : void
*/
void pop(Stack *s){
   Node* tmp;

   tmp = s->top;
   s->top = s->top->next;

   free(tmp);
   s->size--;
}

/*
	#함수 설명 : 크기를 구하고 싶은 디렉토리를 argv를 통해 받아 출력한다.
	#변수 : int argc, char* argv[] - main의 argc,argv
	#리턴값 : void
*/
void fileSize_bfs(int argc, char* argv[]){
	struct stat stbuf;
	int totalSize = 0;
	int intbuf;
	DIR *dirp;

	Queue Q={(char(*)[STR_MAX])malloc(sizeof(char)*STR_MAX*Q_MAX),0,-1,Q_MAX};


	struct dirent *dp;
	char absPath[STR_MAX]={};
	char strbuf[STR_MAX]={};

	if(argc<=1){
		printf("Usage : %s [dirname] [filename]\n",argv[0]);
		exit(1);
   	}

	/*absPath = argv[1] 절대경로*/
	if(argv[1][0]=='/')							//argv[1]==절대경로
		strcpy(absPath,argv[1]);
	else{										//argv[1]==상대경로
		getcwd(absPath,STR_MAX);
		if(strcmp(argv[1],".")){
			for(int i=0; argv[1][i]!='\0'; ){
				if(!strncmp(argv[1]+i,"../",3)){
					/*마지막 /를 \0으로 변경 -> 부모 dir 절대경로*/
					for(int j=0; absPath[j]!='\0';j++)
						if(absPath[j]=='/')
							intbuf = j;
					absPath[intbuf] = '\0';
					i+=3;
				}
				else if(!strncmp(argv[1]+i,"./",2)){
					/* ./는 읽을필요 없음 */
					i+=2;
				}
				else{
					/*남은 문자열경로 strbuf에 하나씩 저장*/
					strncat(strbuf,argv[1]+i,1);
					i++;
				}
			}
			strcat(absPath,"/");
			strcat(absPath,strbuf);
		}
	}

	/*stack에 argv[1] dirp 추가 */
	enqueue(&Q,absPath);

	if((dirp=opendir(absPath))==NULL){
		printf("Error : fail on open directory!\n");
		exit(1);
	}


	/*전체 탐색 알고리즘:BFS*/
	/*
		#구현 : front에 있는 디렉토리를 readdir로 하위 디렉토리(dp) 읽고 stat을 통해 dir, file 판단
		#dp가 dir일 때 : 해당 dir을 queue의 rear에 올려놓음
		#dp가 file일 때 : totalsize에 해당 file사이즈 더하고 정보 출력, 다음 dp로 이동
		#dp가 NULL일 때 : 현재 front에 있는 디렉토리를 dequeque하고 다음 front 디렉토리를 읽기시작
		(하위 디렉토리를 다 읽었을 때)
	 */
	while(!isEmpty(&Q)){
		if((dp=readdir(dirp))!=NULL){		//다음 dp로 이동
			if(strcmp(dp->d_name,".")!=0&&strcmp(dp->d_name,"..")!=0){

			/*absPath = dp의 절대경로*/
            strcpy(absPath, Q.pathptr[Q.front]);
			if(absPath[strlen(absPath)-1]!='/')
	            strcat(absPath,"/");
            strcat(absPath,dp->d_name);
			/*dp의 stat 불러오기*/
            stat(absPath,&stbuf);

            if((stbuf.st_mode&0xF000)==0x8000){		//dp가 file일 경우
				totalSize += stbuf.st_size;
   				printf("%-8ld  %s\n",stbuf.st_size,absPath);
             }
            else									//dp가 dir일 경우
               	enqueue(&Q,absPath);
			}

		}
      	else{										//dp가 NULL일 경우
			/*front갱신 후 디렉토리 변경*/
    		dequeue(&Q);
			dirp=opendir(Q.pathptr[Q.front]);

		}
	}

   printf("<<total : %d>>\n",totalSize);
}

/*
	#함수 설명 : 큐의 rear에 노드를 enqueue한다, 큐가 가득찼으면 용량을 키우고 진행한다.
	#변수 : Queue *q - 경로를 집어넣을 큐, char* newpath - 큐에 들어갈 문자열의 포인터
	#리턴값 : void
*/
void enqueue(Queue *q, char* newpath){

	if(isFull(q))
	   expandCapacity(q);

	q->rear = (q->rear+1)%q->capacity;
	strcpy(q->pathptr[q->rear],newpath);

}

/*
	#함수 설명 : 큐의 front 노드를 dequeue한다.
	#변수 : Queue *q - front을 내보낼 큐의 포인터
	#리턴값 : void
*/
void dequeue(Queue *q){

	if(isEmpty(q)){
		printf("queue is already empty!!\n");
		return;
	}

	q->front = (q->front+1)%q->capacity;
}

/*
	#함수 설명 : 큐가 비어있으면 1 리턴
	#변수 : Queue *q - 확인할 큐
	#리턴값 : void
*/
_Bool isEmpty(Queue *q){

	if(q->front == (q->rear+1)%q->capacity)
		return 1;
	else
		return 0;
}

/*
	#함수 설명 : 큐가 가득찼으면 1리턴 (rear 다음다음이 front일 떄 기준 Full)
	#변수 : Queue *q - 확인할 큐
	#리턴값 : void
*/
_Bool isFull(Queue * q){
	if((q->rear+2)%q->capacity==q->front)
		return 1;
	else
		return 0;
}

/*
	#함수 설명 : 큐의 용량을 증가시킨다. 함수 실행후 front=0이 되도록 설정
	#변수 : Queue *q - 용량을 증가시킬 큐
	#리턴값 : void
*/
void expandCapacity(Queue* q){
	char (*tmp)[STR_MAX] = (char(*)[STR_MAX])malloc(sizeof(char)*STR_MAX*q->capacity*2);

	for(int i=0; i<q->capacity; i++){
		strcpy(tmp[i],q->pathptr[(q->front+i)%(q->capacity)]);
	}

	free(q);

	q->pathptr = tmp;
	q->rear = q->rear - q->front;
	q->front = 0;
	q->capacity *= 2;
}

/*지우*/
/*NODE && QUE*/

//Node
typedef struct NODE{
	DIR* dp;
	char name[MAX];
	struct NODE * next;
}NODE;

//Queue
typedef struct Queue{
	struct NODE* front;
	struct NODE* rear;
	int qSize;
}QUE;

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
	while(!isEmpty(q.qSize)){
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


/* parents: toFind가 ..(부모 디렉터리)인 경우 */

char* parents(char* workDir){
	int i=0;
	char str[MAX], tmp[MAX][20];
	char* ptr = strtok(workDir, "/");
	for (i=0; ptr!=NULL; i++){
		strcpy(tmp[i], ptr);
		ptr = strtok(NULL,"/");
	}
	for (int k=0; k<i-1; k++){
		strcat(str,"/");
		strcat(str, tmp[k]);
	}
	workDir = str;
	return workDir;
}



/*
	printDir:
		1. "."또는 ".."을 입력받은 경우, 탐색하지 않고 바로 경로를 출력한다.
		2. "."부터 시작하는 경로를 입력받은 경우, 현재 디렉토리부터 탐색 후 경로를 출력한다.
		3. 그 외의 경우, 루트부터 탐색 후 경로를 출력한다.
 */

void printDir(char* argv, char* toFind, int BD){
	
	//Prepare
	char workDir[MAX];
	struct passwd *pwd;
	errno = 0;
	if((pwd = getpwuid(getuid())) == NULL) { 
		if(errno == 0 || errno == ENOENT || errno == ESRCH || errno == EBADF || errno == EPERM) { 
			fprintf(stderr,"미등록된 사용자입니다.\n"); 
		} 
		else { 
			fprintf(stderr,"error: %s\n", strerror(errno)); 
		}
		return;	
	}

	//Branching
	if ((strcmp(".", argv) == 0)&&(strlen(argv)==2)){ //탐색x (.)
		getcwd(workDir,MAX);
		printf("[ Path: %s ]\n", workDir);
		return;
	}
	else if (strcmp("..", argv) == 0){ //탐색x (..)
		getcwd(workDir,MAX);
		printf("[ Path: %s ]\n", parents(workDir));
		return;
	}
	else if (strncmp("./", argv, 2) == 0){ //탐색o (.)
		getcwd(workDir, MAX);
	}
	else{ //탐색o (/,name)
		strcpy(workDir,pwd->pw_dir);
	}
	
	//Searching
	if (BD==0){
		printf("BFS Search\n");
		dirBFS(toFind, workDir);
	}
	else{
		printf("DFS Search\n");
		dirDFS(toFind,workDir);
	}

}
