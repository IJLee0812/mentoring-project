#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

void init(char *); void dfs(char *, char *); void dfs_print(int, char *);
char wd[BUFSIZ]; char finddir[BUFSIZ];
