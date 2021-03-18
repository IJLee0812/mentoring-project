#include<stdio.h>    //check by : du -a
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>	

#define STR_MAX 1000
#define Q_MAX 100

typedef struct Queue{		//방향 front->rear
	char (*pathptr)[STR_MAX];
	int front;
	int rear;
	int capacity;
}Queue;

void fileSize(int argc, char* argv[]);
void enqueue(Queue* , char*);
void dequeue(Queue*);
_Bool isEmpty(Queue*);
_Bool isFull(Queue*);
void expandCapacity(Queue*);

int main(int argc, char* argv[]){
	fileSize(argc,argv);
}

/*
	#함수 설명 : 크기를 구하고 싶은 디렉토리를 argv를 통해 받아 출력한다.
	#변수 : int argc, char* argv[] - main의 argc,argv
	#리턴값 : void
*/
void fileSize(int argc, char* argv[]){
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
            else{									//dp가 dir일 경우
               	enqueue(&Q,absPath);
			}
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
