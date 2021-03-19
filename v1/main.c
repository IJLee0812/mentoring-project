#include "head.h"

int main(int argc, char *argv[]){
	
	int BD;
	char* toFind;
	char* absDir;
	
	if(argc != 3){
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
		toFind = getDirName(argv[2]);

		printDir(argv[2], toFind, BD);

	}
	else
		printf("Selcect Again\n");
	
	//익준
	init(argv[1]); 
	selectmod(argv[1]);

}
