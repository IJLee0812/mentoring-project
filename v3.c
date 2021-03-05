#include<stdio.h>    //check by : du -a
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>	

#define MAX 1000

typedef struct Node{
	DIR *dirp;			//검색 진행상황 저장
	char name[MAX];		//현재 경로를 위해
	struct Node* next;
}Node;

typedef struct Stack{
	Node* top;
	int size;
}Stack;

void fileSize(int argc, char* argv[]);
void push(Stack* , Node*);
Node* initNode(DIR*, char*, Node*);
void pop(Stack*);

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
