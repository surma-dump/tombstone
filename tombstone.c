#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "config.h"

#define _calloc(n,t) (t*)malloc(n*sizeof(t)) ;

void die(char *s) ;
void dirname(char *dst, char *src, int n) ;
int isdir(char *path, char *file) ;
int isfiltered(char *filename) ;

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

int isdir(char *path, char *file) {
	int ret ;
	char *c ;
	struct stat f ;

	c = _calloc(strlen(path)+strlen(file)+2, char) ;
	if(!c)
		die("Couldn't allocate memory") ;
	strcpy (c, path);
	strcat (c, "/") ;
	strcat (c, file) ;
	ret = stat(c, &f) ;
	free(c) ;
	if(ret != 0) 
		return 0 ;
	return f.st_mode  & 040000;
}

int main(int argc, char **argv, char **env) {
	char *curdir ;
	DIR *d ;
	struct dirent *c ;
	if (argc != 2)
		die("tombstone-"VERSION" (c) 2009 Alexander Surma\n") ;

	printf("<html><body bgcolor=\"#000000\" text=\"#00FF00\"><pre>\n") ;
	curdir = _calloc(200, char) ;
	dirname(curdir, argv[1], 200) ;
	d = opendir(curdir) ;	
	if(!d) 
		die("Couldn't open directory\n") ;

	while((c = readdir(d)) != NULL) {
		printf("%s - %s\n", isdir(curdir, c->d_name)?"d":"f", c->d_name) ;
	}
	closedir(d) ;
	printf("</pre></body></html>\n") ;
	free(curdir) ;
}
