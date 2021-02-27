#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

void init(char *); void dfs(char *, char *); void dfs_print(int, char *);
char wd[BUFSIZ]; //wd에 현재 경로 저장하기 위함 
int re = 0;

int main(int argc, char *argv[]){ // argv[1] => 총 크기를 구하고 싶은 디렉토리의 경로(or이름)

	char finddir[BUFSIZ]; init(finddir); //디렉토리명 입력, 시작경로 출력
	dfs(finddir, ".");
	return 0;
}

void init(char *finddir){ 
	printf("디렉터리 입력 : ");
	scanf("%s", finddir); //finddir 배열에 디렉터리명 저장
	getcwd(wd, BUFSIZ);
	printf("시작 경로 : %s", wd);
	printf("\n");
	return;
}

void dfs(char *finddir, char *wd){ // 입력한 디렉터리 이름과 같은 디렉터리를 가지는 디렉터리가 있을 때 까지 탐색
	struct dirent *entry; struct stat buf; DIR *dp;
	if (chdir(wd) < 0) {
		printf("Error occured!\n");
		exit(1);
	}
	if ((dp = opendir(".")) == NULL){
		printf("Error occured!\n");
		exit(1);
	}
	while ((entry = readdir(dp)) != NULL) { //현재 디렉터리 내 파일을 다 읽었을 때 탈출(종료)
		lstat(entry->d_name, &buf);
		if (S_ISDIR(buf.st_mode)){ // 입력 디렉터리 이름과 같은지 확인, 재귀
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
				continue;
			//printf("탐색된 디렉터리 : %s\n", entry->d_name);
			if (strcmp(finddir, entry->d_name) == 0){// 현 디렉터리와 일치 시
				printf("%s-----\n", entry->d_name);
				dfs_print(re, entry->d_name);
				chdir(".."); closedir(dp);
				return; // dfs 함수 종료
			}
			dfs(finddir, entry->d_name);// 재귀 호출
		}
	} 
	chdir(".."); closedir(dp);
}

void dfs_print(int re, char *wd) { // 트리구조로 출력하는 함수는 dfs로 고정
	struct dirent *entry; struct stat buf; DIR *dp; 
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
			if (!re){
			printf("-%s\n", entry->d_name);
			printf("\t|\n");
			}
			else{
				printf("-%s\n", entry->d_name);
				printf("|\n");
			}
		}

		else if (S_ISDIR(buf.st_mode)) {
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;
			printf("\t|\n\t|\n\t|\n");
			printf("\t-%s---", entry->d_name);
			dfs_print(re = 1, entry->d_name);
			printf("\t|\n\t|\n\t|\n");
		}
	}
	chdir(".."); closedir(dp);
}


