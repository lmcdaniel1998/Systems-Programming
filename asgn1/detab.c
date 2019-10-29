#include <stdio.h>
#define TABSPACE 8 /* column spacing between tabs */

/* program to replace tabs with proper number of spaces */
int main() {

  int c, col, tab;
  /* c counts the column number starting at 0 */
  col = 0;
  while((c = getchar()) != EOF) {
   
    switch (c) {
      case '\n':
        putchar(c);
        col = 0;
        break;
      case '\t':
        for (tab = TABSPACE - col % TABSPACE, col += tab; tab > 0; --tab) {
          putchar(' ');
        }
        break;
      case '\b':
        if (col >= 1) {
          putchar(c);
          --col;
        }
        else {
          col = 0;
        }
        break;
      default:
        putchar(c);
        ++col;
        break;
    }
  }
  return 0;
}
