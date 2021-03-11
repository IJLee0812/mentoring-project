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

	/*absPath = argv[1] 절대경로*/					
	if(argv[1][0]=='/')							//argv[1]==절대경로
		strcpy(absPath,argv[1]);
	else{										//argv[1]==상대경로
		getcwd(absPath,MAX);
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
	push(&st,initNode(NULL,"",NULL));

	if((st.top->dirp=opendir(absPath))==NULL){
		printf("Error : fail on open directory!\n");	
		exit(1);
	}
   	strcpy(st.top->name,absPath);
 

	/*전체 탐색 알고리즘:DFS*/
	/*
		#구현 : top에 있는 디렉토리를 readdir로 하위 디렉토리(dp) 읽고 stat을 통해 dir, file 판단
		#dp가 dir일 때 : 해당 dir을 stack의 top에 올려서 읽기 시작 
		#dp가 file일 때 : totalsize에 해당 file사이즈 더하고 정보 출력, 다음 dp로 이동
		#dp가 NULL일 때 : 현재 top에 있는 디렉토리를 pop해서 이전에 읽던 디렉토리로 돌아감
		(하위 디렉토리를 다 읽었을 때)
	 */
	while(st.size){	
		if((dp=readdir(st.top->dirp))!=NULL){		//다음 dp로 이동
			if(strcmp(dp->d_name,".")!=0&&strcmp(dp->d_name,"..")!=0){

			/*absPath = dp의 절대경로*/
            strcpy(absPath, st.top->name);
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
               push(&st,initNode(opendir(absPath),absPath,NULL)); 
			}

		}
      	else										//dp가 NULL일 경우
			/*top이 이전에 읽던 디렉토리로 변경*/
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
