/* Opal Hash 1, 64-bit
 * for Unix
 * version 1
 */


/* pieces section */

#include <errno.h>
/* errno
 */

#include <stdio.h>
/* getc()
 * fputs()
 * sscanf()
 * printf()
 * fprintf()
 * getline()
 * perror()
 * fopen()
 * fclose()
 * FILE
 * NULL
 * EOF
 * size_t
 */

#include <stdlib.h>
/* exit()
 * free()
 * NULL
 * EXIT_FAILURE
 * EXIT_SUCCESS
 * size_t
 */

#include <string.h>
/* strtok()
 * strerror_l()
 */

#include <stdint.h>
/* uint64_t
 */

#include <locale.h>
/* uselocale()
 */

#include <unistd.h>
/* getopt()
 */


/* definitions section */

/* hash state */
struct hash_s {
 int pos;
 uint64_t hash;
};


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

/* help message */
void help()
{
 char message[] = "Opal Hash 1, 64-bit\n"
 "version 1\n\n"
 "options\n"
 "h: print help and exit\n"
 "l: list mode\n"
 "c: check mode\n";
 fputs(message, stderr);
 return;
}

uint64_t oh1_round(uint64_t hash)
{
 /* increment to avoid zero state */
 hash++;
 /* multiply by the prime number 65537 (10001 in hexadecimal) */
 hash += hash << 16;
 /* shift the state down by one less than half of the bit width,
  * and XOR that into the state */
 hash ^= hash >> 31;
 return hash;
}

void oh1_new(struct hash_s *hs)
{
 /* initialize state to zero */
 hs->pos = 0;
 hs->hash = 0;
 return;
}

void oh1_add(struct hash_s *hs, int b)
{
 int i;

 /* add byte into state */
 hs->hash += ((int64_t)b) << ((3 - hs->pos) * 8);

 /* if half of the state has been filled */
 if(++hs->pos == 4)
 {
  /* mix the hash state */
  for(i = 0; i < 3; i++) hs->hash = oh1_round(hs->hash);
  /* reset position */
  hs->pos = 0;
 }

 return;
}

uint64_t oh1_fin(struct hash_s *hs)
{
 int i;

 /* add terminating bits */
 hs->hash += ((int64_t)0x80) << ((3 - hs->pos) * 8);
 /* final mix */
 for(i = 0; i < 3; i++) hs->hash = oh1_round(hs->hash);

 return hs->hash;
}

/* hash stream */
void hash_stream(FILE *fp, struct hash_s *hs)
{
 int c;

 while((c = getc(fp)) != EOF) oh1_add(hs, c);

 return;
}

/* hash input data */
void hash_input()
{
 struct hash_s hs;

 /* new hash */
 oh1_new(&hs);

 /* hash data stream */
 hash_stream(stdin, &hs);

 /* write hash */
 printf("%016llX\n", (unsigned long long int)oh1_fin(&hs));

 return;
}

/* hash input files */
void hash_files()
{
 char *fn = NULL;
 size_t siz = 0;
 FILE *fp;
 struct hash_s hs;

 while(getline(&fn, &siz, stdin) != -1)
 {
  if((fp = fopen(strtok(fn, "\n"), "rb")) == NULL)
  {
   perror(fn);
   continue;
  }
  oh1_new(&hs);
  hash_stream(fp, &hs);
  printf("%016llX %s\n", (unsigned long long int)oh1_fin(&hs), fn);
  fclose(fp);
 }

 free(fn);

 return;
}

void check_files()
{
 unsigned int fail_count = 0;
 unsigned long long int p_hash;
 char *line = NULL, *fn;
 size_t siz = 0;
 FILE *fp;
 struct hash_s hs;

 while(getline(&line, &siz, stdin) != -1)
 {
  sscanf(strtok(line, " "), "%llX", &p_hash);
  if((fp = fopen(fn = strtok(NULL, "\n"), "rb")) == NULL)
  {
   perror(fn);
   continue;
  }
  oh1_new(&hs);
  hash_stream(fp, &hs);
  if(p_hash == oh1_fin(&hs)) printf("%s: OK\n", fn);
  else
  {
   printf("%s: FAILED\n", fn);
   fail_count++;
  }
  fclose(fp);
 }

 if(fail_count) fprintf(stderr, "%u check(s) failed\n", fail_count);

 free(line);

 return;
}

int main(int argc, char **argv)
{
 int c, mode = 0;
 extern char *optarg;
 extern int opterr, optind, optopt;

 /* the errno symbol is defined in errno.h */
 errno = 0;

 /* parse command line */
 while((c = getopt(argc, argv, "hlc")) != -1)
  switch(c)
  {
   case 'h': help(); exit(EXIT_SUCCESS);
   case 'l': mode = 1; break;
   case 'c': mode = 2; break;
   case '?': exit(EXIT_FAILURE);
  }

 if(mode == 0) hash_input();
 else if(mode == 1) hash_files();
 else if(mode == 2) check_files();

 return EXIT_SUCCESS;
}
