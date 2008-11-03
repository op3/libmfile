/*
 * lc_getput.c
 */
/*
 * Copyright (c) 1992-2008, Stefan Esser <se@ikp.uni-koeln.de>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *	* Redistributions of source code must retain the above copyright notice, 
 *	  this list of conditions and the following disclaimer.
 * 	* Redistributions in binary form must reproduce the above copyright notice, 
 * 	  this list of conditions and the following disclaimer in the documentation 
 * 	  and/or other materials provided with the distribution.
 *    
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
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
static void verifycompr(lc_minfo *lci, int *line, int num) {

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

static int readline(MFILE *mat, int *buffer, u_int line) {

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

static int writeline(MFILE *mat, int *buffer, u_int line) {

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

static void trycacheline(MFILE *mat, u_int line) {

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

int lc_get(MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num) {

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

int lc_put(MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num) {

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


int lc_flushcache(MFILE *mat) {

  lc_minfo *lci = (lc_minfo *) mat->specinfo.p;

  if (lci->cachedlinedirty
      && writeline (mat, lci->linebuf, lci->cachedline) != mat->columns) {
    return -1;
  }
  lci->cachedlinedirty = 0;
  return 0;
}



