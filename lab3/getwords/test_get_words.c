#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "getwords.h"

extern struct ListAndLength {
  char **list;
  size_t length;
};

char *readline(FILE *file);
struct ListAndLength *getwords(char *line);


int main() {
 
  /*int a;
  char *name[] = { "illegal", "ho", "mommy", "sadfasdfasdf", "x" };

  for(a = 0; a < 5; a++) {
    fputs(name[a], stdout);
  }*/ 

  int x;
  char *str;
  //char **ptr;
  struct ListAndLength *mylist;
  str = readline(stdin);
  mylist = getwords(str);
  printf("Total items: %li\n", mylist->length);
  x = 0;
  for(x; x <= mylist->length; x++) {
    printf("%d. %s\n", x + 1, mylist->list[x]);
  }
  return 0;
}
