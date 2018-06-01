/* Opal binary to hexadecimal converter
 * for POSIX
 * version 1
 */

#include <stdio.h>

int main(int argc, char **argv)
{
 int byte;
 unsigned w = 26, i = 0;

 if(argc > 1)
  sscanf(argv[1], "%u", &w);

 byte = getchar();

 while(byte != EOF)
 {
  printf("%02X", (unsigned)byte);

  i++;
  if(i >= w)
  {
   putchar('\n');
   i = 0;
  }
  else
  {
   putchar(' ');
  }

  byte = getchar();
 }

 return 0;
}
