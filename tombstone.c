#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "config.h"

#define _calloc(n,t) (t*)malloc(n*sizeof(t)) ;

int _isdir(char *path) ;
int isdir(char *path, char *file) ;
void dirname(char *dst, char *src, int n) ;
void die(char *s) ;

void die(char *s) {
	fputs(s, stdout) ;
	exit(EXIT_FAILURE) ;
}

void dirname(char *dst, char *src, int n) {
	int i, max = strlen(src) ;
	n -= 2 ;
	if (max > n) max = n ;
	for(i = max; i >= 0; i--) {
		if(src[i] == '/') {
			strncpy(dst, src, i) ;
			return ;
		}
	}
}

int _isdir(char *path) {
	struct stat f ;

	if(!stat(path, &f))
		return 0 ;
	return S_ISDIR(f.st_mode) ;
}

int isdir(char *path, char *file) {
	char c[MAXPATH] ;

	strcpy (c, path);
	strcat (c, "/") ;
	strcat (c, file) ;
	return _isdir(c) ;
}

int main(int argc, char **argv, char **env) {
	char *curdir ;
	DIR *d ;
	struct dirent *c ;
	if (argc != 2)
		die("tombstone-"VERSION" (c) 2009 Alexander Surma\n") ;

	printf("<html><body bgcolor=\"#000000\" text=\"#00FF00\"><pre>\n") ;
	dirname(curdir, argv[1], 200) ;
	printf("%s\n", curdir) ;
	d = opendir(curdir) ;	
	if(!d) 
		die("Couldn't open directory\n") ;

	while((c = readdir(d)) != NULL) {
		//printf("%d, %s\n", isdir(curdir, c->d_name), c->d_name) ;
		printf("%s\n",  c->d_name) ;
	}
	closedir(d) ;
	printf("</pre></body></html>\n") ;
	free(curdir) ;
}
