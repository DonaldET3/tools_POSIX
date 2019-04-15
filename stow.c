/* Opal file stower
 * for Unix
 * version 2
 */


/* pieces section */

#include <errno.h>
/* errno
 */

#include <stdio.h>
/* getchar()
 * putchar()
 * fputs()
 * fprintf()
 * sscanf()
 * NULL
 * EOF
 */

#include <stdlib.h>
/* exit()
 * malloc()
 * free()
 * NULL
 * EXIT_FAILURE
 * EXIT_SUCCESS
 */

#include <string.h>
/* strerror_l()
 */

#include <stdint.h>
/* uint8_t
 */

#include <locale.h>
/* uselocale()
 */

#include <unistd.h>
/* getopt()
 */


/* definitions section */

/* "ODS1" */
uint8_t magic[] = {0x4F, 0x44, 0x53, 0x31, 0x00};


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
 char message[] = "Opal file stower\n"
 "version 2\n\n"
 "options\n"
 "h: print help and exit\n"
 "b: buffer size (default: 65535 bytes) (not used in retrieve mode)\n"
 "r: retrieve (reverse)\n\n"
 "Input is received from standard input and sent to standard output.\n";
 fputs(message, stderr);
 return;
}

/* write verifiable byte */
void vw_byte(int x)
{
 if(putchar(x &= 0xFF) == EOF) failed("write first verifiable byte");
 if(putchar(x ^ 0xF0) == EOF) failed("write second verifiable byte");
 if(putchar(x ^ 0x0F) == EOF) failed("write third verifiable byte");
 return;
}

/* verify read byte */
int vr_byte()
{
 int x, y, z;

 if((x = getchar()) == EOF) failed("read first verifiable byte");
 if((y = getchar()) == EOF) failed("read second verifiable byte");
 if((z = getchar()) == EOF) failed("read third verifiable byte");

 y ^= 0xF0; z ^= 0x0F;

 if((x != y) || (x != z) || (y != z)) fputs("error found\n", stderr);

 if((x == y) || (x == z)) return x;
 if(y == z) return y;

 failed("correct byte");
}

/* write two byte number */
void vw_word(unsigned int x)
{
 vw_byte((int)((x >> 8) & 0xFF));
 vw_byte((int)(x & 0xFF));
 return;
}

/* read two byte number */
unsigned int vr_word()
{
 unsigned int x;

 x = vr_byte() << 8;

 return x | vr_byte();
}

/* write header */
void w_header()
{
 int i;

 /* magic string */
 for(i = 0; i < 5; i++) vw_byte(magic[i]);

 /* version number */
 vw_byte(1);

 return;
}

/* read header */
void r_header()
{
 int i;

 /* magic string */
 for(i = 0; i < 5; i++)
  if(vr_byte() != magic[i])
   fail("unrecognized file type");

 /* version number */
 if(vr_byte() != 1) fail("incompatible file version");

 return;
}

/* stow data */
void stow(unsigned int buf_siz)
{
 int byte;
 unsigned int level, index;
 uint8_t *buffer;

 if((buffer = malloc(buf_siz)) == NULL) failed("allocate buffer");

 /* write header */
 w_header();

 do
 {
  /* load buffer */
  for(level = 0; level < buf_siz; level++)
  {
   if((byte = getchar()) == EOF) break;
   buffer[level] = byte;
  }

  /* write block header */
  vw_word(level);

  /* write block data */
  for(index = 0; index < level; index++)
   if(putchar(buffer[index]) == EOF)
    failed("write block");
 } while(byte != EOF);

 /* append file terminator */
 if(level != 0) vw_word(0);

 free(buffer);

 return;
}

/* retrieve data */
void retrieve()
{
 int byte;
 unsigned int index, blk_siz;

 /* read header */
 r_header();

 /* process blocks */
 while(blk_siz = vr_word())
  for(index = 0; index < blk_siz; index++)
  {
   if((byte = getchar()) == EOF) failed("read block");
   if(putchar(byte) == EOF) failed("write data");
  }

 return;
}

int main(int argc, char **argv)
{
 int c;
 signed int direction = 1;
 unsigned int buf_siz = 0xFFFF;
 extern char *optarg;
 extern int opterr, optind, optopt;

 /* the errno symbol is defined in errno.h */
 errno = 0;

 /* parse command line */
 while((c = getopt(argc, argv, "hb:r")) != -1)
  switch(c)
  {
   case 'h': help(); exit(EXIT_SUCCESS);
   case 'b': if(sscanf(optarg, "%u", &buf_siz) != 1) invalid(c); break;
   case 'r': direction = -1; break;
   case '?': exit(EXIT_FAILURE);
  }

 /* check input values */
 if(buf_siz < 1) fail("\"b\" must be greater than zero");
 if(buf_siz > 0xFFFF) fail("\"b\" must be less than 65536");

 /* process stream */
 if(direction == 1) stow(buf_siz);
 else if(direction == -1) retrieve();

 return EXIT_SUCCESS;
}
