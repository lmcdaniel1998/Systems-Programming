#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "test_get_words.h"

/*
typedef struct ListAndLength {
  char **list;
  size_t length;
} ListAndLength;
*/

/*ListAndLength *list_length_init(void) {
  ListAndLength * listlength = malloc(sizeof(ListAndLength));
  if(!listlength) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  return listlength;
}*/

struct ListAndLength *getwords(char *line) {
  int word_count = 0;
  int index_new_word = 0;
  int i;
  size_t line_len = strlen(line);
  /* if not in a word 0 if in a word 1 */
  int in_or_out = 0;
  
  /* allocate space for words */
  char **words = malloc(line_len * sizeof(char));
  if(!words) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  
  /* loop through line */
  for(i = 0; i <= line_len; i++) {
    /* if a letter */
    if(isalpha(line[i]) != 0) {
      if(in_or_out == 0) {
        in_or_out = 1;
        index_new_word = i;
      }
    }
    /* if not a letter */
    else {
      if(in_or_out == 1) {
        /* allocate space for a new word */
        words[word_count] = malloc(i - index_new_word * sizeof((char) + 1));
        /*copy word from string to array */
        strncpy(words[word_count], line + index_new_word, i - index_new_word);
        in_or_out = 0;
        word_count++;
      }
    }
  }
  struct ListAndLength *listlength = malloc(sizeof(struct ListAndLength));
  listlength->length = word_count;
  listlength->list = words;

  return listlength;
}
