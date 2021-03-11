#include "head.h"

int main(int argc, char *argv[]){
	
	if(argc != 3){
		fprintf(stderr, "Usage : Program [-Dir] [-File]\n");
		exit(1);
	}

	//argv[1]
	fileSize(argc, argv);

	//트리
	tree(argc, argv);

	//argv[2]
	char * toFind = getDirName(argv[2]);
	printDir(argv[2], toFind);
	

}
