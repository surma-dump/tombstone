#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#define _calloc(n,t) (t*)malloc(n*sizeof(t)) 
#define F_DIR		00
#define F_FILE		01
#define F_SELECTED	02

char *dirpath ;

struct dir {
	char *name ;
	short int flags ;
	struct *dir s ;
	struct *dir c ;
} 


struct dir *build_tree (const char *path) ;
void die(char *s) ;
void prepare_path (char *t, const char *s1, const char *s2) ;


#include "config.h"


struct dir *build_tree (const char *path) {
	struct dir *ret ;
	struct dirent *e ;
	struct stat *s ;
	DIR *d = opendir(path) ;
	if (!d)
		return NULL ;
	while((e = readdir(path)) != NULL) {
	}

}

void die(char *s) {
	fputs(s, stdout) ;
	exit(EXIT_FAILURE) ;
}

void prepare_path (char *t, const char *s1, const char *s2) {
	strcpy(t,s1) ;
	strcat(t,s2) ;
}



int main(int argc, char **argv, char **env) {
	if (argc != 2)
		die("tombstone-"VERSION" (c) 2009 Alexander Surma\n") ;
	dirpath = _calloc(strlen(argv[1]+strlen(ROOT)),char) ;
	prepare_path(dirpath, ROOT, argv[1]) ;
	head = build_tree(ROOT) ;
	free_tree(head)
	free(dirpath) ;
}
