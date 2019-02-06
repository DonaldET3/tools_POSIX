/* Opal binary to hexadecimal converter
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
 char *message = "Opal binary to hexadecimal converter\n"
 "version 2\n\n"
 "options\n"
 "h: print help and exit\n"
 "w: bytes per line (default: 26)\n"
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

/* binary quartet to hexadecimal digit */
int b2h(int b)
{
 switch(b & 0xF)
 {
  case 0x0: return '0';
  case 0x1: return '1';
  case 0x2: return '2';
  case 0x3: return '3';
  case 0x4: return '4';
  case 0x5: return '5';
  case 0x6: return '6';
  case 0x7: return '7';
  case 0x8: return '8';
  case 0x9: return '9';
  case 0xA: return 'A';
  case 0xB: return 'B';
  case 0xC: return 'C';
  case 0xD: return 'D';
  case 0xE: return 'E';
  case 0xF: return 'F';
 }
}

/* convert byte from binary to hexadecimal */
bool conv_byte(FILE *inf, FILE *outf)
{
 int c;

 if((c = getc(inf)) == EOF) return false;
 if(putc(b2h(c >> 4), outf) == EOF) error("write first byte digit");
 if(putc(b2h(c), outf) == EOF) error("write second byte digit");
 return true;
}

int main(int argc, char **argv)
{
 int i, c, w;
 char *inn, *outn;
 FILE *inf, *outf;
 extern char *optarg;
 extern int opterr, optind, optopt;

 i = 0; w = 26;
 inn = NULL; outn = NULL;
 inf = stdin; outf = stdout;

 /* parse the command line */
 while((c = getopt(argc, argv, "hw:i:o:")) != -1)
  switch(c)
  {
   case 'h': help(); exit(EXIT_SUCCESS);
   case 'w': if(sscanf(optarg, "%d", &w) != 1) invalid(c); break;
   case 'i': inn = optarg; break;
   case 'o': outn = optarg; break;
   case '?': exit(EXIT_FAILURE);
  }

 if(w < 1)
  fail("\"w\" must be greater than zero");

 if(inn != NULL)
  if((inf = fopen(inn, "rb")) == NULL)
   error("open input file");

 if(outn != NULL)
  if((outf = fopen(outn, "wb")) == NULL)
   error("open output file");

 while(conv_byte(inf, outf))
 {
  i++;

  if(i < w)
  {
   if(putc(' ', outf) == EOF) error("write space");
  }
  else
  {
   if(putc('\n', outf) == EOF) error("write newline");
   i = 0;
  }
 }

 if(inn != NULL)
  fclose(inf);

 if(outn != NULL)
  fclose(outf);

 return EXIT_SUCCESS;
}
