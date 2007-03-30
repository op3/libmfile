/*
 * lc_getput.c:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <errno.h>

#include "mfile.h"
#include "maccess.h"
#include "getputint.h"
#include "lc_minfo.h"
#include "lc_c1.h"
#include "lc_c2.h"
#include "lc_getput.h"

static char rcsid[] = "$Id: lc_getput.c,v 1.12 1993/07/20 17:56:15 se Exp $";

/* $Log: lc_getput.c,v $
 * Revision 1.12  1993/07/20  17:56:15  se
 * modified for use with access method module
 *
 * Revision 1.11  1992/07/30  12:09:41  se
 * added copyright note
 *
 * Revision 1.10  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.9  92/07/08  06:23:10  06:23:10  se (Stefan Esser)
 * prepended explicit cast to void* to 2nd parameter of get and put
 * 
 * Revision 1.8  1992/06/30  14:48:32  se
 * removed forced definition of VERIFY_COMPRESSION
 *
 * Revision 1.7  1992/06/10  11:12:04  se
 * changes to reflect mat->specinfo now being union {int, int*}
 *
 * Revision 1.6  1992/05/13  23:30:35  se
 * put back in the call to verifycompr() which was erroneously deleted
 * at the time of the last major changes
 *
 * Revision 1.5  1992/03/31  02:25:31  se
 * now includes stdio.h and memory.h
 *
 * Revision 1.4  1992/03/31  01:58:17  se
 * removed unneccessary definition of lc_alloc()
 *
 * Revision 1.3  1992/03/17  15:12:36  se
 * complete rewrite to allow detection and signalling of write errors
 * lc_flush renamed to lc_flushcache, now returns int
 * tryreadcomprline replaced by readline, returns int
 * trywritecomprline replaced by writeline, returns int
 * deleted flushcacheline
 *
 * Revision 1.2  1992/02/13  14:37:45  se
 * added rcsid[]
 *
 */

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

/* static int lc_alloc P_((MFILE *mat, int n)); */
static int readline P_((MFILE *mat, int *buffer, u_int line));
static int writeline P_((MFILE *mat, int *buffer, u_int line));
static void trycacheline P_((MFILE *mat, u_int line));

#undef P_

#ifdef VERIFY_COMPRESSION
static void verifycompr (lci, line, num)
     lc_minfo *lci;
     int *line;
     int num;
{
  static int uncline[MAT_COLMAX];

  if (num > MAT_COLMAX) return;

  uncline[num-1] = line[num-1] -1;
  
  if ((lci->uncomprf (uncline, lci->comprlinebuf, num) != num) ||
      (memcmp (uncline, line, num * sizeof (int)) != 0)	) {
    u_int c;
    for (c = 0; c < num; c++) {
      if (uncline[c] != line[c]) {
        fprintf (stderr, "\ncompression error, aborting !!!\ncol = %d,  %d (correct %d)\n", c, uncline[c], line[c]);
	abort ();
      }
    }
  }
}
#endif /* VERIFY_COMPRESSION */

static int readline (mat, buffer, line)
     MFILE *mat;
     int *buffer;
     u_int line;
{
  lc_minfo *lci = (lc_minfo *) mat->specinfo.p;
  
  if (lci->cachedcomprline != line) {
    amp ap = mat->ap;
    lc_poslen *poslentable = lci->poslentableptr;

    u_int l		   = poslentable[line].len;
    u_int p		   = poslentable[line].pos;

    if (l == 0) return 0;

    if (_get (ap, (void *) lci->comprlinebuf, p, l) == l) {
      lci->comprlinelen	   = l;
      lci->cachedcomprline = line;
    }
  }
  if (lci->cachedcomprline == line) {
    return lci->uncomprf (buffer, lci->comprlinebuf, mat->columns);
  }
  return -1;
}

static int writeline (mat, buffer, line)
     MFILE *mat;
     int *buffer;
     u_int line;
{
  amp ap		  = mat->ap;
  lc_minfo *lci		  = (lc_minfo *) mat->specinfo.p;

  lc_poslen *poslentable  = lci->poslentableptr;

  u_int p		  = poslentable[line].pos;
  u_int l		  = poslentable[line].len;
  
  u_int fp		  = lci->freepos;
  u_int nl = lci->comprf (lci->comprlinebuf, buffer, mat->columns);
#ifdef VERIFY_COMPRESSION
  verifycompr (lci, buffer, mat->columns);
#endif
  if (nl > 0) {
    if (l + p == fp)	  { fp = p; l = 0; }
    if (l < nl)		  { l = nl; p = fp; fp += l; }

    if (_put (ap, (void *) lci->comprlinebuf, p, l) == l) {
      lci->freepos		= fp;
      poslentable[line].len	= l;
      poslentable[line].pos	= p;
      return mat->columns;
    }
  }
  return -1;
}

static void trycacheline (mat, line)
     MFILE *mat;
     u_int line;
{
  lc_minfo *lci = (lc_minfo *) mat->specinfo.p;
  
  if (lci->cachedline != line) {
    if (lci->cachedlinedirty
	&& writeline (mat, lci->linebuf, lci->cachedline) == mat->columns) {
      lci->cachedlinedirty = 0;
    }
    if (!lci->cachedlinedirty
	&& readline (mat, lci->linebuf, line) == mat->columns) {
      lci->cachedline = line;
    }
  }
}

int lc_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  lc_minfo *lci = (lc_minfo *) mat->specinfo.p;

  line += level * mat->lines;

  if (num != mat->columns) {
    trycacheline (mat, line);
  }

  if (line == lci->cachedline) {
    memcpy (buffer, lci->linebuf + col, num * sizeof (int));
    return num;
  }
  if (num == mat->columns) {
    return readline (mat, buffer, line);
  }

  return -1;
}

int lc_put (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  lc_minfo *lci = (lc_minfo *) mat->specinfo.p;

  line += level * mat->lines;

  if (num == mat->columns) {
    return writeline (mat, buffer, line);
  }
  if (lci->cachedline != line) {
    trycacheline (mat, line);
    if (lci->cachedline != line && !lci->cachedlinedirty) {
      memset (lci->linebuf, 0, mat->columns * sizeof (int));
      lci->cachedline = line;
    }
  }
  if (lci->cachedline == line) {
    lci->cachedlinedirty = 1;
    memcpy (lci->linebuf + col, buffer, num * sizeof (int));
    return num;
  }

  return -1;
}


int lc_flushcache (mat)
     MFILE *mat;
{
  lc_minfo *lci = (lc_minfo *) mat->specinfo.p;

  if (lci->cachedlinedirty
      && writeline (mat, lci->linebuf, lci->cachedline) != mat->columns) {
    return -1;
  }
  lci->cachedlinedirty = 0;
  return 0;
}



