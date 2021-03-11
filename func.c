#include "head.h"

/*
	#함수 설명 : 크기를 구하고 싶은 디렉토리를 argv를 통해 받아 출력한다.
	#변수 : int argc, char* argv[] - main의 argc,argv
	#리턴값 : void
*/
void fileSize(int argc, char* argv[]){
	struct stat stbuf;
	int totalSize = 0;	

	Stack st={NULL,0};
	struct dirent *dp;
	char tmp[MAX]={};

	if(argc<=1){
		printf("Usage : %s [dirname] [filename]\n",argv[0]);
		exit(1);
   	}

	/*argv[1] 문자열 처리*/
	if(argv[1][0]=='.'){			//argv[1]==상대경로
		getcwd(tmp,MAX);			
		if(strcmp(argv[1],".")){	//현재 디렉터리가 아닐 때
			strcat(tmp,"/");
			strcat(tmp,argv[1]+1);	//'.'제외
		}
	}
	else							//argv[1]==절대경로
		strcpy(tmp,argv[1]);

	/*stack에 argv[1] dirp 추가 */
	push(&st,addNode(NULL,"",NULL));

	if((st.top->dirp=opendir(tmp))==NULL){
		printf("Error : fail on open directory!\n");	
		exit(1);
	}
   	strcpy(st.top->name,tmp);
	

	/*전체 탐색 알고리즘*/
	while(st.size){										//모든 디렉토리가 스택에서 pop되면 종료
		if((dp=readdir(st.top->dirp))!=NULL){
			if(strcmp(dp->d_name,".")!=0&&strcmp(dp->d_name,"..")!=0){

			/*탐색 디렉토리 문자열 처리*/
            strcpy(tmp, st.top->name);
            strcat(tmp,"/");
            strcat(tmp,dp->d_name);
 
            stat(tmp,&stbuf);	//탐색 디렉토리 정보 불러오기

            if((stbuf.st_mode&0xF000)==0x8000){		//파일일 경우 : S_ISDIR(buf.st_mode)
				totalSize += stbuf.st_size;
   				printf("%-8ld  %s\n",stbuf.st_size,tmp);
             }
            else									//디렉토리일 경우 top에 추가후 위의 과정 반복
               push(&st,addNode(opendir(tmp),tmp,NULL));
            
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
Node* addNode(DIR* newDirp, char* newName, Node* newNext){
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
