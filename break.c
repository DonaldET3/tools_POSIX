/* Opal file breaker
 * for Unix
 * version 4
 */


/* pieces section */

#include <errno.h>
/* errno
 */

#include <stdio.h>
/* getc()
 * putc()
 * getchar()
 * putchar()
 * fputs()
 * fprintf()
 * snprintf()
 * sscanf()
 * fopen()
 * fclose()
 * NULL
 * FILE
 * EOF
 */

#include <stdlib.h>
/* exit()
 * realloc()
 * free()
 * NULL
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
	char message[] = "Opal file breaker\n"
	"version 4\n\n"
	"options\n"
	"h: print help and exit\n"
	"s: maximum size of broken output files\n"
	"u: unit scale (K, M, G, T, or P)\n"
	"a: first part of broken file names\n"
	"d: minimum number of digits in the file number\n"
	"z: last part of broken file names\n"
	"r: reconstruction mode\n\n"
	"File to break or merge is sent through a standard input or output stream.\n";
	fputs(message, stderr);
	return;
}

void break_f(unsigned long long int f_size, char *brkn_a, int digits, char *brkn_z)
{
	int c, length, n_size = 0;
	unsigned int f_num = 0;
	unsigned long long int position;
	char *brkn = NULL;
	FILE *brkf;

	if((c = getchar()) == EOF) return;

	/* break the file */
	while(c != EOF)
	{
		/* increment the file number */
		if(!(++f_num)) fail("too many output files");

		/* put together the output file name */
		length = snprintf(brkn, n_size, "%s%0*u%s", brkn_a, digits, f_num, brkn_z) + 1;
		if(length > n_size)
		{
			/* get more space for the string if neccessary */
			if((brkn = realloc(brkn, (size_t)(n_size = length))) == NULL) failed("allocate broken file name");
			snprintf(brkn, n_size, "%s%0*u%s", brkn_a, digits, f_num, brkn_z);
		}

		/* open the output file */
		if((brkf = fopen(brkn, "wb")) == NULL) fail(brkn);

		/* copy no more than the specified number of bytes per file */
		for(position = 0; position < f_size; position++)
		{
			if(putc(c, brkf) == EOF) fail(brkn);
			if((c = getchar()) == EOF) break;
		}

		/* close the output file */
		fclose(brkf);
	}

	free(brkn);

	return;
}

void merge_f(char *brkn_a, int digits, char *brkn_z)
{
	int c, length, n_size = 0;
	unsigned int f_num = 0;
	unsigned long long int position;
	char *brkn = NULL;
	FILE *brkf;

	/* merge the files */
	while(1)
	{
		/* increment the file number */
		if(!(++f_num)) fail("too many output files");

		/* put together the output file name */
		length = snprintf(brkn, n_size, "%s%0*u%s", brkn_a, digits, f_num, brkn_z) + 1;
		if(length > n_size)
		{
			/* get more space for the string if neccessary */
			if((brkn = realloc(brkn, (size_t)(n_size = length))) == NULL) failed("allocate broken file name");
			snprintf(brkn, n_size, "%s%0*u%s", brkn_a, digits, f_num, brkn_z);
		}

		/* open the input file */
		if((brkf = fopen(brkn, "rb")) == NULL) break;

		/* copy all the bytes */
		while((c = getc(brkf)) != EOF)
			if(putchar(c) == EOF)
				failed("write file");

		/* close the input file */
		fclose(brkf);
	}

	free(brkn);

	return;
}

int main(int argc, char **argv)
{
	int c, digits = 1;
	signed int mode = 1;
	char unit = '\0', nc = '\0', *brkn_a = NULL, *brkn_z = NULL;
	char *s_err = "output file size too large";
	unsigned long long int size = 0, check = 0;
	extern char *optarg;
	extern int opterr, optind, optopt;

	/* the errno symbol is defined in errno.h */
	errno = 0;

	/* parse the command line */
	while((c = getopt(argc, argv, "hs:u:a:d:z:r")) != -1)
		switch(c)
		{
			case 'h': help(); exit(EXIT_SUCCESS);
			case 's': if(sscanf(optarg, "%llu", &size) != 1) invalid(c); break;
			case 'u': unit = *optarg; break;
			case 'a': brkn_a = optarg; break;
			case 'd': if(sscanf(optarg, "%d", &digits) != 1) invalid(c); break;
			case 'z': brkn_z = optarg; break;
			case 'r': mode = -1; break;
			case '?': exit(EXIT_FAILURE);
		}

	/* check input values */
	if((mode == 1) && (size < 1)) fail("\"s\" must be greater than zero");
	if(digits < 1) fail("\"d\" must be greater than zero");

	/* multiply size by unit */
	switch(unit)
	{
		case 'k': case 'K': check = size * 1000;
			if((check / 1000) != size) fail(s_err); break;
		case 'm': case 'M': check = size * 1000000L;
			if((check / 1000000L) != size) fail(s_err); break;
		case 'g': case 'G': check = size * 1000000000L;
			if((check / 1000000000L) != size) fail(s_err); break;
		case 't': case 'T': check = size * 1000000000000LL;
			if((check / 1000000000000LL) != size) fail(s_err); break;
		case 'p': case 'P': check = size * 1000000000000000LL;
			if((check / 1000000000000000LL) != size) fail(s_err); break;
		case '\0': break;
		default: fail("unrecognized unit");
	}
	if(check) size = check;

	/* if no output name is provided */
	if(brkn_a == NULL) brkn_a = &nc;
	if(brkn_z == NULL) brkn_z = &nc;

	if(mode == 1) break_f(size, brkn_a, digits, brkn_z);
	else if(mode == -1) merge_f(brkn_a, digits, brkn_z);

	return EXIT_SUCCESS;
}
