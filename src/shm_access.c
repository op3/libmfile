/*
 * shm_access.c:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */


#ifndef NO_SHM

#include <stdlib.h>
#include <errno.h>
#include <memory.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "mfile.h"
#include "maccess.h"
#include "shm_access.h"

static char rcsid[] = "$Id: shm_access.c,v 1.2 1997/04/23 14:03:48 root Exp $";

/* $Log: shm_access.c,v $
 * Revision 1.2  1997/04/23  14:03:48  root
 * Error removed in function shm_tryaccess.
 * File will be closed now.
 *
 * Revision 1.1  1993/07/20  17:58:54  se
 * Initial revision
 *
 * Revision 1.8  1992/12/31  10:59:06  se
 * nothing will be compiled if NO_SHM is defined
 *
 * Revision 1.7  1992/07/30  12:09:41  se
 * added copyright note
 *
 * Revision 1.6  1992/07/25  14:19:42  se
 * now includes string.h instead of strings.h
 *
 * Revision 1.5  1992/07/08  21:03:39  se
 * general cleanup for strictest GCC warning levels
 *
 * Revision 1.4  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.3  92/06/10  11:21:24  11:21:24  se (Stefan Esser)
 * changes to reflect mat->specinfo now being union {int, int*}
 * 
 * Revision 1.2  1992/02/13  14:37:45  se
 * added rcsid[]
 *
 */

/* ------------------------------------------------------------------------- */

static void *shm_geta(amp ap, acc_pos offset, acc_pos nbytes) {

  char *p = (char *) ap->specinfo.p;

  if (offset + nbytes > ap->size) return NULL;
  return p + offset;
}

static int shm_close(amp ap) {
  /* 
    not yet implemented:
    if shmctl() --> shm_attach == 1: 
       try to delete shm segment at close !
       no error, if we don't have permission to do this.
   */
  return shmdt ((char *)ap->specinfo.p);
}  

/* ------------------------------------------------------------------------- */

int shm_tryaccess(amp ap, char *name, char *mode) {

  FILE *f;
  char buf[128], *p;
  int shmid;
  struct shmid_ds shm_stat;
  void *shm_addr;
  int shm_size;
  
  static char shm_magic[] = "shared memory (shm_alloc)\nid=";

  memset (buf, 0, sizeof(buf));

  f = fopen (name, "r");
  if (!f) return -1;

  if (fread (buf, 1, sizeof (buf), f) < sizeof(shm_magic)) {
    fclose(f);
    return -1;
  }

  if (strncmp (buf, shm_magic, sizeof (shm_magic) -1) != 0) {
    fclose(f);
    return -1;
  }

  p = buf + sizeof(shm_magic) -1;
  while (isspace(*p)) p++;
  shmid = 0;
  while (isdigit(*p)) shmid = shmid * 10 + (*p++ - '0');

  shm_addr = (void *) shmat (shmid, NULL, SHM_RDONLY);
  if ((int)shm_addr == -1) {
    fclose(f);
    return -1;
  }

  if (shmctl (shmid, IPC_STAT, &shm_stat) != 0)	{
    fclose(f);
    return -1;
  }

  shm_size = shm_stat.shm_segsz;

  ap->size		= shm_size;
  ap->specinfo.p	= shm_addr;

  ap->geta		= shm_geta;
/*  ap->puta		= shm_puta;*/

  ap->close		= shm_close;

  return 0;
}

#endif /* NO_SHM */
