#include <stdio.h>
#include <unistd.h>
#include <time.h>

double progA(int sz) {

  int n;
  double total_time;
  clock_t start, end;
  char buffer[sz];

  start = clock();

  while((n = read(STDIN_FILENO, buffer, sz)) > 0) {
    write(STDOUT_FILENO, buffer, n);
  }
  end = clock();

  total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
  return total_time;

}

double progB() {
  int c;
  double total_time;
  clock_t start, end;
  
  start = clock();

  while(EOF != (c = getchar())) {
    putchar(c);
  }

  end = clock();

  total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
  return total_time;
}

int main() {
  printf("ProgA buffer 1: %f\n", progA(1));
  printf("ProgA buffer 8192: %f\n", progA(8192));
  printf("ProgB: %f\n", progB);
  return 0;
}
