/*
 * shm_getput.c:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#ifndef NO_SHM

#include <stdlib.h>
#include <string.h>
#include "mfile.h"
#include "shm_minfo.h"
#include "shm_getput.h"



/* $Log: shm_getput.c,v $
 * Revision 1.7  1992/12/31  10:59:06  se
 * nothing will be compiled if NO_SHM is defined
 *
 * Revision 1.6  1992/07/30  12:09:41  se
 * added copyright note
 *
 * Revision 1.5  1992/07/08  21:03:39  se
 * general cleanup for strictest GCC warning levels
 *
 * Revision 1.4  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.3  92/06/10  11:21:03  11:21:03  se (Stefan Esser)
 * changes to reflect mat->specinfo now being union {int, int*}
 * 
 * Revision 1.2  1992/02/13  14:37:45  se
 * added rcsid[]
 *
 */


int shm_get(MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num) {

  int *p = (int *) mat->specinfo.p;

  memcpy (buffer, p + col, num * sizeof(buffer[0]));
  return num;
}

#endif /* NO_SHM */
