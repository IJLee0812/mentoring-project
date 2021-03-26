
#include "head.h"
char wd[MAX];
_Bool found = 0;


/* < 기능1: path > */
/* 
   isEmpty 
	# 사용: enQ, deQ, BFS(while시작)
 */
_Bool isEmpty(int qSize){
	return (qSize==0); //qSize를 통해 q가 비어있는지 확인, 비어있으면 리턴 true
}



/* 
   enQue_for_Path
	# 사용: BFS(firstNODE, S_ISDIR)
 */
void enQue_for_Path(QUE* q, struct NODE* newnode){
	
	if (isEmpty(q->qSize)){ //q가 비어있는 경우
		q->front = q->rear = newnode; //q의 front와 rear를 newnode로 일치
	}
	else { //q가 비어있지 않은 경우
		q->rear->next = newnode; //q의 끝에 newnode를 배치
		q->rear = q->rear->next; //q의 끝 설정
	}
	
	q->qSize++; //qSize Up
}



/* 
   deQue_for_Path 
    # 사용: BFS(while끝)
 */
void deQue_for_Path(QUE* q){

	if (isEmpty(q->qSize)){ //q가 비어있는 경우(deQ 불가능)
		fprintf(stderr,"delete error: QisEmpty\n");
		return;
	}

	//q의 front를 deQ
	struct NODE *tmp = q->front; 
	q->front = q->front->next;
	free(tmp);

	q->qSize--; //qSize Down
}



/* 
   create_NODE 
     # 사용: BFS(firstNODE, S_ISDIR)
 */
struct NODE* create_NODE(DIR* dp, char* Nname){
	
	struct NODE* newnode = (NODE*)malloc(sizeof(NODE));

	//init newnode
	newnode->dp = dp;
	strcpy(newnode->Nname, Nname);
	
	return newnode;
}



/* 
   Bfs_for_Path
	# 인자: toFind - 찾을 파일 또는 디렉터리의 이름 / workDir - 탐색 시작 경로
	# 결과: 찾은 경우, 그 경로를 출력함 / 못 찾은 경우, 아무 것도 출력하지 않음
	# 방식: 큐를 활용해 넓이 우선 탐색
	# 사용: before_Search(끝) 
 */
void Bfs_for_Path(char* toFind, char* workDir){

	//prepare
	struct dirent *dir = NULL;
	QUE q = {NULL, NULL, 0};
	struct NODE* firstNODE = create_NODE(NULL, workDir); //탐색 시작 경로를 firstNODE로 만들어 enQ
	enQue_for_Path(&q, firstNODE);
	if ((q.front->dp = opendir(workDir))==NULL){
		perror("Error Occurred!\n");
		exit(1);
	}


	//bfs
	while(!isEmpty(q.qSize)){ //q가 비어있지 않을 동안
		while((dir = readdir(q.front->dp))!=NULL){ //q의 front->dp가 끝에 도달할 때까지 읽음
			struct stat statbuf;
		
			//check
			if (strcmp(dir->d_name, toFind)==0){ //원하는 파일 또는 디렉터리를 찾았을 경우
				found++;
				printf("PATH: %s/%s\n", q.front->Nname, dir->d_name); //출력 후 break
				break; //동일한 이름을 가진 파일 또는 디렉터리를 모두 찾기 위해서 return이 아닌 break
			}

			//pass
			if (strncmp(dir->d_name,".",1)==0){ //.과 ..그리고 .으로 시작하는 파일(숨김파일 등)은 모두 건너뜀
				continue;
			}

			//enque
			char tmp[MAX]; //현재 위치를 tmp에 경로로 나타냄
			strcpy(tmp, q.front->Nname);
			strcat(tmp,"/");
			strcat(tmp, dir->d_name);
			if (lstat(tmp, &statbuf)<0){ //tmp(현재 위치)를 statbuf에 넣음
				perror("Stat Error\n");
				exit(1);
			}
			
			if (S_ISDIR(statbuf.st_mode)){ //dir이면 enQ(추후에 탐색)
				struct NODE* n = create_NODE(opendir(tmp),tmp);
				enQue_for_Path(&q,n);
			}
		
		}
		closedir(q.front->dp);
		deQue_for_Path(&q); //q의 front를 deQ (새로운 q의 front를 탐색하기 위해)
	}
}



/*
	Dfs_for_Path
	# 인자: toFind - 찾을 파일 또는 디렉터리의 이름 / workDir - 탐색 시작 경로
	# 결과: 찾은 경우, 그 경로를 출력함 / 못 찾은 경우, 아무 것도 출력하지 않음
	# 방식: 재귀를 활용해 깊이 우선 탐색
	# 사용: before_Search(끝)
 */
void Dfs_for_Path(char* toFind, char* workDir) {

	//prepare
	DIR* dp = NULL;
	struct dirent *dir = NULL;
	struct stat statbuf;
	if ((dp = opendir(workDir))==NULL){
		perror("File Open Error\n");
		exit(1);
	}


	//dfs
	while((dir=readdir(dp))!=NULL){ //dp가 끝에 도달할 때까지 읽음
		
		//check
		if (strcmp(dir->d_name, toFind) == 0){ //원하는 파일 또는 디렉터리를 찾았을 경우
			printf("PATH: %s/%s\n", workDir, dir->d_name); //출력 후 return
			found++;
			closedir(dp);
			return; //재귀를 활용했기 때문에 break가 아닌 return
		}

		//pass
		if ((strncmp(dir->d_name,".",1)==0)){ //.과 ..그리고 .으로 시작하는 파일(숨김파일 등)은 모두 건너뜀
			continue;
		}

		//recurse
		char tmp[MAX]; //현재 위치를 tmp에 경로로 나타냄
		strcpy(tmp,workDir);
		strcat(tmp,"/");
		strcat(tmp,dir->d_name);
		if (lstat(tmp, &statbuf)<0){ //tmp(현재 위치)를 statbuf에 넣음
			perror("Stat Error\n");
			exit(1);
		}

		if (S_ISDIR(statbuf.st_mode)){ //dir이면 recurse(탐색)
			Dfs_for_Path(toFind, tmp);
		}
	}

	closedir(dp);
}



/* 
   extract_Filename 
	# 인자: Path - argv[2]로 입력받은 문자열 (찾을 파일/디렉토리의 경로 또는 이름)
	# 목적: toFind에 argv[2]에서 추출한 파일/디렉토리의 이름을 넣음
	# 사용: main 
 */
char* extract_Filename(char* Path){

	//No slashes
	if (strstr(Path,"/")==NULL) { // argv[2]에서 파일 또는 디렉터리의 이름을 입력받은 경우
		return Path; //그대로 리턴
	}

	//Slashes
	char* temp = (char*)malloc(sizeof(char)*MAX); //파일 또는 디렉터리의 경로에서 이름만을 추출하기 위해
	char* ptr = strtok(Path,"/"); // "/" 기준으로 token화
	while (ptr!=NULL) {
		strcpy(temp, ptr); //(결국엔) 마지막 ptr을 temp에 copy
		ptr = strtok(NULL,"/");
	}
	return temp;

}



/* 
   set_Parentdir_Path
	# 목적: toFind가 ".."(부모 디렉터리)인 경우, 경로 설정을 위해
	# 사용: before_Search("..") 
 */
char* set_Parentdir_Path(char* workDir){
	int i=0;
	char str[MAX], tmp[MAX][20];
	char* ptr = strtok(workDir, "/"); //getcwd로 얻은 경로를 "/"기준으로 token화
	for (i=0; ptr!=NULL; i++){
		strcpy(tmp[i], ptr);
		ptr = strtok(NULL,"/");
	}
	for (int k=0; k<i-1; k++){ //마지막 token을 제외해 경로를 재생성
		strcat(str,"/");
		strcat(str, tmp[k]);
	}
	workDir = str;
	return workDir;
}




/* 
   before_Search 
	# 인자: argv - 찾을 파일/디렉토리의 경로 또는 이름 / toFind - 찾을 파일/디렉토리의 이름 / BD - BFS/DFS
	# 사용: main
 */
void before_Search(char* argv, char* toFind, int BD){
	
	//Prepare
	char workDir[MAX];
	struct passwd *pwd;
	errno = 0;
	if((pwd = getpwuid(getuid())) == NULL) { //사용자 정보를 불러옴 - 사용자 계정명 등 (모든 컴퓨터에서 작동이 가능하도록)
		if(errno == 0 || errno == ENOENT || errno == ESRCH || errno == EBADF || errno == EPERM) { 
			fprintf(stderr,"미등록된 사용자입니다.\n"); 
		} 
		else { 
			fprintf(stderr,"error: %s\n", strerror(errno)); 
		}
		exit(1);
	}

	//Branch
	if ((strcmp(".", argv) == 0)||(strcmp("./", argv) == 0)){ //탐색x (.)
		getcwd(workDir,MAX); //현위치 (작업 디렉토리)를 불러와 출력
		printf("Path: %s\n", workDir); 
		return;
	}
	else if (strcmp("..", argv) == 0){ //탐색x (..)
		getcwd(workDir,MAX); //현위치를 기준으로 부모 디렉토리의 경로를 설정해 출력
		printf("Path: %s\n", set_Parentdir_Path(workDir));
		return;
	}
	else if (strncmp("./", argv, 2) == 0){ //탐색o (~)
		getcwd(workDir, MAX); //현재 디렉토리를 workDir에 넣음
	}
	else{ //탐색o (/)
		strcpy(workDir,pwd->pw_dir); //"/home/계정명"을 workDir에 넣음
	}
	
	//Search
	printf("탐색할 디렉터리 또는 파일 이름(경로) : %s\n", argv);
	if (BD==0){
		Bfs_for_Path(toFind, workDir);
	}
	else{
		Dfs_for_Path(toFind,workDir);
	}
 
	//전역변수 cnt : 재귀/반복의 횟수
	if (!found){
		printf("해당 디렉터리 또는 파일이 없습니다.\n");
	}	

}



/* < 기능2: size > */
/*
	#함수 설명 : 경로를 받아서 절대경로로 바꿔준다.
	#변수 : char* path - 절대/상대 경로
	#리턴값 : path의 절대경로
   */
char* absolute(char* path){
	char *absPath = (char*)malloc(sizeof(char)*MAX);
	char strbuf[MAX]={};
	int intbuf;

	/*path 문자열 처리*/
	if(path[0]=='/')								//path==절대경로
		strcpy(absPath,path);
	else{											//path==상대경로
		getcwd(absPath,MAX);
		if(strcmp(path,".")){
			for(int i=0; path[i]!='\0'; ){
				if(!strncmp(path+i,"../",3)){
					/*가장 뒤에 있는 '/'찾아서'\0'대입*/
					for(int j=0; absPath[j]!='\0';j++)
						if(absPath[j]=='/')
							intbuf = j;
					absPath[intbuf] = '\0';
					i+=3;
				}
				else if(!strncmp(path+i,"./",2)){
					i+=2;
				}
				else{
					strncat(strbuf,path+i,1);
					i++;
				}
			}
			if (strcmp(strbuf,"")){
				strcat(absPath,"/");
				strcat(absPath,strbuf);
			}
		}
	}

	//printf("absPath: %s\n", absPath);
	return absPath;
}


/*
	#함수 설명 : 처음 BFS/DFS 선택지에 따라 fileSize함수를 실행하고 결과를 출력한다.
	#변수 : char* absPath - 크기를 구할 디렉토리의 절대 경로
			int BD - BFS/DFS 선택지 (BFS:0, DFS:1)
	#리턴값 : void
   */
void bfs_or_dfs (char* absPath,int BD){

	int totalSize = 0;
	
	if(BD==0)
		totalSize = Bfs_for_Size(absPath);
	else if(BD==1)
		totalSize = Dfs_for_Size(absPath);
	else
		perror("Error : unexpected value of valiable \"BD\"!!\n");

	printf("Total : %d\n",totalSize);
}

/*
void printSt(Stack* s){
	NODE* horse = s->top;
	while(horse!=NULL){
		printf("horse->Nname: %s\n", horse->Nname);
		printf("horse->size: %d\n", s->size);
		horse=horse->next;
	}
}
*/

/*
	#함수 설명 : 해당 디렉토리 내의 모든 파일크기를 DFS로 탐색하여 총합을 리턴
	#변수 : char* absPath - 크기를 구할 디렉토리의 절대 경로
	#리턴값 : 해당 디렉토리 내의 모든 파일크기 합
*/
int Dfs_for_Size(char* absPath){
	struct stat stbuf;
	int totalSize = 0;

	Stack st={NULL,0};
	struct dirent *dir;
	DIR* openable;

	/*stack에 시작 디렉토리 노드 추가*/
	push(&st,initNODE(NULL,"",NULL));

	if((st.top->dp=opendir(absPath))==NULL){
		printf("Error : fail on open directory!\n");
		exit(1);
	}
   	strcpy(st.top->Nname,absPath);


	/*전체 탐색 알고리즘:DFS*/
	/*
		#구현 : top에 있는 디렉토리를 readdir로 하위 디렉토리(dir) 읽고 stat을 통해 디렉토리, 파일 판단
		#dp가 dir일 때 : 해당 dir을 stack의 top에 올려서 읽기 시작
		#dp가 file일 때 : totalsize에 해당 file사이즈 더하고 정보 출력, 다음 dir로 이동
		#dp가 NULL일 때 : 현재 top에 있는 디렉토리를 pop하고 이전 top에 있던 디렉토리를 이어서 읽기 시작
		(하위 디렉토리를 다 읽었을 때)
	 */
	while(st.size){										//모든 디렉토리가 스택에서 pop되면 종료
		//printf("st.top->Nname: %s, st.size: %d\n", st.top->Nname, st.size);
		if((dir=readdir(st.top->dp))!=NULL){
			if(strcmp(dir->d_name,".")!=0&&strcmp(dir->d_name,"..")!=0){

			/*탐색 디렉토리 문자열 처리*/
            strcpy(absPath, st.top->Nname);
			if(absPath[strlen(absPath)-1]!='/')
	            strcat(absPath,"/");
            strcat(absPath,dir->d_name);

            stat(absPath,&stbuf);	//탐색 디렉토리 정보 불러오기

            if((stbuf.st_mode&0xF000)==0x8000){		//dir이 파일일 경우
//				printf("File\n");
				totalSize += stbuf.st_size;
             }
            else{									//dir이 디렉토리일 경우
//				printf("Dir\n");
				if((openable = opendir(absPath))!=NULL)
            		push(&st,initNODE(openable,absPath,NULL));
			}
			}

		}
		else											//dir이 NULL일 경우
        	pop(&st); /*top을 이전에 읽던 디렉토리로 변경*/
	}

	return totalSize;
}

/*
	#함수 설명 : 스택의 top에 노드를 push한다.
	#변수 : Stack *s - 노드를 추가할 스텍의 포인터, NODE* n - 추가할 노드의 포인터
	#리턴값 : void
*/
void push(Stack *s, NODE* n){
	NODE* tmp;

	tmp = s->top;
	s->top = n;
    s->top->next = tmp;
	s->size++;
	//printf("Push! %s\n", s->top->Nname);
}

/*
	#함수 설명 : 노드를 생성한다.
	#변수 : DIR* newDirp, char* newName, NODE* newNext -새로운 노드의 정보들
	#리턴값 : 해당 정보를 삽입한 새로운 노드의 포인터
*/
NODE* initNODE(DIR* newDp, char* newName, NODE* newNext){
	NODE* new = (NODE*)malloc(sizeof(NODE));

	new->dp = newDp;
	strcpy(new->Nname, newName);
	new->next = newNext;

	return new;
}

/*
	#함수 설명 : 스택의 top 노드를 pop한다.
	#변수 : Stack *s - top을 내보낼 스텍의 포인터
	#리턴값 : void
*/
void pop(Stack *s){
   NODE* tmp;

   tmp = s->top;
   s->top = s->top->next;
	//printf("Pop! %s\n\n", tmp->Nname);
   free(tmp);
   s->size--;
}

/*
	#함수 설명 : 해당 디렉토리 내의 모든 파일크기를 BFS로 탐색하여 총합을 리턴
	#변수 : char* absPath - 크기를 구할 디렉토리의 절대 경로
	#리턴값 : 해당 디렉토리 내의 모든 파일크기 합
	
*/
int Bfs_for_Size(char* absPath){
	struct stat stbuf;
	int totalSize = 0;
	DIR *dirp;

	Queue Q={(char(*)[STR_MAX])malloc(sizeof(char)*STR_MAX*MAX),0,-1,MAX};

	struct dirent *dir;

	/*queue에 시작 디렉토리 경로 추가*/
	enqueue_for_Size(&Q,absPath);

	if((dirp=opendir(absPath))==NULL){
		printf("Error : fail on open directory!\n");
		exit(1);
	}


	/*전체 탐색 알고리즘:BFS*/
	/*
		#구현 : front에 있는 디렉토리를 readdir로 하위 디렉토리(dir) 읽고 stat을 통해 디렉토리, 파일 판단
		#dp가 dir일 때 : 해당 dir을 queue의 rear에 올려놓고 다음 dir로 이동
		#dp가 file일 때 : totalsize에 해당 file사이즈 더하고 정보 출력, 다음 dir로 이동
		#dp가 NULL일 때 : 현재 front에 있는 디렉토리를 dequeque하고 다음 front 디렉토리를 읽기 시작
		(하위 디렉토리를 다 읽었을 때)
	 */
	while(!isEmpty(Q.rear-Q.front+1)){
		if(dirp==NULL){
			dequeue_for_Size(&Q);
			dirp=opendir(Q.pathptr[Q.front]);
			continue;
		}
		else if((dir=readdir(dirp))!=NULL){		//다음 dir로 이동
			if(strcmp(dir->d_name,".")!=0&&strcmp(dir->d_name,"..")!=0){

			/*absPath = dir의 절대경로*/
            strcpy(absPath, Q.pathptr[Q.front]);
			if(absPath[strlen(absPath)-1]!='/')
	            strcat(absPath,"/");
            strcat(absPath,dir->d_name);
			/*dir의 stat 불러오기*/
            stat(absPath,&stbuf);

            if((stbuf.st_mode&0xF000)==0x8000){		//dir이 파일일 경우
				totalSize += stbuf.st_size;
             }
            else									//dir이 디렉토리일 경우
               	enqueue_for_Size(&Q,absPath);
			}

		}
      	else{										//dp가 NULL일 경우
			/*front갱신 후 디렉토리 변경*/
			dequeue_for_Size(&Q);
			dirp=opendir(Q.pathptr[Q.front]);
		}
	}

	return totalSize;
}

/*
	#함수 설명 : 큐의 rear에 노드를 enqueue_for_Size한다, 큐가 가득찼으면 용량을 키운다.
	#변수 : Queue *q - 경로를 집어넣을 큐, 
			char* newpath - 큐에 들어갈 문자열의 포인터
	#리턴값 : void
*/
void enqueue_for_Size(Queue *q, char* newpath){

	if(isFull(q))
	   expand_Capacity(q);

	q->rear = (q->rear+1)%q->capacity;
	strcpy(q->pathptr[q->rear],newpath);

}

/*
	#함수 설명 : 큐의 front 노드를 dequeue_for_Size한다.
	#변수 : Queue *q - front를 내보낼 큐의 포인터
	#리턴값 : void
*/
void dequeue_for_Size(Queue *q){

	if(isEmpty(q->rear-q->front+1)){
		printf("queue is already empty!!\n");
		return;
	}

	q->front = (q->front+1)%q->capacity;
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
	#함수 설명 : 큐의 용량을 증가시킨다. (default : 함수 실행 후 front=0)
	#변수 : Queue *q - 용량을 증가시킬 큐
	#리턴값 : void
*/
void expand_Capacity(Queue* q){
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



/* < 기능3: Tree > */
/* 
	#함수 설명 : 탐색할 디렉터리 이름(또는 경로), 탐색 시작 절대경로(프로그램 위치)를 출력하기 위한 함수이다.
	#변수 : char *name - 찾을 디렉터리명(경로)
	#리턴값 : void
*/
void init(char *name){ // 탐색할 디렉터리 이름(또는 경로), 탐색 시작 절대경로(프로그램 위치)출력
	printf("탐색할 디렉터리 이름(경로) : %s\n", name);
	getcwd(wd, BUFSIZ); // 프로그램 위치를 불러오기 위해 사용, wd에 저장
	return;
}

/* 
	#함수 설명 : BFS/DFS 둘 중 하나를 선택하게 하는 기능을 위한 함수이다.
	#변수 : char *argv - 찾을 디렉터리명(경로)
	#리턴값 : void
*/
void selectmod(char *argv, int mod){
	
	if (mod == 0){
		if (strncmp("/", argv, 1) == 0) // 절대 경로로 입력했을 경우
			Bfs_for_SearchTree(argv, "..");
		else if (strncmp("..", argv, 2) == 0) // ..로 시작할 경우
			if ((strcmp("..", argv) == 0) || (strcmp("../", argv) == 0)) // 부모 디렉터리
				Dfs_for_PrintTree(0, "..");
			else if (strncmp("../.", argv, 4) == 0) // 상위 디렉터리
				Dfs_for_PrintTree(0, argv);
			else
				Bfs_for_SearchTree(argv, ".."); // 프로그램 위치 디렉터리와 같은 깊이의 디렉터리
		else if (strncmp(".", argv, 1) == 0) // .로 시작할 경우
			if ((strcmp(".", argv) == 0) || (strcmp("./", argv) == 0)) // 현재 디렉터리
				Dfs_for_PrintTree(0, ".");
			else
				Bfs_for_SearchTree(argv, "."); // 하위 디렉터리
		else // dirname 또는 상대경로로 입력했을 경우
			Bfs_for_SearchTree(argv, ".");
	}
	
	else if (mod == 1){
		if (strncmp("/", argv, 1) == 0) // 절대 경로로 입력했을 경우
			Dfs_for_SearchTree(argv, "..");
		else if (strncmp("..", argv, 2) == 0) // ..로 시작할 경우
			if ((strcmp("..", argv) == 0) || (strcmp("../", argv) == 0)) // 부모 디렉터리
				Dfs_for_PrintTree(0, "..");
			else if (strncmp("../.", argv, 4) == 0) // 상위 디렉터리
				Dfs_for_PrintTree(0, argv);
			else
				Dfs_for_SearchTree(argv, ".."); // 프로그램 위치 디렉터리와 같은 깊이의 디렉터리
		else if (strncmp(".", argv, 1) == 0) // .로 시작할 경우
			if ((strcmp(".", argv) == 0) || (strcmp("./", argv) == 0)) // 현재 디렉터리
				Dfs_for_PrintTree(0, ".");
			else
				Dfs_for_SearchTree(argv, "."); // 하위 디렉터리
		else // dirname 또는 상대경로로 입력했을 경우
			Dfs_for_SearchTree(argv, ".");
	}
}

/* 
	#함수 설명 : BFS 알고리즘으로 찾고자 하는 디렉터리를 탐색하는 함수이다. 
	#변수 : char *name, char *wd - 찾을 디렉터리명(경로), 탐색하기 시작할 디렉터리명(경로)
	#리턴값 : void
*/
void Bfs_for_SearchTree(char *name, char *wd){ 
	struct dirent *entry; struct stat buf; DIR *dp;
	NODE queue[MAX]; int front, rear; front = rear = -1; // 큐 생성
	
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
				
				if (strncmp(strrev(name), strrev(entry->d_name), strlen(entry->d_name)) == 0){ // <입력한 이름(경로) == 탐색한 디렉터리명>인 경우					
					strrev(name); strrev(entry->d_name);   
					printf("\n%s----\n", entry->d_name); // 입력한(경로의) 디렉터리명 출력
					Dfs_for_PrintTree(0, entry->d_name); // Dfs_for_PrintTree함수 호출
					closedir(dp); return; // 반복문 탈출, Bfs_for_SearchTree함수 종료
				}
				
				else{
				strrev(name); strrev(entry->d_name);
				}
				
				getcwd(path, BUFSIZ);
				strcat(path, "/");
				strcat(path, entry->d_name);
				
				if (havedir(path)){ // 탐색 중인 디렉터리가 하위 디렉터리를 보유한 경우
					rear++; // 저장할 공간 확보
					strcpy(queue[rear].Nname, path);// 대상 디렉터리 경로 저장
				}
				
				chdir("..");
			}
		}
		// 같은 깊이의 모든 노드들의 탐색이 끝난 경우
		front++;
		closedir(dp); chdir(queue[front].Nname); // 선입선출, 비었으면 함수 종료
		
		if ((dp = opendir(queue[front].Nname)) == NULL){ // Dequeue한 디렉터리로 이동
			printf("Dequeue 오류 발생. 프로그램 종료\n");
			exit(1);
		}
	}
	return;
}

/* 
	#함수 설명 : 인자로 받은 경로, 즉 디렉터리가 하위 디렉터리를 가지는지 검사하는 함수이다. 
	#변수 : char *path - 검사할 디렉터리 경로
	#리턴값 : 0 (하위 디렉터리가 없을 경우), 1 (하위 디렉터리가 있을 경우)
*/
int havedir(char *path){ // 하위 디렉터리 보유 여부 체크 함수
	struct dirent *entry; struct stat buf; DIR *dp;
	
	chdir(path); dp = opendir(path);
	
	while ((entry = readdir(dp)) != NULL){
		
		lstat(entry->d_name, &buf);
		
		if (S_ISREG(buf.st_mode)) continue; 
		
		else if (S_ISDIR(buf.st_mode)){ // 하위 디렉터리가 있을 경우
			
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;
			closedir(dp);
			return 1; // 1 반환, if문 True
		
		}
	}
	
	closedir(dp);
	return 0; // 0 반환, if문 False
}

/* 
	#함수 설명 : DFS 알고리즘으로 찾고자 하는 디렉터리를 탐색하는 함수이다.
	#변수 : char *name, char *wd - 찾을 디렉터리명(경로), 탐색하기 시작할 디렉터리명(경로)
	#리턴값 : void 
*/
void Dfs_for_SearchTree(char *name, char *wd){ // 입력한 디렉터리를 찾는 함수, dfs 알고리즘
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
				Dfs_for_PrintTree(0, entry->d_name); // Dfs_for_PrintTree함수 호출
				chdir(".."); closedir(dp); return; // Dfs_for_SearchTree 함수 종료
			}
			
			else{
				strrev(name); strrev(entry->d_name); // 비교 위해 뒤집은 문자열 원상태로 복구
			}
			
			Dfs_for_SearchTree(name, entry->d_name); // 없을 경우, 재귀 호출(더 하위 디렉터리로 이동)
		}
	} 
	
	chdir(".."); closedir(dp); // 백트래킹(부모 디렉터리로 올라감)
}

/* 
	#함수 설명 : DFS 알고리즘으로 찾은 디렉터리의 하위 파일과 디렉터리들을 트리 구조로 출력하는 함수이다. (BFS 방식은 트리 구조를 출력하는 것이 까다롭기 때문에 DFS 방식으로 만듬)
	#변수 : int tmp, char *wd - depth 구분하기 위한 정수, 트리 구조를 출력할 디렉터리
	#리턴값 : void
*/
void Dfs_for_PrintTree(int tmp, char *wd){ // 디렉터리를 트리구조로 출력하는 함수, dfs 알고리즘
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
			Dfs_for_PrintTree(tmp, entry->d_name); // 재귀 호출 (더 하위 디렉터리로 이동)
			
			for (int i = 0 ; i < 3 ; i++){ // 위와 동일
				for (int j = 0 ; j <= count ; j++)
					printf("\t");
				printf("|\n");
			}
		
		}
	}
	
	chdir(".."); closedir(dp); // 백트래킹(부모 디렉터리로 올라감)
}

char *strrev(char *str){ // (문자열 뒤집는 함수(string.h_strrev는 리눅스 사용 불가))
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
