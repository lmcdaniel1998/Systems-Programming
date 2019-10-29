#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void talk(int in, int out) {
  int num;
  char buffer[1024];

  while((num = read(in, buffer, 1024)) > 0) {
    if(write(out, buffer, num) < 0) {
      perror("write");
      exit(EXIT_FAILURE);
    }
    if(num < 0) {
      perror("write");
      exit(EXIT_FAILURE);
    }
  }
}

int main(int argc, char *argv[]) {
  int npipe[2];
  pid_t child;
  char *prog;

  if(pipe(npipe) < 0) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  
  if((child = fork()) == 0) {
    /*child*/
    if(dup2(npipe[1], STDOUT_FILENO) == -1) {
      perror("dup2");
      exit(EXIT_FAILURE);
    }
    close(npipe[0]);
    prog = "ls";

    execlp(prog, prog, NULL);
    perror("exec");
    exit(EXIT_FAILURE);
  }
  
  /*parent*/
  close(npipe[1]);
  talk(npipe[0], STDOUT_FILENO);
  wait(NULL);
  return 0;
}
