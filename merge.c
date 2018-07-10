/* Opal file merger
 * for POSIX
 * version 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
 int c;
 unsigned i, dc = 0, fnum = 1, limit = 0;
 char a[128], z[128], fname[256], fmt[32], *sp;
 FILE *fp;

 if(argc < 2)
 {
  fputs("Error: missing argument\n", stderr);
  exit(EXIT_FAILURE);
 }

 strncpy(a, argv[1], 127);
 a[127] = '\0';
 sp = strchr(a, '#');

 if(sp != NULL)
 {
  dc = strspn(sp, "#");
  strcpy(z, strrchr(a, '#') + 1);
  *sp = '\0';
 }
 else
 {
  z[0] = '\0';
 }

 sprintf(fmt, "%%s%%0%uu%%s", dc);

 limit = ~limit;
 sprintf(fname, fmt, a, fnum, z);
 fp = fopen(fname, "rb");
 while((fp != NULL) && (fnum < limit))
 {
  c = getc(fp);
  while(c != EOF)
  {
   putchar(c);
   c = getc(fp);
  }

  fclose(fp);
  fnum++;
  sprintf(fname, fmt, a, fnum, z);
  fp = fopen(fname, "rb");
 }

 return 0;
}
