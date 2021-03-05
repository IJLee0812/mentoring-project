#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

void init(char *); void bfs(char *, char *); void dfs(char *, char *); void dfs_print(int, char *); char *strrev(char *);
char wd[BUFSIZ];//wd에 시작 경로, finddir에 찾고자 할 디렉터리명 저장

int main(int argc, char *argv[]){ // argv[1] => 총 크기를 구하고 싶은 디렉토리의 경로 or 이름
	init(argv[1]); //탐색할 디렉터리 이름(경로), 탐색 시작 경로 출력
	
	if (strncmp("/", argv[1], 1) == 0){
		dfs(argv[1], "..");
	}
	else{
		dfs(argv[1], ".");
	}
	return 0;
}

char *strrev(char *str){ //문자열 뒤집는 strrev 함수(string.h_strrev는 리눅스에서 사용 불가)
	char *p1, *p2;
	if (!str || !*str)
		return str;
	for (p1 = str, p2 = str + strlen(str)-1; p2 > p1 ; ++p1, --p2){
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	return str;
}

void init(char *name){
	printf("탐색할 디렉터리 이름(경로) : %s\n", name);
	getcwd(wd, BUFSIZ);
	printf("프로그램 위치(절대경로) : %s", wd);
	printf("\n");
	return;
}

void dfs(char *name, char *wd){ // finddir -> name 으로 수정
	struct dirent *entry; struct stat buf; DIR *dp;
	if (chdir(wd) < 0){
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}
	if ((dp = opendir(".")) == NULL){
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}
	while ((entry = readdir(dp)) != NULL){ //현재 디렉터리 내용 모두 읽었을 때 탈출
		lstat(entry->d_name, &buf);
		if (S_ISDIR(buf.st_mode)){ // 입력 디렉터리 이름과 같은지 확인, 재귀
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
				continue;
			if (strncmp(strrev(name), strrev(entry->d_name), strlen(entry->d_name)) == 0){// 현 디렉터리와 일치 시
				strrev(name); strrev(entry->d_name);
				printf("%s----\n", entry->d_name);
				dfs_print(0, entry->d_name);
				chdir(".."); closedir(dp);
				return; // dfs 함수 종료
			}
			else{
				strrev(name); strrev(entry->d_name);
			}
			dfs(name, entry->d_name);// 재귀 호출
		}
	} 
	chdir(".."); closedir(dp);
}
/*
void bfs(char *finddir, char *wd){ // Queue 필요 -> 전역변수로. 노드 방문시 포인터저장
	struct dirent *entry; struct stat buf; DIR *dp;
	if (chdir(wd) < 0){
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}
	if ((dp = opendir(".")) == NULL){
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}
	/////bfs algorithm/////
	while ((entry = readdir(dp)) != NULL){
}
*/
void dfs_print(int tmp, char *wd){ // 트리구조 출력 -> dfs
	struct dirent *entry; struct stat buf; DIR *dp; int count = 0;
	count = tmp; // \t 횟수 구분(depth)
	if (chdir(wd) < 0){
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}
	if ((dp = opendir(".")) == NULL){
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}  
	while ((entry = readdir(dp)) != NULL){
		lstat(entry->d_name, &buf);
		if (S_ISREG(buf.st_mode)){
			for (int i = 0 ; i <= count ; i++)
				printf("\t");
			printf("-%s\n", entry->d_name);
		}
		else if (S_ISDIR(buf.st_mode)){
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
