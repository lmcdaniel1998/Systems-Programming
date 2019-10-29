#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sorts.h"
#include "readline.h"
#include "hash.h"
#include "getwords.h"
#include "putwords.h"

int hash_size = 15001;

void init();
unsigned hash(char *s, long hashsize);
node *lookup(char *word);
node *install(char *key, int prev_count);
char *readline(FILE *file);
char** getwords(char* line);
char** sort(node *hashtab[], long hash_size, int clip_size);
void putwords(node *hashtab[], int numWords);
int rehash(int old_hash);
void secondary(FILE *file, int numWords);



int main(int argc, char* argv[]) { 
  int i;
  /* locates input parameters */
  int numWords = 10;
  if(argc > 1) {
    for(i = 1; i < argc; i++) {
      if(strcmp(argv[i], "-n") == 0) {
        if(atoi(argv[i + 1])) {
          numWords = atoi(argv[i + 1]);
          i++;
        }
        else {
          perror("stdin");
          exit(1);
        }
      }
      else {
        FILE *file = fopen(argv[i], "r");
        if(!file) {
          perror("file io");
        }
        else {
          secondary(file, numWords);
          fclose(file);
        }
      }
    }
  }
  else {
    secondary(stdin, numWords);
  }
  return 0;
}
  
void secondary(FILE *file, int numWords) {
  char *line;
  char *new_word;
  char **words;
  int c, z;

  line = malloc(sizeof(char));
  if(!line) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  /* set first line of file to curr */
  line = (char*) readline(file);
  /* initialize hashtab */
  init();
  /* loop through file until end */
  while(line) {
     c = 0;
     words = malloc(sizeof(char*) * sizeof(line));
     if(!words) {
       perror("malloc");
       exit(EXIT_FAILURE);
     }
     words = (char**) getwords(line);
     /* insert each word into hashtable */
     new_word = malloc(sizeof(char) * 4);
     if(!new_word) {
       perror("malloc");
       exit(EXIT_FAILURE);
     }
     while(words[c] != NULL) {
       if(sizeof(words[c]) > sizeof(new_word)) {
         new_word = realloc(new_word, (sizeof(char*) * sizeof(words[c])));
         if(!new_word) {
           perror("malloc");
           exit(EXIT_FAILURE);
         }
       }
       if(elements < 1200) {
         new_word = (char*) install(words[c], 0);
         c++;
       }
       else {
         exit(EXIT_SUCCESS);
       }
     }
     line = readline(file);
  }
  free(new_word);
  /* sort hashtable */
  words = realloc(words, (sizeof(char) * (numWords + 1)));
  if(!words) {
    perror("realloc");
    exit(1);
  }
  words = (char**) sort(hashtab, hash_size, numWords);
  putwords(hashtab, numWords);
}
