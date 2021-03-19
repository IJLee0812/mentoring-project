#include "head.h"

int main(int argc, char *argv[]){
	
	int BD; //BFS or DFS
	char* toFind; //File or Dir Name to Find
	char* absDir;
	
	if(argc != 3){ //Need more data
		fprintf(stderr, "Usage : Program [-Dir] [-File]\n");
		exit(1);
	}

	printf("Search Option(BFS:0, DFS:1): ");
	scanf("%d",&BD);

	if((BD==0)||(BD==1)){
		//준호
		absDir = absolute(argv[1]);
		dfs_or_bfs(absDir,BD);		

		//지우
		char tmp_Path[MAX];
		strcpy(tmp_Path,argv[2]);
		char* toFind = extract_Filename(tmp_Path); //입력받은 argv[2]에서 파일 또는 디렉터리 이름만을 추출해 toFind에 넣음
		before_Search(argv[2], toFind, BD);

	}
	else
		printf("Select Error(BFS:0, DFS:1)\n");
	
	//익준
	init(argv[1]); 
	selectmod(argv[1]);

}
