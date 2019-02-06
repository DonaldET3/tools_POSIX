/* Opal file merger
 * for Unix
 * version 2
 */


/* pieces section */

#include <stdio.h>
/* getc()
 * putc()
 * sscanf()
 * fprintf()
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

#include <string.h>
/* strlen()
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
 char *message = "Opal file merger\n"
 "version 2\n\n"
 "options\n"
 "h: print help and exit\n"
 "f: specify output file name (default: standard output)\n"
 "a: first part of input file names\n"
 "d: minimum number of digits in the file number\n"
 "z: last part of input file names\n";
 fputs(message, stderr);
}

/* library error */
void error(char *message)
{
 perror(message);
 exit(EXIT_FAILURE);
}

/* internal failure */
void fail(char *message)
{
 fprintf(stderr, "%s\n", message);
 exit(EXIT_FAILURE);
}

/* invalid command line argument */
void invalid(char c)
{
 fprintf(stderr, "argument supplied to -%c is invalid\n", c);
 exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
 int c;
 unsigned f_num = 0, digits = 1;
 char *inn, *outn, *inn_a, *inn_z, *fmt, nc;
 FILE *inf, *outf;
 extern char *optarg;
 extern int opterr, optind, optopt;

 outf = stdout;
 inn = inn_a = inn_z = outn = NULL;
 nc = '\0';

 /* parse the command line */
 while((c = getopt(argc, argv, "hf:a:z:d:")) != -1)
  switch(c)
  {
   case 'h': help(); exit(EXIT_SUCCESS);
   case 'f': outn = optarg; break;
   case 'a': inn_a = optarg; break;
   case 'z': inn_z = optarg; break;
   case 'd': if(sscanf(optarg, "%u", &digits) != 1) invalid(c); break;
   case '?': exit(EXIT_FAILURE);
  }

 /* check input values */
 if(digits < 1) fail("\"d\" must be greater than zero");

 /* setup output file */
 if(outn != NULL)
  if((outf = fopen(outn, "wb")) == NULL)
   error(outn);

 /* if no input name is provided */
 if(inn_a == NULL) inn_a = &nc;
 if(inn_z == NULL) inn_z = &nc;

 /* allocate space for string processing */
 if((inn = malloc(strlen(inn_a) + (sizeof(unsigned) / 2) + strlen(inn_z) + 1)) == NULL) error("allocate input name");
 if((fmt = malloc((sizeof(unsigned) / 2) + 8)) == NULL) error("allocate format string");

 /* prepare format string */
 sprintf(fmt, "%%s%%0%uu%%s", digits);

 /* merge the files */
 while(true)
 {
  /* increment the file number */
  if(!(++f_num)) fail("too many output files");
  /* put together the input file name */
  sprintf(inn, fmt, inn_a, f_num, inn_z);
  /* open the input file */
  if((inf = fopen(inn, "rb")) == NULL) break;

  c = 0;
  /* copy all the bytes */
  while(c != EOF)
  {
   if((c = getc(inf)) == EOF) break;
   if(putc(c, outf) == EOF) error(outn);
  }

  /* close the output file */
  fclose(inf);
 }

 /* free the strings */
 free(inn);
 free(fmt);

 /* close the input file */
 if(outn != NULL)
  fclose(outf);

 return EXIT_SUCCESS;
}
