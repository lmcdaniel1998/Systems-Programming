#ifndef HASH_H
#define HASH_H

struct nlist {
	char *key;
	int count;
	struct nlist *next;
};

typedef struct nlist node;

extern int hash_size;

extern int elements;

node **hashtab;

#endif
