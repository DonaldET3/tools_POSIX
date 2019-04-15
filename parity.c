/* Opal parity generator
 * for Unix
 * version 1
 */


/* pieces section */

#include <errno.h>
/* errno
 */

#include <stdio.h>
/* getchar()
 * putchar()
 * putc()
 * getc()
 * fputs()
 * fprintf()
 * sscanf()
 * fopen()
 * fclose()
 * getline()
 * perror()
 * NULL
 * EOF
 */

#include <stdlib.h>
/* exit()
 * calloc()
 * free()
 * NULL
 * EXIT_FAILURE
 * EXIT_SUCCESS
 */

#include <string.h>
/* strtok()
 * strerror_l()
 */

#include <stdint.h>
/* uint8_t
 * uintmax_t
 */

#include <locale.h>
/* uselocale()
 */

#include <unistd.h>
/* getopt()
 */

#include <sys/stat.h>
/* stat()
 * struct stat
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
 char message[] = "Opal parity generator\n"
 "version 1\n\n"
 "options\n"
 "h: print help and exit\n"
 "s: maximum number of bytes to output\n\n"
 "File names are read from standard input.\n"
 "Parity data is sent to standard output.\n";
 fputs(message, stderr);
 return;
}

void generate(uintmax_t output_size)
{
 int b;
 char *fn, *string = NULL;
 uintmax_t data_size = 0, i;
 uint8_t *data = NULL, *new_space;
 size_t s_size = 0;
 FILE *fp;
 struct stat fsmd;

 /* for each file name */
 while(getline(&string, &s_size, stdin) != -1)
 {
  /* trim newline off */
  fn = strtok(string, "\n");

  /* find file length */
  if(stat(fn, &fsmd) == -1)
  {perror(fn); continue;}

  /* resize state */
  if(fsmd.st_size > data_size)
  {
   if((new_space = calloc((size_t)fsmd.st_size, 1)) == NULL) failed("allocate space");
   for(i = 0; i < data_size; i++) new_space[i] = data[i];
   free(data);
   data = new_space;
   data_size = fsmd.st_size;
  }

  /* add file */
  fp = fopen(fn, "rb");
  for(i = 0; i < data_size; i++)
  {
   if((b = getc(fp)) == EOF) break;
   data[i] ^= b;
  }
  fclose(fp);
 }

 /* output parity data */
 if(!output_size) output_size = UINTMAX_MAX;
 for(i = 0; (i < output_size) && (i < data_size); i++)
  if(putchar(data[i]) == EOF)
   failed("output data");

 free(string);

 return;
}

int main(int argc, char **argv)
{
 int c;
 unsigned long long int output_size = 0;
 extern char *optarg;
 extern int opterr, optind, optopt;

 /* the errno symbol is defined in errno.h */
 errno = 0;

 /* parse command line */
 while((c = getopt(argc, argv, "hs:")) != -1)
  switch(c)
  {
   case 'h': help(); exit(EXIT_SUCCESS);
   case 's': if(sscanf(optarg, "%llu", &output_size) != 1) invalid(c); break;
   case '?': exit(EXIT_FAILURE);
  }

 /* process files */
 generate((uintmax_t)output_size);

 return EXIT_SUCCESS;
}
