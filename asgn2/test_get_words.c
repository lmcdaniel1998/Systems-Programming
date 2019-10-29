#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **getwords(char *line);

int main() {
 
  /*int a;
  char *name[] = { "illegal", "ho", "mommy", "sadfasdfasdf", "x" };

  for(a = 0; a < 5; a++) {
    fputs(name[a], stdout);
  }*/
  
  int x;
  putchar('x');
  putchar('\n');
  char str[] = "asdfasdsdf fsdfad fasdfas dfasdfa sdfasd fasdfa sdfasdfa sdfasdfa sdfdsfas dfasdfa sdfasdfa sdfasdfa sdfasd fasdfasd ";
  char **ptr;
  ptr = getwords(str);
  x = 0;
  while(ptr[x] != NULL) {
    fputs(ptr[x], stdout);
    putchar('\n');
    x++;
  }
  return 0;
}
