/*
   Copyright (C) 2001-2018 Free Software Foundation, Inc.
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.
   
   Written by Michael Meskes 
   Certain elements duplicated and simplified by Adam McCartney.
      
*/

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

/* stat.c: replica of the unix program stat
 * usage: stat <file or directory>
 *
 * */

/* stat the file and print what we find */
static bool ATTRIBUTE_WARN_UNUSED_RESULT
do_stat (char const *filename, char const *format,
         char const *format2)
{
  int fd = STREQ (filename, "-") ? 0 : -1;
  struct stat statbuf;

  if (0 <= fd)
    {
      if (fstat (fd, &statbuf) != 0)
        {
          error (0, errno, _("cannot stat standard input"));
          return false;
        }
    }
  /* We can't use the shorter
     (follow_links?stat:lstat) (filename, &statbug)
     since stat might be a function-like macro.  */
  else if ((follow_links
            ? stat (filename, &statbuf)
            : lstat (filename, &statbuf)) != 0)
    {
      error (0, errno, _("cannot stat %s"), quoteaf (filename));
      return false;
    }

  if (S_ISBLK (statbuf.st_mode) || S_ISCHR (statbuf.st_mode))
    format = format2;

  bool fail = print_it (format, fd, filename, print_stat, &statbuf);
  return ! fail;
}

int main(int argc, char* argv[]) 
{
    struct stat *s;

    if (argc != 2) {
        printf("Usage: stat <file or directory>\n");
    }

    if (argc == 2) {

        const char *restrict path = argv[1];
        char buf[PATH_MAX]; 

        char *res = realpath(path, buf);
        stat(res, s);
        printf("File: %s\n", argv[1]);
        printf("Size: %ld\t", s->st_size);
        printf("Blocks: %ld\t", s->st_blocks);
        printf("IO Blocks: %ld\t", s->st_blksize);
        if (S_ISREG(s->st_mode)) {
            printf("regular file");
        }
        printf("\n");
    }
    return 0;
}
