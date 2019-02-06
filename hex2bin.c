/* Opal hexadecimal to binary converter
 * for Unix
 * version 2
 */


/* pieces section */

#include <stdio.h>
/* putc()
 * perror()
 * fopen()
 * fclose()
 * FILE
 * NULL
 * EOF
 */

#include <stdlib.h>
/* exit()
 * NULL
 * EXIT_FAILURE
 * EXIT_SUCCESS
 */

#include <stdbool.h>
/* bool
 * true
 * false
 */

#include <unistd.h>
/* getopt()
 */


/* functions section */

/* help message */
void help()
{
 char *message = "Opal hexadecimal to binary converter\n"
 "version 2\n\n"
 "options\n"
 "h: print help and exit\n"
 "i: specify input file (default: standard input)\n"
 "o: specify output file (default: standard output)\n";
 fputs(message, stderr);
}

/* library error */
void error(char *message)
{
 perror(message);
 exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
 int c;
 unsigned b;
 char *inn, *outn;
 FILE *inf, *outf;
 extern char *optarg;
 extern int opterr, optind, optopt;

 inn = NULL; outn = NULL;
 inf = stdin; outf = stdout;

 /* parse the command line */
 while((c = getopt(argc, argv, "hi:o:")) != -1)
  switch(c)
  {
   case 'h': help(); exit(EXIT_SUCCESS);
   case 'i': inn = optarg; break;
   case 'o': outn = optarg; break;
   case '?': exit(EXIT_FAILURE);
  }

 /* open input file */
 if(inn != NULL)
  if((inf = fopen(inn, "rb")) == NULL)
   error("open input file");

 /* open output file */
 if(outn != NULL)
  if((outf = fopen(outn, "wb")) == NULL)
   error("open output file");

 /* get hexadecimal numbers; output binary bytes */
 while(fscanf(inf, "%2x", &b) != EOF)
  if(putc((int)b, outf) == EOF)
   error("write byte");

 /* close input file */
 if(inn != NULL)
  fclose(inf);

 /* close output file */
 if(outn != NULL)
  fclose(outf);

 return EXIT_SUCCESS;
}
