/*
 * shm_minfo.c:		Copyright 1992 by 
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

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "mfile.h"
#include "getputint.h"
#include "shm_minfo.h"
#include "shm_getput.h"

static char rcsid[] = "$Id: shm_minfo.c,v 1.9 1993/07/20 17:59:23 se Exp $";

/* $Log: shm_minfo.c,v $
 * Revision 1.9  1993/07/20  17:59:23  se
 * obsolete, has been replaced by shm_access access methods
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


#ifdef undef
void shm_probe (mat)
     MFILE *mat;
{
  char buf[128], *p;
  int shmid;
  struct shmid_ds shm_stat;
  void *shm_addr;
  int shm_size;
  
  static char shm_magic[] = "shared memory (shm_alloc)\nid=";

  memset (buf, 0, sizeof(buf));
  if (get (mat->fd, buf, 0, sizeof(buf)) < sizeof(shm_magic))	return;
  if (strncmp (buf, shm_magic, sizeof (shm_magic) -1) != 0)	return;

  p = buf + sizeof(shm_magic) -1;
  while (isspace(*p)) p++;
  shmid = 0;
  while (isdigit(*p)) shmid = shmid * 10 + (*p++ - '0');

  shm_addr = (void *) shmat (shmid, NULL, SHM_RDONLY);
  if ((int)shm_addr == -1)					return;

  if (shmctl (shmid, IPC_STAT, &shm_stat) != 0)			return;
  shm_size = shm_stat.shm_segsz;

  mat->status	       |= MST_DIMSFIXED;
  mat->filetype		= MAT_SHM;

  mat->levels		= 1;
  mat->lines		= 1;
  mat->columns		= shm_size / sizeof(int);

  mat->mgeti4f		= shm_get;
  mat->mputi4f		= NULL;
  mat->muninitf		= shm_uninit;

  mat->specinfo.p	= shm_addr;
}

void shm_init (mat)
     MFILE *mat;
{
  mat->filetype = MAT_INVALID;
}


int shm_putinfo (mat, info)
     MFILE *mat;
     minfo* info;
{
  return 0;
}


int shm_uninit (mat)
     MFILE *mat;
{
  /* 
    not yet implemented:
    if shmctl() --> shm_attach == 1: 
       try to delete shm segment at close !
       no error, if we don't have permission to do this.
   */
  return shmdt ((char *)mat->specinfo.p);
}  

#endif /* NO_SHM */
#endif /*undef*/
