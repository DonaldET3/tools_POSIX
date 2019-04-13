/* Opal hexadecimal converter
 * for Unix
 * version 3
 */


/* pieces section */

#include <errno.h>
/* errno
 */

#include <stdio.h>
/* putc()
 * getchar()
 * putchar()
 * fputs()
 * printf()
 * scanf()
 * fprintf()
 * sscanf()
 * EOF
 */

#include <stdlib.h>
/* exit()
 * EXIT_FAILURE
 * EXIT_SUCCESS
 */

#include <string.h>
/* strerror_l()
 */

#include <locale.h>
/* uselocale()
 * locale_t
 */

#include <unistd.h>
/* getopt()
 */


/* functions section */

/* print error message and quit */
void fail(char *message)
{
 /* print error message */
 fputs(message, stderr);
 /* elaborate on the error if possible */
 if(errno) fprintf(stderr, ": %s", strerror_l(errno, uselocale((locale_t)0)));
 putc('\n', stderr);
 exit(EXIT_FAILURE);
}

/* "failed to" <error message> and quit */
void failed(char *message)
{
 /* prepend "failed to" to the error message */
 fputs("failed to ", stderr);
 fail(message);
}

/* invalid command line argument */
void invalid(char c)
{
 fprintf(stderr, "argument supplied to -%c is invalid\n", c);
 exit(EXIT_FAILURE);
}

/* help message */
void help()
{
 char message[] = "Opal hexadecimal converter\n"
 "version 3\n\n"
 "default operation: convert binary bytes to hexadecimal text\n\n"
 "options\n"
 "h: print help and exit\n"
 "x: convert hexadecimal text to binary bytes\n"
 "w: hexadecimal digit pairs to write per line (default: 26)\n\n"
 "This program reads from standard input and writes to standard output.\n";
 fputs(message, stderr);
 return;
}

/* convert a stream from binary to hexadecimal */
void bth(int w)
{
 int i = 0, c;

 /* convert the bytes */
 while((c = getchar()) != EOF)
 {
  if(printf("%02X", (unsigned int)c) != 2) failed("write byte");

  if(++i < w)
  {
   /* insert spaces between the digit pairs */
   if(putchar(' ') == EOF) failed("write space");
  }
  else
  {
   /* put a newline after every "w" digit pairs */
   if(putchar('\n') == EOF) failed("write newline");
   i = 0;
  }
 }

 /* end the stream with a newline */
 if(i) if(putchar('\n') == EOF) failed("write newline");

 return;
}

/* convert a stream from hexadecimal to binary */
void htb()
{
 unsigned int b;

 /* get hexadecimal numbers; output binary bytes */
 while(scanf("%2X", &b) != EOF)
  if(putchar((int)b) == EOF)
   failed("write byte");

 return;
}

int main(int argc, char **argv)
{
 int c, w = 26;
 signed int mode = 1;
 extern char *optarg;
 extern int opterr, optind, optopt;

 /* the errno symbol is defined in errno.h */
 errno = 0;

 /* parse the command line */
 while((c = getopt(argc, argv, "hxw:")) != -1)
  switch(c)
  {
   case 'h': help(); exit(EXIT_SUCCESS);
   case 'x': mode = -1; break;
   case 'w': if(sscanf(optarg, "%d", &w) != 1) invalid(c); break;
   case '?': exit(EXIT_FAILURE);
  }

 /* check input values */
 if(w < 1) fail("\"w\" must be greater than zero");

 /* enter mode */
 if(mode == 1) bth(w);
 else if(mode == -1) htb();

 return EXIT_SUCCESS;
}
