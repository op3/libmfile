#include <stdio.h>
#include <sys/file.h>
#include <sys/stat.h>

#include "maccess.h"
#include "disk_access.h"
#include "debug.h"

/* ------------------------------------------------------------------------- */

int disk_get(amp ap, void *buffer, acc_pos offset, acc_pos nbytes) {

  FILE *f = (FILE*) ap->specinfo.p;
  if (fseek (f, (long)offset, SEEK_SET) != 0) {
    PERROR("fseek");
    return -1;
  }

  return fread (buffer, 1, nbytes, f);
}

int disk_put(amp ap, void *buffer, acc_pos offset, acc_pos nbytes) {

  FILE *f = (FILE*) ap->specinfo.p;

  if (fseek (f, (long)offset, SEEK_SET) != 0) {
    PERROR("fseek");
    return -1;
  }

  return fwrite (buffer, 1, nbytes, f);
}

/* ------------------------------------------------------------------------- */

int disk_flush(amp ap) {

  FILE *f = (FILE*) ap->specinfo.p;

  return fflush (f);
}

int disk_close (amp ap) {

  FILE *f = (FILE*) ap->specinfo.p;

  return fclose (f);
}

/* ------------------------------------------------------------------------- */

int disk_tryaccess(amp ap, char *name, char *mode) {

  FILE *f;
  struct stat stat_buf;

  f = fopen (name, mode);
  if (!f) {
    PERROR("fopen");
    return -1;
  }
  ap->specinfo.p = (void*) f;
  ap->get	 = disk_get;
  ap->put	 = disk_put;
  ap->close	 = disk_close;
  ap->flush	 = disk_flush;
  
  if (fstat (fileno(f), &stat_buf) == 0)
  {
    ap->size = stat_buf.st_size;
  }

  return 0;
}

/* ------------------------------------------------------------------------- */

