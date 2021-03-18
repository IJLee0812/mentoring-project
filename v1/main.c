#include "head.h"

int main(int argc, char *argv[]){
	
	int BD;
	char* toFind;

	if(argc != 3){
		fprintf(stderr, "Usage : Program [-Dir] [-File]\n");
		exit(1);
	}

	printf("Search Option(BFS:0, DFS:1): ");
	scanf("%d",&BD);

	if((BD==0)||(BD==1)){
		//익준
		init(argv[1]); 
		selectmod(argv[1]);
		//준호
		if(BD==0)
			fileSize_bfs(argc,argv);
		else
			fileSize_dfs(argc,argv);		//스읍 나도 그냥 함수 인자로 받을까

		//지우
		toFind = getDirName(argv[2]);

		printDir(argv[2], toFind, BD);

	}
	else
		printf("Selcect Again\n");

}
