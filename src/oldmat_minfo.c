/*
 * oldmat_minfo.c:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "mfile.h"
#include "maccess.h"
#include "getputint.h"
#include "oldmat_minfo.h"
#include "oldmat_getput.h"
#include "mat_types.h"

static char rcsid[] = "$Id: oldmat_minfo.c,v 1.14 1993/07/20 17:58:49 se Exp $";

/* $Log: oldmat_minfo.c,v $
 * Revision 1.14  1993/07/20  17:58:49  se
 * modified for use with access method module
 *
 * Revision 1.13  1993/04/22  15:26:28  se
 * Support added for SIGNED 2 byte integer data
 *
 * Revision 1.12  1992/07/30  12:09:41  se
 * added copyright note
 *
 * Revision 1.11  1992/07/25  14:18:10  se
 * now includes string.h instead of strings.h and stdlib.h instead of malloc.h
 * added code to identify triagonal matrizes
 *
 * Revision 1.10  1992/07/08  21:03:39  se
 * general cleanup for strictest GCC warning levels
 *
 * Revision 1.9  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.8  1992/07/08  06:26:05  se
 * now #includes strings.h
 *
 * Revision 1.7  1992/06/10  11:30:35  se
 * moved definition of MAGIC_OLDMAT into this file
 *
 * Revision 1.6  1992/06/10  11:19:17  se
 * introduced version 2 with file format described in trailer, version 1
 * is traditional file format, default is v.1 for integer, v.2 for float data
 *
 * Revision 1.5  1992/03/31  02:06:39  se
 * now includes malloc.h
 *
 * Revision 1.4  1992/02/17  23:19:06  se
 * init code now handles 8 byte float
 *
 * Revision 1.3  1992/02/14  19:13:26  se
 * added initialisation code for floating point matrixes
 *
 * Revision 1.2  1992/02/13  14:37:45  se
 * added rcsid[]
 *
 */

#define TESTBUFSIZE	(4096*4)	/* must be a power of 2 */

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

static void guessdatatype P_((MFILE *mat, u_int pos));
static void guesslinescols P_((MFILE *mat, u_int size));

#undef P_

char MAGIC_OLDMAT[] = "\nMatFmt: ";

static void guessdatatype (mat, pos)
     MFILE *mat;
     u_int pos;
{
  u_char buf[TESTBUFSIZE];
  int nread;
  int n1 = 0, n2 = 0, n3 = 0, n4 = 0;
  int lf4 = 0, lf8 = 0, hf4 = 0, hf8 = 0, vaxf = 0, vaxg = 0;
  
  u_int i, lim4, lim8;
  
  pos &= (-8);
  nread = _get (mat->ap, (char *)buf, pos, sizeof(buf));
  nread &= (-8);

  if (nread <= 0)						return;
  
  for (i = 0; i < nread; i += 4) {
    u_int tli,thi,tvax,e;

    n1 += buf[i];
    n2 += buf[i+1];
    n3 += buf[i+2];
    n4 += buf[i+3];
/* consider floating point number, if within 2^-3 to 2^20 */
#define GETEXP(_n,_bits) (((_n) & 0x7fff) >> (15 - (_bits)))
#define MINEXP (-3)
#define MAXEXP (20)
    tli  = (buf[i+3] << 8) + buf[i+2];
    thi  = (buf[i  ] << 8) + buf[i+1];
    tvax = (buf[i+1] << 8) + buf[i  ];

    e = GETEXP (tli, 8);
    if (127 + MINEXP < e && e < 127 + MAXEXP) lf4++;
    e = GETEXP (thi, 8);
    if (127 + MINEXP < e && e < 127 + MAXEXP) hf4++;
    e = GETEXP (tvax, 8);
    if (127 + MINEXP < e && e < 127 + MAXEXP) vaxf++;

    if (i & 4) {
      e = GETEXP (tli, 11);
      if (1023 + MINEXP < e && e < 1023 + MAXEXP) lf8++;
    } else {
      e = GETEXP (thi, 11);
      if (1023 + MINEXP < e && e < 1023 + MAXEXP) hf8++;
      e = GETEXP (tvax, 11);
      if (1023 + MINEXP < e && e < 1023 + MAXEXP) vaxg++;
    }
#undef GETEXP    
#undef MINEXP
#undef MAXEXP
  }

  lim4 = 3 * (nread >> 4);
  lim8 = lim4 >> 1;

  mat->filetype = 
    n1 > n4 << 3			/* looks like low endian ? */
    ? n3 > n2 << 3
      ? MAT_LE2
      : n1 > n4 << 12 && n2 >= n3 << 2
	? MAT_LE4
	: MAT_UNKNOWN
    : n4 > n1 << 3			/* looks like high endian ? */
      ? n2 > n3 << 3
	? MAT_HE2
	: n4 > n1 << 12 && n3 >= n2 << 2
	  ? MAT_HE4
	  : MAT_UNKNOWN
      : MAT_UNKNOWN;

  if (mat->filetype == MAT_UNKNOWN &&
      (lf4 > lim4) + (hf4 > lim4) + (vaxf > lim4) +
      (lf8 > lim8) + (hf8 > lim8) + (vaxg > lim8) == 1) {
    mat->filetype =
      lf4 > lim4
      ? MAT_LF4
      : hf4 > lim4
	? MAT_HF4
	: vaxf > lim4
	  ? MAT_VAXF
	  : lf8 > lim8
	    ? MAT_LF8
	    : hf8 > lim8
	      ? MAT_HF8
	      : vaxg > lim8
		? MAT_VAXG
		: MAT_UNKNOWN;
  }
}

static void guesslinescols (mat, size)
     MFILE *mat;
     u_int size;
{
  int filetype = mat->filetype;
  
  if (filetype != MAT_INVALID) {
    u_int elems, lines, columns;

    switch (filetype) {
  case MAT_LE2:
  case MAT_HE2:
      elems = size >> 1;
      break;
    
  case MAT_LE4:
  case MAT_HE4:
  case MAT_LF4:
  case MAT_HF4:
  case MAT_VAXF:      
      elems = size >> 2;
      break;

  case MAT_LF8:
  case MAT_HF8:
  case MAT_VAXG:      
      elems = size >> 3;
      break;

  default:
      elems = 0;
      break;
    }

    columns = 0;
    if (elems == 4096 *4096) {
      lines   = 4096;
      columns = 4096;
    } else {
      lines = 1;
      while (lines <= (2<<15)) {
        if (elems == ((lines * (lines+1)) >> 1)) {
          columns = lines;
	  switch (filetype) {
	case MAT_LE2: { mat->filetype = MAT_LE2T; break; }
	case MAT_LE4: { mat->filetype = MAT_LE4T; break; }
	case MAT_HE2: { mat->filetype = MAT_HE2T; break; }
	case MAT_HE4: { mat->filetype = MAT_HE4T; break; }
	default:      { columns = 0;		  break; }
	  }
	  break;
	}
	lines <<= 1;
      }
      if (columns == 0) {
	lines   = 1;
	columns = elems;
      }
    }

    mat->lines      = lines;
    mat->columns    = columns;
    mat->version    = OLDMAT_STD_VERSION;
  }
}

static void checkformagic (mat, size)
     MFILE *mat;
     int   size;
{
  oldmat_header	omh;
  unsigned int s = sizeof (omh);
  unsigned int l = strlen (MAGIC_OLDMAT);

  if (size < s || _get (mat->ap, (char *) omh, size - s, s) != s) return;

  if (strncmp (omh, MAGIC_OLDMAT, l) != 0) return;
  msetfmt (mat, omh + l);
}

void oldmat_probe (mat)
     MFILE *mat;
{
  u_int size = mat->ap->size;

  checkformagic (mat, size);
  if (mat->filetype != MAT_UNKNOWN) return;

  guessdatatype (mat, (size /3) & -TESTBUFSIZE);
  if (mat->filetype == MAT_INVALID) return;

  guesslinescols (mat, size);
}

void oldmat_init (mat)
     MFILE *mat;
{
  if (0 < mat->columns && mat->columns <= MAT_COLMAX) {
    int filetype = mat->filetype;
    int datatype = matproc_datatype (filetype);
    int elemsize = datatype & MAT_D_SIZE;
    mgetf* getf  = matproc_getf (filetype);
    mputf* putf  = matproc_putf (filetype);
    
    mat->specinfo.i = elemsize;
    mat->version = OLDMAT_STD_VERSION;

    switch (datatype) {
  case MAT_D_I2U:
      mat->mgeti4f		= getf;
      mat->mputi4f		= putf;
      break;
  case MAT_D_I2S:
      mat->mgeti4f		= getf;
      mat->mputi4f		= putf;
      break;
  case MAT_D_I4S: 
      mat->mgeti4f		= getf;
      mat->mputi4f		= putf;
      break;
  case MAT_D_F4:
      mat->mgetf4f		= getf;
      mat->mputf4f		= putf;
      mat->version		= 2;
      break;
  case MAT_D_F8:
      mat->mgetf8f		= getf;
      mat->mputf8f		= putf;
      mat->version		= 2;
      break;
  default:
      return;
      break;
    }

    mat->muninitf		= oldmat_uninit;
  }
}

int oldmat_uninit (mat)
     MFILE *mat;
{
  if ((mat->status & MST_DIRTY) == 0) return 0;

  if (mat->version == 2)
  {
    oldmat_header omh;
    unsigned int elemsize = mat->specinfo.i;
    unsigned int matsize  = mat->levels * mat->lines * mat->columns * elemsize;

    if (matsize == 0) return 0;
    
    sprintf (omh, "%s%s\n", MAGIC_OLDMAT, mgetfmt (mat, NULL));

    if (_put (mat->ap, (char *) omh, matsize, sizeof (omh)) 
	!= sizeof (omh)) return -1;
  }
  return 0;
}

int oldmat_putinfo (mat, info)
     MFILE *mat;
     minfo *info;
{
  return 0;
}
