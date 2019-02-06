/* Opal file breaker
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
 char *message = "Opal file breaker\n"
 "version 2\n\n"
 "options\n"
 "h: print help and exit\n"
 "f: specify file to break (default: standard input)\n"
 "s: maximum size of each output file (required)\n"
 "u: unit scale (K, M, G, T, or P)\n"
 "a: first part of output file names\n"
 "d: minimum number of digits in the file number\n"
 "z: last part of output file names\n";
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
 long long unsigned size = 0, position;
 char unit, *inn, *outn, *outn_a, *outn_z, *fmt, nc;
 FILE *inf, *outf;
 extern char *optarg;
 extern int opterr, optind, optopt;

 inf = stdin;
 inn = outn_a = outn_z = outn = NULL;
 unit = nc = '\0';

 /* parse the command line */
 while((c = getopt(argc, argv, "hs:u:f:a:z:d:")) != -1)
  switch(c)
  {
   case 'h': help(); exit(EXIT_SUCCESS);
   case 's': if(sscanf(optarg, "%llu", &size) != 1) invalid(c); break;
   case 'u': unit = *optarg; break;
   case 'f': inn = optarg; break;
   case 'a': outn_a = optarg; break;
   case 'z': outn_z = optarg; break;
   case 'd': if(sscanf(optarg, "%u", &digits) != 1) invalid(c); break;
   case '?': exit(EXIT_FAILURE);
  }

 /* check input values */
 if(size < 1) fail("\"s\" must be greater than zero");
 if(digits < 1) fail("\"d\" must be greater than zero");

 /* multiply size by unit */
 switch(unit)
 {
  case 'k': size *= 1000; break;
  case 'K': size *= 1000; break;
  case 'm': size *= 1000000; break;
  case 'M': size *= 1000000; break;
  case 'g': size *= 1000000000; break;
  case 'G': size *= 1000000000; break;
  case 't': size *= 1000000000000; break;
  case 'T': size *= 1000000000000; break;
  case 'p': size *= 1000000000000000; break;
  case 'P': size *= 1000000000000000; break;
  case '\0': break;
  default: fail("unrecognized unit");
 }

 /* setup input file */
 if(inn != NULL)
  if((inf = fopen(inn, "rb")) == NULL)
   error(inn);

 /* if no output name is provided */
 if(outn_a == NULL) outn_a = &nc;
 if(outn_z == NULL) outn_z = &nc;

 /* allocate space for string processing */
 if((outn = malloc(strlen(outn_a) + (sizeof(unsigned) / 2) + strlen(outn_z) + 1)) == NULL) error("allocate output name");
 if((fmt = malloc((sizeof(unsigned) / 2) + 8)) == NULL) error("allocate format string");

 /* prepare format string */
 sprintf(fmt, "%%s%%0%uu%%s", digits);

 c = 0;
 /* break the file */
 while(c != EOF)
 {
  /* increment the file number */
  if(!(++f_num)) fail("too many output files");
  /* put together the output file name */
  sprintf(outn, fmt, outn_a, f_num, outn_z);
  /* open the output file */
  if((outf = fopen(outn, "wb")) == NULL) error(outn);

  /* copy no more than the specified number of bytes per file */
  for(position = 0; position < size; position++)
  {
   if((c = getc(inf)) == EOF) break;
   if(putc(c, outf) == EOF) error(outn);
  }

  /* close the output file */
  fclose(outf);
 }

 /* free the strings */
 free(outn);
 free(fmt);

 /* close the input file */
 if(inn != NULL)
  fclose(inf);

 return EXIT_SUCCESS;
}
