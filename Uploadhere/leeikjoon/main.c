#include "head.h"
int main(int argc, char *argv[]){
	init(argv[1]);
	dfs(finddir, ".");
	return 0;
}
