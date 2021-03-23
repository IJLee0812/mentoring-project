#include "head.h"

int main(int argc, char *argv[]){
	
	int BD; //BFS or DFS
	char* toFind; //File or Dir Name to Find
	char* absDir;
	
	if(argc != 3){ //Need more data
		fprintf(stderr, "Usage : Program [-Dir] [-Dir/-File]\n");
		exit(1);
	}

	printf("Search Option(BFS:0, DFS:1): ");
	scanf("%d",&BD);

	if((BD==0)||(BD==1)){
		
		if (BD==0)
			printf("BFS Search\n");
		else
			printf("DFS Search\n");
		printf("\n");

		//path
		char tmp_Path[MAX];
		strcpy(tmp_Path,argv[2]);
		char* toFind = extract_Filename(tmp_Path); //입력받은 argv[2]에서 파일 또는 디렉터리 이름만을 추출해 toFind에 넣음
		before_Search(argv[2], toFind, BD);
		printf("\n");

		//size && tree
		init(argv[1]);
		absDir = absolute(argv[1]); //size
		bfs_or_dfs(absDir,BD);
		printf("\n");
		
		printf("Tree:"); //tree
		selectmod(argv[1], BD);
	}
	else
		printf("Select Error(BFS:0, DFS:1)\n");

	return 0;
}
