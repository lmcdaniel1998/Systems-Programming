#include <stdio.h>
#include <unistd.h>
#include <string.h>

unsigned char make_byte(const int bits[]) {

  int i;
  unsigned char byte = 0;

  for (i = 0; i < 8; i++ ) {
    if (bits[i] == 1) {
      byte = byte | (1 << 1);
    }
    else {
      byte = byte | (0 << 1);
    }
  }
  return byte;
}

int main() {

  int byte_1[] = { 1, 1, 1, 1, 0, 1, 0, 1 }; /*F5*/

  int byte_2[] = { 0, 0, 1, 0, 1, 1, 0, 0 }; /*2C*/

  int byte_3[] = { 0, 0, 0, 0, 0, 0, 0, 1 }; /*01*/

  int byte_4[] = { 1, 1, 1, 1, 1, 1, 1, 1 }; /*FF*/

  printf("F5 = %x\n", make_byte(byte_1));
  printf("2C = %x\n", make_byte(byte_2));
  printf("01 = %x\n", make_byte(byte_3));
  printf("FF = %x\n", make_byte(byte_4));
  return 0;
}
