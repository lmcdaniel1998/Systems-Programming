#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void talk(int fd) {
  int c;
  while((c = getchar()) != EOF) {
    putchar(c);
  }
  printf("This is %d\n", fd);
}

int main(int argc, char *argv[]) {
  pid_t child;
  int next[2], last[2];
  int num, i;

  num = atoi(argv[1]);

  for(i = 0; i < num; i++) {
    if(i < (num -1)) {
      pipe(next);
    }
    if((child = fork()) == 0) {
      /* not first stage */
      if(i > 0) {
        if(dup2(last[0], STDIN_FILENO) == -1) {
          perror("dup2");
          exit(EXIT_FAILURE);
        }
        close(last[0]);
        close(last[1]);
      }
      /* not last stage */
      if(i < (num - 1)) {
        if(dup2(next[1], STDOUT_FILENO) == -1) {
          perror("dup2");
          exit(EXIT_FAILURE);
        }
      }
      close(next[0]);
      close(next[1]);
      talk(num);
      exit(EXIT_SUCCESS);
    }
    /* parent */
    close(last[0]);
    close(last[1]);
    last[0] = next[0];
    last[1] = next[1];
  }
  close(last[0]);
  close(last[1]);
  /* waits */
  for(i = 0; i < (num - 1); i++) {
    wait(NULL);
  }
  return 0;
}
