/* Opal hexadecimal to binary converter
 * for POSIX
 * version 1
 */

#include <stdio.h>

int main(int argc, char **argv)
{
 int end, byte;

 end = scanf("%2x", &byte);

 while(end != EOF)
 {
  putchar(byte);
  end = scanf("%2x", &byte);
 }

 return 0;
}
