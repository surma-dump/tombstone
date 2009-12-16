#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#define _calloc(t,n) (t*)malloc(n*sizeof(t)) 
#define F_DIR		001
#define F_FILE		002
#define F_SELECTED	004
#define F_INVALID	010



typedef int(*hash_func)(int size, char* name) ;

struct item {
	char *name ;
	int flags ;
} ;

struct tree {
	struct item *item ;
	struct tree *sibling ;
	struct tree *child ;
	struct tree *parent ;
} ;

struct bucket {
	struct item *item ;
	struct bucket *next ;
} ;

struct hashlist {
	int size;
	hash_func func ;
	struct bucket *list;
} ;

struct filespace {
	struct hashlist *hash ;
	struct tree *tree ;
} ;

void add_child(struct tree *t, struct item *_c) ;
void die(char *s) ;
void free_filespace(struct filespace *f) ;
void free_hashlist(struct hashlist *h) ;
void free_item(struct item *i) ;
void free_tree(struct tree *t) ;
void init_filespace(struct filespace *f, char *path) ;
void init_hashlist(struct hashlist *h, int size, hash_func hf) ;
void init_tree(struct tree *t) ;
int modulo_hash(int size, char* name) ;
void prepare_path (char *t, char *s1, char *s2) ;

#ifdef DEBUG
void print_tree(struct tree *t, int indent) ;	
#endif

#include "config.h"


void add_child(struct tree *t, struct item *_c) {
	struct tree *c = _calloc(struct tree, 1) ;
	init_tree(c) ;
	c->item = _c ;

	if (t->child != NULL) {
		struct tree *p = t->child->sibling ;
		t->child->sibling = c ;
		c->sibling = p ;
	}
	else {
		t->child = c ;
		c->sibling = c ;
	}
	c->parent = t ;
	return ;
}

void die(char *s) {
	fputs(s, stdout) ;
	exit(EXIT_FAILURE) ;
}

void free_filespace(struct filespace *f) {
	free_hashlist(f->hash) ;
	free_tree(f->tree) ;
}

void free_hashlist(struct hashlist *h) {
}

void free_item(struct item *i) {
	free(i->name) ;
}

void free_tree(struct tree *t) {
	if (t->child != NULL) {
		struct tree *p = t->child->sibling ;
		while (p != t->child) {
			struct tree *new = p->sibling ;
			free_tree(p) ;
			p = new ;
		}
		free_tree(p) ;
	}
	if(t->item != NULL)
		free_item(t->item) ;
	free(t) ;
}

void init_filespace(struct filespace *f, char *path) {
	f->hash = _calloc(struct hashlist, 1) ;
	init_hashlist((f->hash), HASHSIZE, hash) ;
	f->tree = _calloc(struct tree, 1) ;
	init_tree(f->tree) ;
}

void init_hashlist(struct hashlist *h, int size, hash_func hf) {
	h->size = size ;
	h->list = _calloc(struct bucket, size) ;
	h->func = hf ;
}

void init_tree(struct tree *t) {
	t->item = NULL ;
	t->sibling = t ;
	t->child = NULL ;
	t->parent = NULL ;
}

// Interpretes the string as a numbe to base 256
int modulo_hash(int size, char *name) {
	int i = 0;
	while (*name != 0) 
		i = (i*256+*(name++))%size ;
	return i ;
}

void prepare_path (char *t, char *s1, char *s2) {
	strcpy(t,s1) ;
	strcat(t,s2) ;
}



int main(int argc, char **argv, char **env) {
	char *dirpath ;
	struct filespace fs ;
	if (argc != 2)
		1; //die("tombstone-"VERSION" (c) 2009 Alexander Surma\n") ; // DEBUG!! //FIXME
	init_filespace(&fs, ROOT) ;
	print_tree(fs.tree,0) ;
	free_filespace(&fs) ;
	return 0 ;
}

#ifdef DEBUG
// Bad, recursive DFS
void print_tree(struct tree *t, int indent) {
	int i;
	for(i = 0; i < indent; i++)
		printf("\t") ;
	if(t->item != NULL)
		printf("%s\n",t->item->name) ;
	else
		printf("Root\n") ;
	if(t->child != NULL) {
		struct tree *p = t->child->sibling ;
		while(p != t->child) {
			print_tree(p, indent+1) ;
			p = p->sibling ;
		}
		print_tree(p, indent+1) ;
	}
}
#endif
