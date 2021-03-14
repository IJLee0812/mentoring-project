#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

#define MAX 1000

void init(char *); void selectmod(char *); void bfs(char *, char *); int checkdir(char *); void dfs(char *, char *); void dfs_print(int, char *); char *strrev(char *); 
char wd[MAX];// 시작 경로, name에 찾고자 할 디렉터리명 저장할 배열

typedef struct{
	char Nname[MAX]; // 디렉터리 경로 저장
	// int size; // 용량 저장
}Node;

int main(int argc, char *argv[]){ 
	init(argv[1]); 
	selectmod(argv[1]);
	return 0;
}

void init(char *name){ // 탐색할 디렉터리 이름(또는 경로), 탐색 시작 절대경로(프로그램 위치)출력
	printf("탐색할 디렉터리 이름(경로) : %s\n", name);
	getcwd(wd, BUFSIZ); // 프로그램 위치를 불러오기 위해 사용, wd에 저장
	printf("프로그램 위치(절대경로) : %s\n", wd); 
	printf("\n");
	return;
}

void selectmod(char *argv){
	int mod; printf("모드 선택 (0 : bfs, 1 : dfs) : "); scanf("%d", &mod); // bfs, dfs 모드 선택
	
	if (mod == 0){
		if (strncmp("/", argv, 1) == 0) // 절대 경로로 입력했을 경우
			bfs(argv, "..");
		else if (strncmp("..", argv, 2) == 0) // ../dirname의 경우
			bfs(argv, "..");
		else if (strcmp(".", argv) == 0) // . 입력했을 경우(프로그램 위치한 디렉터리) 탐색X, 출력
			dfs_print(0, ".");
		else // dirname 또는 상대경로로 입력했을 경우
			bfs(argv, ".");
	}
	
	else if (mod == 1){
		if (strncmp("/", argv, 1) == 0) // 절대 경로로 입력했을 경우
			dfs(argv, "..");
		else if (strncmp("..", argv, 2) == 0) // ../dirname의 경우
			dfs(argv, "..");
		else if (strcmp(".", argv) == 0) // . 입력했을 경우(프로그램 위치한 디렉터리) 탐색X, 출력
			dfs_print(0, ".");
		else // dirname 또는 상대경로로 입력했을 경우
			dfs(argv, ".");
	}
}

void bfs(char *name, char *wd){ 
	struct dirent *entry; struct stat buf; DIR *dp;
	Node queue[MAX]; int front, rear; front = rear = -1; // 큐 생성
	
	if (chdir(wd) < 0){ // 디렉터리 이동, 실패 시 프로그램 종료
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}
	
	if ((dp = opendir(".")) == NULL){ // 디렉터리 열기, 실패 시 프로그램 종료
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}

	while (1){ // BFS 알고리즘
		
		while ((entry = readdir(dp)) != NULL){ // 현재 디렉터리 내용을 모두 읽었을 때 탈출
			
			lstat(entry->d_name, &buf);
			
			if (S_ISDIR(buf.st_mode)){ // 하위 디렉터리일 경우
				
				char path[MAX];
				
				if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) // ".", ".."은 고려하지 않음
					continue;
				
				printf("탐색된 디렉터리 : %s\n", entry->d_name); 
				
				if (strncmp(strrev(name), strrev(entry->d_name), strlen(entry->d_name)) == 0){ // <입력한 이름(경로) == 탐색한 디렉터리명>인 경우					
					strrev(name); strrev(entry->d_name);   
					printf("\n%s----\n", entry->d_name); // 입력한(경로의) 디렉터리명 출력
					dfs_print(0, entry->d_name); // dfs_print함수 호출
					closedir(dp); return; // 반복문 탈출, bfs함수 종료
				}
				
				else{
				strrev(name); strrev(entry->d_name);
				}
				
				getcwd(path, BUFSIZ);
				strcat(path, "/");
				strcat(path, entry->d_name);
				
				if (checkdir(path)){ // 탐색 중인 디렉터리가 하위 디렉터리를 보유한 경우
					rear++; // 저장할 공간 확보
					strcpy(queue[rear].Nname, path);// 대상 디렉터리 경로 저장
					printf("Queue에 %s 디렉터리 정보(경로) 저장\n", entry->d_name);
				}
				
				chdir("..");
			}
		}
		// 같은 깊이의 모든 노드들의 탐색이 끝난 경우
		front++;
		printf("%s 디렉터리로 이동\n", queue[front].Nname); // 선입선출, 비었으면 함수 종료
		closedir(dp); chdir(queue[front].Nname);
		
		if ((dp = opendir(queue[front].Nname)) == NULL){ // Dequeue한 디렉터리로 이동
			printf("Dequeue 오류 발생. 프로그램 종료\n");
			exit(1);
		}
	}
	return;
}

int checkdir(char *path){ // 하위 디렉터리 보유 여부 체크 함수
	struct dirent *entry; struct stat buf; DIR *dp;
	
	chdir(path); dp = opendir(path);
	
	while ((entry = readdir(dp)) != NULL){
		
		lstat(entry->d_name, &buf);
		
		if (S_ISREG(buf.st_mode)) continue; 
		
		else if (S_ISDIR(buf.st_mode)){ // 하위 디렉터리가 있을 경우
			
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;
			printf("탐색된 디렉터리는 하위 디렉터리 존재\n");
			closedir(dp);
			return 1; // 1 반환, if문 True
		
		}
	}
	
	printf("탐색된 디렉터리는 하위 디렉터리 존재 X\n");
	closedir(dp);
	return 0; // 0 반환, if문 False
}

void dfs(char *name, char *wd){ // 입력한 디렉터리를 찾는 함수, dfs 알고리즘
	struct dirent *entry; struct stat buf; DIR *dp; 
	
	if (chdir(wd) < 0){ // 디렉터리 이동, 실패 시 프로그램 종료
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}
	
	if ((dp = opendir(".")) == NULL){ // 디렉터리 열기, 실패 시 프로그램 종료
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}
	
	while ((entry = readdir(dp)) != NULL){ // 현재 디렉터리 내용을 모두 읽었을 때 탈출
		
		lstat(entry->d_name, &buf);
		
		if (S_ISDIR(buf.st_mode)){ // 하위 디렉터리일 경우
			
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) // ".", ".."은 고려하지 않음
				continue;
			
			if (strncmp(strrev(name), strrev(entry->d_name), strlen(entry->d_name)) == 0){ // <입력한 이름(경로) == 탐색한 디렉터리명>인 경우
				strrev(name); strrev(entry->d_name); // 비교 위해 뒤집은 문자열 원상태로 복구
				printf("%s----\n", entry->d_name); // 입력한(경로의) 디렉터리명 출력
				dfs_print(0, entry->d_name); // dfs_print함수 호출
				chdir(".."); closedir(dp); return; // dfs 함수 종료
			}
			
			else{
				strrev(name); strrev(entry->d_name); // 비교 위해 뒤집은 문자열 원상태로 복구
			}
			
			dfs(name, entry->d_name); // 없을 경우, 재귀 호출(더 하위 디렉터리로 이동)
		}
	} 
	
	chdir(".."); closedir(dp); // 백트래킹(부모 디렉터리로 올라감)
}

void dfs_print(int tmp, char *wd){ // 디렉터리를 트리구조로 출력하는 함수, dfs 알고리즘
	struct dirent *entry; struct stat buf; DIR *dp; int count = 0;
	
	count = tmp; // \t 횟수 구분(깊이 동일한 노드들 같은 열에 출력하기 위함)
	
	if (chdir(wd) < 0){ // dfs()와 동일
		printf("오류 발생! 프로그램 종료.\n");
		exit(1);
	}
	
	if ((dp = opendir(".")) == NULL){
		printf("오류 발생! 프로그램 종료.\n");
	}  
	
	while ((entry = readdir(dp)) != NULL){ // dfs()와 동일
		
		lstat(entry->d_name, &buf);
		
		if (S_ISREG(buf.st_mode)){ // 탐색한 것이 파일일 경우
			
			for (int i = 0 ; i <= count ; i++)
				printf("\t");
			printf("-%s\n", entry->d_name);
		
		}
		
		else if (S_ISDIR(buf.st_mode)){ // 탐색한 것이 디렉터리일 경우
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) // dfs()와 동일
				continue;
			
			for (int i = 0 ; i < 3 ; i++){ // 칸을 벌려서 겹치는 경우 예방하기 위함
				for (int j = 0 ; j <= count ; j++)
					printf("\t");
				printf("|\n");
			}
			
			for (int i = 0 ; i <= count ; i++) printf("\t");
			printf("-%s----\n", entry->d_name); // (하위)디렉터리명 출력
			tmp = count + 1;
			dfs_print(tmp, entry->d_name); // 재귀 호출 (더 하위 디렉터리로 이동)
			
			for (int i = 0 ; i < 3 ; i++){ // 위와 동일
				for (int j = 0 ; j <= count ; j++)
					printf("\t");
				printf("|\n");
			}
		
		}
	}
	
	chdir(".."); closedir(dp); // 백트래킹(부모 디렉터리로 올라감)
}

char *strrev(char *str){ // 문자열 뒤집는 함수(string.h_strrev는 리눅스 사용 불가)
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
