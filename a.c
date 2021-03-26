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
            		else						//dir이 디렉토리일 경우
               			enqueue_for_Size(&Q,absPath);
			}
		}
      		else{							//dp가 NULL일 경우
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
