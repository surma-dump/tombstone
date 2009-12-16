#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#define _calloc(n,t) (t*)malloc(n*sizeof(t)) 
#define F_DIR		01
#define F_FILE		02
#define F_SELECTED	04



struct item {
	char *name ;
	int flags ;
}

struct tree {
	struct item *item ;
	struct tree *sibling ;
	struct tree *child ;
}

struct level {
	struct item *item ;
	struct level *next ;
}

struct levelhead {
	struct level *list ;
	struct levelhead *next ;
}

struct dir *build_tree (char *path) ;
void die(char *s) ;
void prepare_path (char *t, char *s1, char *s2) ;


#include "config.h"


struct dir *build_tree (char *path) {
	struct dirent *e ;
	struct stat *s ;
	DIR *d ;
	struct dir *ret ;
	ret = _calloc(1,struct dir) ;

	*d = opendir(path) ;
	if (!d)
		return NULL ;
	while((e = readdir(path)) != NULL) {
		
	}

}

void die(char *s) {
	fputs(s, stdout) ;
	exit(EXIT_FAILURE) ;
}


void prepare_path (char *t, char *s1, char *s2) {
	strcpy(t,s1) ;
	strcat(t,s2) ;
}



int main(int argc, char **argv, char **env) {
	char *dirpath ;
	if (argc != 2)
		die("tombstone-"VERSION" (c) 2009 Alexander Surma\n") ;
	dirpath = _calloc(strlen(argv[1]+strlen(ROOT)),char) ;
	prepare_path(dirpath, ROOT, argv[1]) ;
	head = build_tree(ROOT) ;
	free_tree(head)
	free(dirpath) ;
}
