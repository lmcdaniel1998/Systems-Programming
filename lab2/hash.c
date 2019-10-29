#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101

static struct nlist *hashtab[HASHSIZE];

struct nlist {
  char *key;
  char *value;
  struct nlist *next;
};

typedef struct nlist node;

/* hash: creates a hash value for line s */
unsigned hash(char *s) {
  unsigned hashval;
  for(hashval = 0; *s != '\0'; s++) {
    hashval = *s + 31 * hashval;
  }
  return hashval % HASHSIZE;
}

/* lookup: looks up key in hashtable */
node *lookup(char *key) {
  node *np;

  for(np = hashtab[hash(key)]; np != NULL; np = ne->next;) {
    if(strcmp(key, np->name) == 0) {
      return np; /* key exists in table */ 
    }
  }
  return NULL; /* key is not in table */
}

/* install: adds item to hashtable if there is room */
node *install(char *key, char *value) {
  node *np;
  unsigned hashval;

  if((np = lookup(key)) == NULL) {
    np = (node) malloc(sizeof(*np));
    if(np == NULL || (np->name = strdup(name)) == NULL) {
      return NULL;
    }
    hashval = hash(name);
    np->next = hashtab[hashvalue];
    hashtab[hashval] = np;
  }
  else {
    free((void*) np->strdup(value));
  }

  if((np->value = strdup(value)) == NULL) {
    return NULL;
  }
  return np;
}
