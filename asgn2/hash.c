#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "getwords.h"

int elements = 0;

void init() {
  int i;
  hashtab = malloc(sizeof(node) * hash_size);
  if(!hashtab) {
    perror("malloc");
    exit(1);
  }
  for(i = 0; i < hash_size; i++) {
    /* allocate memory for hashtable */
    hashtab[i] = malloc(sizeof(node));
    if(hashtab[i] == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }
    hashtab[i]->next = NULL;
  }
}

/* hash: creates a hash value for word according to hashsize */
unsigned hash(char *s, long hashsize) {
  unsigned hashval;
  for(hashval = 0; *s != '\0'; s++) {
    hashval = *s + 31 * hashval;
  }
  return hashval % hashsize;
}

/* lookup: looks up word in hashtable */
node *lookup(char *key) {
  node *np;
  /* first check hash location then contine through next until end of table */
  for(np = hashtab[hash(key, hash_size)]; np != NULL; np = np->next) {
    if((np->next) != NULL) {
      if(strcmp(key, np->key) == 0) {
        return np; /* key exists in table */ 
      }
    }
  }
  return NULL; /* key is not in table */
}

/* install: puts an node in the hashtable */
node *install(char *key, int prev_count) {
  node *np;
  unsigned hashval;
  /* check if not in hashtable */
  np = lookup(key);
  if(np == NULL) {
    np = (node*) malloc(sizeof(*np));
    /* return NULL because K&R said so*/
    if(np == NULL || (np->key = strdup(key)) == NULL) {
      return NULL;
    }
    /* assign attributes to new node */
    hashval = hash(key, hash_size);
    np->next = hashtab[hashval];
    np->count = 1 + prev_count;
    hashtab[hashval] = np;
    elements ++;
    return np;
  }
  /* if already in table */
  else {
    /* increment counter if already in hashtable */
    hashtab[hash(key, hash_size)]->count = hashtab[hash(key, hash_size)]->count + 1;
    return hashtab[hash(key, hash_size)];
  } 
}

/*returns pointer to pointers of all keys in array 
char **getkeys(node **hashtab, long hash_size) {
  char **keys;
  char *key = NULL;
  int key_count, hash_loc;
  size_t keys_size;

  keys_size = 4;
  hash_loc = 0;
  key_count = 0;
  llocate memory for pointer pointer keys
  keys = malloc(keys_size);
  if(!keys) {
    perror("malloc");
    exit(1);
  }
  loop throught hashtable 
  while(hash_loc < hash_size) {
    if((hashtab[hash_loc]->next) != NULL) {
      allocate space for exact size of key 
      key = malloc(sizeof(hashtab[hash_size]->key));
      if(!key) {
        perror("malloc");
        exit(1);
      }
      check if ther isn't room in keys for key
      if(key_count > keys_size) {
        keys_size = keys_size * 2;
        keys = realloc(keys, keys_size);
        if(!keys) {
          perror("realloc");
          exit(1);
        }
      }
      add key pointer to keys
      keys[key_count] = hashtab[hash_loc]->key;
      //puts(hashtab[hash_loc]->key);
      key_count++;
    }
    hash_loc++;
  }
  if((key_count + 1) > keys_size) {
    keys_size = key_count + 1;
    keys = realloc(keys, keys_size);
    if(!keys) {
      perror("realloc");
      exit(1);
    }
  }
  keys[key_count] = NULL;
  return keys;
}

int rehash(int old_hash) {
  int new_hash, p, count;
  char *word;
  node *re_hash;
  node *temp;
  new_hash = (old_hash * 2) + 1;
  temp = malloc(sizeof(node*));
  if(!temp) {
    perror("malloc");
    exit(1);
  }
  re_hash = malloc(sizeof(node*));
  if(!re_hash) {
    perror("malloc");
    exit(1);
  }
  hashtab = realloc(hashtab, sizeof(node *) * new_hash);
  if(!hashtab) {
    perror("realloc");
    exit(1);
  }
  word = malloc(sizeof(char*) * 4);
  if(!word) {
    perror("malloc");
    exit(1);
  }
  for(p = 0; p < old_hash; p++) {
    if(hashtab[p]->next != NULL) {
      puts(hashtab[p]);
      temp = hashtab[p];
      if(sizeof(hashtab[p]->key) > sizeof(word)) {
        word = realloc(word, (sizeof(char) * sizeof(hashtab[p]->key)));
        if(!word) {
          perror("realloc");
          exit(1);
        }
      }
      word = temp->key;
      count = temp->count;
      //puts(temp->key);
      //puts(word);
      //free(hashtab[p]);
      //hashtab[p] = NULL;
      //re_hash = install(word, count);
      //puts(hashtab[p]->key);
      //putchar(hashtab[p]->count + '0');
    }
  }
  return new_hash;
}*/
