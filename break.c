/* Opal file breaker
 * for POSIX
 * version 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
 int c;
 unsigned i, dc = 0, fnum = 0, limit = 0;
 long long unsigned bnum, bmax;
 char a[128], z[128], fname[256], fmt[32], *sp;
 FILE *fp;

 if(argc < 3)
 {
  fputs("Missing argument\n", stderr);
  exit(EXIT_FAILURE);
 }

 sscanf(argv[1], "%llu", &bmax);

 strncpy(a, argv[2], 127);
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
 c = getchar();
 while((c != EOF) && (fnum < limit))
 {
  sprintf(fname, fmt, a, fnum, z);
  fp = fopen(fname, "wb");

  if(fp == NULL)
  {
   fprintf(stderr, "Error: cannot not write file: %s\n", fname);
   exit(EXIT_FAILURE);
  }

  bnum = 0;
  while((c != EOF) && (bnum < bmax))
  {
   putc(c, fp);
   bnum++;
   c = getchar();
  }

  fclose(fp);
  fnum++;
 }

 return 0;
}
