#include "head.h"
void init(char *name){
	printf("탐색할 디렉터리 이름 : %s\n", name);
	strcpy(finddir, name);
	getcwd(wd, BUFSIZ);
	printf("탐색 시작 경로 : %s", wd);
	printf("\n");
	return;
}

void dfs(char *finddir, char *wd){ 
	struct dirent *entry; struct stat buf; DIR *dp;
	if (chdir(wd) < 0) {
		printf("Error occured!\n");
		exit(1);
	}
	if ((dp = opendir(".")) == NULL){
		printf("Error occured!\n");
		exit(1);
	}
	while ((entry = readdir(dp)) != NULL) { //현재 디렉터리 내용 모두 읽었을 때 탈출
		lstat(entry->d_name, &buf);
		if (S_ISDIR(buf.st_mode)){ // 입력 디렉터리 이름과 같은지 확인, 재귀
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
				continue;
			if (strcmp(finddir, entry->d_name) == 0){// 현 디렉터리와 일치 시
				printf("%s----\n", entry->d_name);
				dfs_print(0, entry->d_name);
				chdir(".."); closedir(dp);
				return; // dfs 함수 종료
			}
			dfs(finddir, entry->d_name);// 재귀 호출
		}
	} 
	chdir(".."); closedir(dp);
}

void dfs_print(int tmp, char *wd) { // 트리구조 출력 -> dfs
	struct dirent *entry; struct stat buf; DIR *dp; int count = 0;
	count = tmp; // \t 횟수 구분(depth)
	if (chdir(wd) < 0) {
		printf("Error occured!\n");
		exit(1);
	}
	if ((dp = opendir(".")) == NULL) {
		printf("Error occured!\n");
		exit(1);
	}  
	while ((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name, &buf);
		if (S_ISREG(buf.st_mode)) {
			for (int i = 0 ; i <= count ; i++)
				printf("\t");
			printf("-%s\n", entry->d_name);
		}
		else if (S_ISDIR(buf.st_mode)) {
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;
			for (int i = 0 ; i < 3 ; i++){
				for (int j = 0 ; j <= count ; j++)
					printf("\t");
				printf("|\n");
			}
			for (int i = 0 ; i <= count ; i++) printf("\t");
			printf("-%s----\n", entry->d_name);
			tmp = count + 1;
			dfs_print(tmp, entry->d_name);
			for (int i = 0 ; i < 3 ; i++){
				for (int j = 0 ; j <= count ; j++)
					printf("\t");
				printf("|\n");
			}
		}
	}
	chdir(".."); closedir(dp);
}
