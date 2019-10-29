#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char *filename = argv[1];
  char *backup = argv[2];
  int one, two, three;

  one = open(filename, O_RDWR);
  write(one, "grue?", 5);
  write(one, "xyzzy!", 6);
 
  symlink(filename, backup);
  two = open(backup, O_WRONLY);
  three = dup(two);
  write(two, "Wayne", 3);
  write(three, "SNL", 3);
  
  lseek(one, 12, SEEK_SET);
  lseek(three, -5, SEEK_END);
  write(three, "ears?", 2);

  write(three, "kernel", 1);
  write(one, "42", 2);
  
  close(one);
  close(two);
  close(three);

  return 0;
}
