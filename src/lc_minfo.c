/*
 * lc_minfo.c:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			xUniversity of Cologne, Germany
 *
 */

#include <stdlib.h>
#include <memory.h>

#include "mfile.h"
#include "maccess.h"
#include "sys_endian.h"
#include "getputint.h"
#include "lc_minfo.h"
#include "lc_getput.h"
#include "lc_c1.h"
#include "lc_c2.h"

static char rcsid[] = "$Id: lc_minfo.c,v 1.13 1993/07/20 17:56:36 se Exp $";

/* $Log: lc_minfo.c,v $
 * Revision 1.13  1993/07/20  17:56:36  se
 * modified for use with access method module
 *
 * Revision 1.12  1992/07/30  12:09:41  se
 * added copyright note
 *
 * Revision 1.11  1992/07/08  21:03:39  se
 * general cleanup for strictest GCC warning levels
 *
 * Revision 1.10  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.9  92/06/30  14:49:04  14:49:04  se (Stefan Esser)
 * removed references to lc3_compress etc.
 * 
 * Revision 1.8  1992/06/10  11:13:34  se
 * changes to reflect mat->specinfo now being union {int, int*}
 *
 * Revision 1.7  1992/05/13  23:32:12  se
 * *** empty log message ***
 *
 * Revision 1.6  1992/03/31  01:59:46  se
 * corrected calculation of lci->freepos
 * added check for file corruption due to wrong lci->freepos
 *
 * Revision 1.5  1992/03/17  15:20:46  se
 * complete rewrite to improve signalling of error conditions
 * init_lci now allocates heap space in the way required by free_lci
 * new subroutine lc_flush to allow flushing cashed data to disk
 *
 * Revision 1.4  1992/02/19  19:16:53  se
 * set MST_DIMSFIXED and MST_INITIALIZED in mat->status
 * check MST_INITIALIZED instead of MST_DIMSFIXED in init_lci()
 *
 * Revision 1.3  1992/02/18  23:28:59  se
 * init_lci simply returns if MST_DIMSFIXED already set
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

static int init_lci P_((MFILE *mat, u_int freepos, u_int freelistpos, u_int poslentablepos));
/* static int lc_updateheader P_((MFILE *mat)); */
static void free_lci P_((MFILE *mat));
static int lc_flush P_((MFILE *mat));

#undef P_

static int init_lci(MFILE *mat, unsigned int freepos, 
		    unsigned int freelistpos, unsigned int poslentablepos) {

  u_int n = mat->lines * mat->levels;

  lc_minfo *lci = (lc_minfo *)malloc (sizeof (lc_minfo));

  mat->specinfo.p = (void *)lci;

  if (lci) {
    lci->version		= mat->version;
    lci->cachedline		= -1;
    lci->cachedcomprline	= -1;
    lci->comprlinelen		= 0;
    lci->cachedlinedirty	= 0;
    lci->linebuf		= NULL;
    lci->comprlinebuf		= NULL;

    switch (lci->version) {

 case LC_C1_VERSION:
      lci->comprf	= lc1_compress;
      lci->uncomprf	= lc1_uncompress;
      lci->comprlinebuf	= (void *)malloc (lc1_comprlinelenmax (mat->columns));
      break;

 case LC_C2_VERSION:
      lci->comprf	= lc2_compress;
      lci->uncomprf	= lc2_uncompress;
      lci->comprlinebuf	= (void *)malloc (lc2_comprlinelenmax (mat->columns));
      break;

    }

    lci->linebuf	= (int *)malloc (mat->columns * sizeof(int));
    lci->poslentableptr	= (lc_poslen *)malloc (n * sizeof(lc_poslen));
  
    if (lci->poslentableptr && lci->linebuf && lci->comprlinebuf) {
  
      if (freepos != 0) {
	lci->poslentablepos	= poslentablepos;
        lci->freepos		= freepos;
	lci->freelistpos	= freelistpos;
        if (getle4 (mat->ap, (int *)lci->poslentableptr, 
		    poslentablepos, 2 * n) == 2 * n) {
	  lc_poslen lpc;
	  lpc.len = lci->poslentableptr[0].len;
	  lpc.pos = lci->poslentableptr[0].pos;
	  if (lpc.len && lpc.pos < sizeof (lc_header) + n * sizeof(lc_poslen))
	    return -1;
	  return 0;
	}
      } else {
	lci->poslentablepos	= sizeof (lc_header);
        lci->freepos		= lci->poslentablepos + n * sizeof(lc_poslen);
	lci->freelistpos	= 0;
        memset (lci->poslentableptr, 0, n * sizeof(lc_poslen));
	return 0;
      }
    }
  }

  return -1;
}

void lc_probe(MFILE *mat) {

  lc_header lch;
  
  if (_get (mat->ap, &lch, 0, sizeof(lch)) != sizeof(lch))	return;

  if (lch.magic != GETLE4((unsigned)MAGIC_LC))				return;

  mat->status		|=MST_DIMSFIXED;
  mat->filetype		= MAT_LC;
  mat->version		= GETLE4(lch.version);

  mat->levels		= GETLE4(lch.levels);
  mat->lines		= GETLE4(lch.lines);
  mat->columns		= GETLE4(lch.columns);

  mat->mgeti4f		= lc_get;
  mat->mputi4f		= lc_put;
  mat->mflushf		= lc_flush;
  mat->muninitf		= lc_uninit;

  if (init_lci (mat, GETLE4(lch.freepos), 
			GETLE4(lch.freelistpos), 
			GETLE4(lch.poslentablepos)) != 0) free_lci (mat);
  if (mat->specinfo.p) mat->status |= (MST_INITIALIZED | MST_DIMSFIXED);
}

void lc_init(MFILE *mat) {

  if (mat->status & MST_INITIALIZED) return;

  if (mat->version == 0) {
    mat->version = LC_STD_VERSION;
  }

  if (init_lci (mat, 0, 0, 0) != 0) {
    free_lci (mat);
    mat->filetype = MAT_INVALID;
    return;
  }  

  mat->mgeti4f		= lc_get;
  mat->mputi4f		= lc_put;
  mat->mflushf		= lc_flush;
  mat->muninitf		= lc_uninit;
}


#ifdef undef
int lc_putinfo(MFILE *mat, minfo *info) {
  return 0;
}
#endif


int lc_uninit(MFILE *mat) {

  int status;

  status = lc_flush (mat);
  free_lci (mat);

  return status;
}  


static int lc_flush(MFILE *mat) {

  if (mat->status & MST_DIRTY) {
    lc_header lch;
    lc_minfo *lci = (lc_minfo *)mat->specinfo.p;
    unsigned int n;
  
    if (lc_flushcache (mat) !=0) return -1;

    lch.magic		= GETLE4((unsigned)MAGIC_LC);

    lch.levels		= GETLE4(mat->levels);
    lch.lines		= GETLE4(mat->lines);
    lch.columns		= GETLE4(mat->columns);

    lch.version		= GETLE4(lci->version);
    lch.poslentablepos	= GETLE4(lci->poslentablepos);
    lch.freepos		= GETLE4(lci->freepos);
    lch.freelistpos	= GETLE4(lci->freelistpos);
    lch.used		= 0;			/* not yet implemented */
    lch.free		= 0;
    lch.status		= 0;

    if (_put (mat->ap, &lch, 0, sizeof(lch)) != sizeof(lch)) return -1;

    n = mat->levels * mat->lines;
    if (putle4 (mat->ap, (int *)lci->poslentableptr, lci->poslentablepos, 2*n) != 2*n) return -1;
    if (_flush (mat->ap) != 0) return -1;
    mat->status &= ~MST_DIRTY;
  }
  return 0;
}


static void free_lci(MFILE *mat) {

  if (mat != NULL) {
    lc_minfo *lci = (lc_minfo *)mat->specinfo.p;
    if (lci->linebuf != NULL)		free (lci->linebuf);
    if (lci->comprlinebuf != NULL)	free (lci->comprlinebuf);
    if (lci->poslentableptr != NULL)	free (lci->poslentableptr);
    free (lci);
  }
  mat->filetype = MAT_INVALID;
}


