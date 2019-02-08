/* Opal file stower
 * 16-bit
 * for Unix
 * version 1
 */


/* pieces section */

#include <stdio.h>
/* getc()
 * putc()
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

#include <stdint.h>
/* uint8_t
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
 char *message = "Opal file stower\n"
 "version 1, 16-bit\n\n"
 "options\n"
 "h: print help and exit\n"
 "i: specify input file (default: standard input)\n"
 "o: specify output file (default: standard output)\n"
 "b: block and buffer size (default: 65535 bytes) (not used in retrieve mode)\n"
 "r: retrieve (reverse)\n";
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

/* write big-endian 16-bit word */
void write_word(unsigned x, FILE *fp)
{
 if(putc(0xFF & (x >> 8), fp) == EOF) error("failed to write word");
 if(putc(0xFF & x, fp) == EOF) error("failed to write word");

 return;
}

/* read big-endian 16-bit word */
unsigned read_word(FILE *fp)
{
 int c;
 unsigned x = 0;

 if((c = getc(fp)) == EOF) error("failed to read word");
 x = ((unsigned)c) << 8;
 if((c = getc(fp)) == EOF) error("failed to read word");
 x |= c;

 return x;
}

void stow(FILE *inf, FILE *outf, unsigned bs)
{
 int c = 0;
 unsigned i, level;
 uint8_t *buf;

 buf = malloc(bs);

 while(c != EOF)
 {
  for(level = 0; level < bs; level++)
  {
   if((c = getc(inf)) == EOF) break;
   buf[level] = c;
  }

  write_word(level, outf);

  for(i = 0; i < level; i++)
   if(fputc(buf[i], outf) == EOF)
    error("failed to write byte");
 }

 write_word(0, outf);

 free(buf);

 return;
}

void retrieve(FILE *inf, FILE *outf)
{
 int c;
 unsigned i;

 while(i = read_word(inf))
  while(i--)
  {
   if((c = getc(inf)) == EOF) fail("failed to read byte");
   if(putc(c, outf) == EOF) fail("failed to write byte");
  }

 return;
}

int main(int argc, char **argv)
{
 int c, direction = 1;
 unsigned bs = 0xFFFF;
 char *inn, *outn;
 FILE *inf, *outf;
 extern char *optarg;
 extern int opterr, optind, optopt;

 inn = NULL; outn = NULL;
 inf = stdin; outf = stdout;

 /* parse the command line */
 while((c = getopt(argc, argv, "hi:o:b:r")) != -1)
  switch(c)
  {
   case 'h': help(); exit(EXIT_SUCCESS);
   case 'i': inn = optarg; break;
   case 'o': outn = optarg; break;
   case 'b': if(sscanf(optarg, "%u", &bs) != 1) invalid(c); break;
   case 'r': direction = -1; break;
   case '?': exit(EXIT_FAILURE);
  }

 /* check input values */
 if(bs < 1) fail("\"b\" must be greater than zero");
 if(bs > 0xFFFF) fail("\"b\" must be less than 65536");

 /* open input file */
 if(inn != NULL)
  if((inf = fopen(inn, "rb")) == NULL)
   error("open input file");

 /* open output file */
 if(outn != NULL)
  if((outf = fopen(outn, "wb")) == NULL)
   error("open output file");

 /* process stream */
 if(direction == 1) stow(inf, outf, bs);
 else if(direction == -1) retrieve(inf, outf);

 /* close input file */
 if(inn != NULL)
  fclose(inf);

 /* close output file */
 if(outn != NULL)
  fclose(outf);

 return EXIT_SUCCESS;
}
