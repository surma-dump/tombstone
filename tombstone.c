#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

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
	struct tree *tree ;
	struct bucket *next ;
} ;

struct hashlist {
	int size;
	hash_func func ;
	struct bucket **list;
} ;

struct filespace {
	struct hashlist *hash ;
	struct tree *tree ;
} ;

struct tree *add_child(struct tree *t, struct item *i) ;
void add_hash(struct hashlist *h, struct tree *t) ;
void die(char *s) ;
void free_filespace(struct filespace *f) ;
void free_hashlist(struct hashlist *h) ;
void free_item(struct item *i) ;
void free_tree(struct tree *t) ;
void init_filespace(struct filespace *f, char *path) ;
void init_hashlist(struct hashlist *h, int size, hash_func hf) ;
void init_tree(struct tree *t) ;
int is_dir(char *path) ;
int modulo_hash(int size, char* name) ;
void path_to_filespace(struct tree *t, struct hashlist *h, char *path) ;
void prepare_path (char *t, char *s1, char *s2, int separator) ;

#ifdef DEBUG
void print_tree(struct tree *t, int indent) ;	
void print_hash(struct hashlist *h) ;
#endif

#include "config.h"


struct tree *add_child(struct tree *t, struct item *i) {
	struct tree *c = _calloc(struct tree, 1) ;
	init_tree(c) ;
	c->item = i ;

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
	return c;
}

void add_hash(struct hashlist *h, struct tree *t) {
	struct bucket *b = _calloc(struct bucket, 1) ;	
	int j = hash(HASHSIZE, t->item->name) ;
	b->tree = t ;
	b->next = h->list[j] ; 
	h->list[j] = b ;
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
	int i ;
	struct bucket *b ;

	for(i = 0; i < HASHSIZE; i++) {
		while(h->list[i] != NULL) {
			b = h->list[i] ;
			h->list[i] = h->list[i]->next ;
			free(b) ;
		}
	}
	free(h->list) ;
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
	path_to_filespace(f->tree, f->hash, path) ;
}

void init_hashlist(struct hashlist *h, int size, hash_func hf) {
	int i ;

	h->size = size ;
	h->list = _calloc(struct bucket*, size) ;
	for(i = 0; i < HASHSIZE; i++) 
		h->list[i] = NULL ;
	h->func = hf ;
}

void init_tree(struct tree *t) {
	t->item = NULL ;
	t->sibling = t ;
	t->child = NULL ;
	t->parent = NULL ;
}

int is_dir(char *path) {
	struct stat s ;
	stat(path, &s) ;
	return S_ISDIR(s.st_mode) ;
}

// Interpretes the string as a numbe to base 256
int modulo_hash(int size, char *name) {
	int i = 0;
	while (*name != 0) 
		i = (i*256+*(name++))%size ;
	return i ;
}

void path_to_filespace(struct tree *t, struct hashlist *h, char *path) {
	DIR *d ;
	struct dirent *e ;
	struct tree *newt ;
	char *newpath ;
	struct item *item ;
	int l = strlen(path) ;

	if((d = opendir(path)) != NULL) {
		while((e = readdir(d)) != NULL) {
			if(e->d_name[0] != '.') {
				item = _calloc(struct item, 1) ;
				item->name = _calloc(char, strlen(e->d_name) + 1) ;
				strcpy(item->name, e->d_name) ;
				newt = add_child(t, item) ;

				newpath = _calloc(char, l+NAME_MAX+2) ;
				prepare_path(newpath, path, e->d_name, 1) ;

				if(is_dir(newpath)) {
					item->flags = F_DIR ;
					path_to_filespace(newt, h, newpath) ;
				}
				else{
					item->flags = F_FILE ;
				}
				free(newpath) ;
				add_hash(h, newt) ;
			}
		}
		closedir(d) ;
	}
}

void prepare_path(char *t, char *s1, char *s2, int separator) {
	strcpy(t,s1) ;
	if(separator)
		strcat(t,"/") ;
	strcat(t,s2) ;
}



int main(int argc, char **argv, char **env) {
	char *dirpath ;
	struct filespace fs ;
	if (argc != 2)
		1; //die("tombstone-"VERSION" (c) 2009 Alexander Surma\n") ; // DEBUG!! //FIXME

	init_filespace(&fs, ROOT) ;
	print_tree(fs.tree,0) ;
	print_hash(fs.hash) ;
	free_filespace(&fs) ;
	return 0 ;
}

#ifdef DEBUG

void print_hash(struct hashlist *h) {
	int i;
	struct bucket *b ;
	printf("HASH | Inhalt\n") ; 
	for(i = 0; i < HASHSIZE; i++) {
		printf("%4d | ",i) ;
		b = h->list[i] ;
		while(b != NULL) {
			printf("\"%s\" ->",b->tree->item->name) ;
			b = b->next ;
		}
		printf("NULL\n") ;
	}
}

// Bad, recursive DFS
void print_tree(struct tree *t, int indent) {
	int i;
	for(i = 0; i < indent; i++)
		printf("\t") ;
	if(t->item != NULL)
		printf("%s (%s)\n",t->item->name,(((t->item->flags & F_DIR)==F_DIR)?"Dir":"File")) ;
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
