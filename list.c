/* Opal List
 * for Unix
 * version 1.1
 */


/* pieces section */

#include <errno.h>
/* errno
 */

#include <stdio.h>
/* size_t
 * NULL
 * EOF
 * getc()
 * putc()
 * fputs()
 * printf()
 * fprintf()
 * sprintf()
 * perror()
 */

#include <stdlib.h>
/* size_t
 * NULL
 * EXIT_FAILURE
 * EXIT_SUCCESS
 * exit()
 * malloc()
 * realloc()
 * free()
 */

#include <string.h>
/* strlen()
 * strcpy()
 * strcmp()
 * strerror_l()
 */

#include <stdint.h>
/* uintmax_t
 */

#include <stdbool.h>
/* bool
 * true
 * false
 */

#include <dirent.h>
/* DIR
 * struct dirent
 * opendir()
 * readdir()
 * closedir()
 */

#include <locale.h>
/* uselocale()
 */

#include <unistd.h>
/* getopt()
 */

#include <sys/stat.h>
/* struct stat
 * stat()
 * S_ISREG()
 * S_ISDIR()
 */


/* definitions section */

struct write_type {
 bool file;
 bool dir;
 bool size;
};

/* directory record */
struct dir_rec {
 char *path;
 ino_t *node_list;
 size_t node_count;
 struct dir_rec *next;
};


/* functions section */

/* print error message and quit */
void fail(char *message)
{
 /* print error message */
 fputs(message, stderr);
 /* elaborate on the error if possible */
 if(errno) fprintf(stderr, ": %s", strerror_l(errno, uselocale(0)));
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
 char message[] = "Opal List\n"
 "version 1.1\n\n"
 "options\n"
 "h: print help and exit\n"
 "f: list regular file paths\n"
 "d: list directory paths\n"
 "s: prepend file size in bytes to each file path\n";
 fputs(message, stderr);
 return;
}

struct dir_rec * dr_remove(struct dir_rec *c_dir)
{
 struct dir_rec *n_dir;

 n_dir = c_dir->next;
 free(c_dir->path);
 free(c_dir->node_list);
 free(c_dir);

 return n_dir;
}

void proc_dir(char *origin, struct write_type *write)
{
 char *f_path = NULL;
 size_t i, pre_len, com_len, siz = 0;
 struct dir_rec *c_dir, *l_dir;
 struct dirent *dir_e;
 struct stat fsmd;
 DIR *dp;

 /* get directory metadata */
 if(stat(origin, &fsmd) == -1)
 {perror(origin); return;}

 /* write starting directory */
 if(write->dir)
  if(strcmp(origin, "."))
   printf("%s\n", origin);

 /* add first directory record */
 if((l_dir = c_dir = malloc(sizeof(struct dir_rec))) == NULL) failed("allocate first directory record");
 if((c_dir->path = malloc(strlen(origin) + 1)) == NULL) failed("allocate first directory path");
 strcpy(c_dir->path, origin);
 if((c_dir->node_list = malloc(sizeof(ino_t))) == NULL) failed("allocate first inode list");
 c_dir->node_list[0] = fsmd.st_ino;
 c_dir->node_count = 1;
 c_dir->next = NULL;

 /* process directory tree */
 while(c_dir != NULL)
 {
  /* open directory */
  if((dp = opendir(c_dir->path)) == NULL)
  {
   perror(c_dir->path);
   c_dir = dr_remove(c_dir);
   continue;
  }

  /* prefix length */
  pre_len = strlen(c_dir->path);

  /* for each directory record */
  while((dir_e = readdir(dp)) != NULL)
  {
   /* ignore the current and parent directories */
   if((!strcmp(dir_e->d_name, ".")) || (!strcmp(dir_e->d_name, ".."))) continue;

   com_len = pre_len + strlen(dir_e->d_name) + 2;
   if(siz < com_len)
    if((f_path = realloc(f_path, siz = com_len)) == NULL)
     failed("allocate file path");

   /* put together path */
   if(strcmp(c_dir->path, ".")) sprintf(f_path, "%s/%s", c_dir->path, dir_e->d_name);
   else strcpy(f_path, dir_e->d_name);

   /* get file metadata */
   if(stat(f_path, &fsmd) == -1)
   {perror(f_path); continue;}

   /* if regular file */
   if(S_ISREG(fsmd.st_mode))
   {
    if(write->size) printf("%ju %s\n", (uintmax_t)fsmd.st_size, f_path);
    else if(write->file) printf("%s\n", f_path);
   }
   /* if directory */
   else if(S_ISDIR(fsmd.st_mode))
   {
    if(write->dir) printf("%s\n", f_path);

    /* check inodes */
    for(i = 0; i < c_dir->node_count; i++)
     if(dir_e->d_ino == c_dir->node_list[i])
      fail("infinite hardlink loop");

    /* add directory record */
    if((l_dir->next = malloc(sizeof(struct dir_rec))) == NULL) failed("allocate directory record");
    l_dir = l_dir->next;
    if((l_dir->path = malloc(com_len)) == NULL) failed("allocate directory path");
    strcpy(l_dir->path, f_path);
    l_dir->node_count = c_dir->node_count + 1;
    if((l_dir->node_list = malloc(l_dir->node_count * sizeof(ino_t))) == NULL) failed("allocate inode list");
    for(i = 0; i < c_dir->node_count; i++)
     l_dir->node_list[i] = c_dir->node_list[i];
    l_dir->node_list[c_dir->node_count] = dir_e->d_ino;
    l_dir->next = NULL;
   }
  }

  closedir(dp);
  c_dir = dr_remove(c_dir);
 }

 free(f_path);

 return;
}

void make_list(char **fnames, int ind, struct write_type *write)
{
 struct stat fsmd;

 /* for each command line entry */
 for(; fnames[ind] != NULL; ind++)
 {
  /* get file metadata */
  if(stat(fnames[ind], &fsmd) == -1)
  {perror(fnames[ind]); continue;}

  /* if regular file */
  if(S_ISREG(fsmd.st_mode))
  {
   if(write->size) printf("%ju %s\n", (uintmax_t)fsmd.st_size, fnames[ind]);
   else if(write->file) printf("%s\n", fnames[ind]);
  }
  /* if directory */
  else if(S_ISDIR(fsmd.st_mode)) proc_dir(fnames[ind], write);
 }

 return;
}

int main(int argc, char **argv)
{
 int c;
 extern char *optarg;
 extern int opterr, optind, optopt;
 struct write_type write;

 write.file = false;
 write.dir = false;
 write.size = false;

 /* the errno symbol is defined in errno.h */
 errno = 0;

 /* parse command line */
 while((c = getopt(argc, argv, "hfds")) != -1)
  switch(c)
  {
   case 'h': help(); exit(EXIT_SUCCESS);
   case 'f': write.file = true; break;
   case 'd': write.dir = true; break;
   case 's': write.size = true; break;
   case '?': exit(EXIT_FAILURE);
  }

 make_list(argv, optind, &write);

 return EXIT_SUCCESS;
}
