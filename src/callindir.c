/*
 * callindir.c:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include "mfile.h"
#include "callindir.h"
#include "converters.h"
#include "mat_types.h"
#include "mopen.h"

static char rcsid[] = "$Id: callindir.c,v 1.11 1992/07/30 12:09:41 se Exp $";

/* $Log: callindir.c,v $
 * Revision 1.11  1992/07/30  12:09:41  se
 * added copyright note
 *
 * Revision 1.10  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.9  92/03/31  01:53:38  01:53:38  se (Stefan Esser)
 * added includes of converters.h and mat_types.h
 * 
 * Revision 1.8  1992/03/17  15:11:37  se
 * added paramok makro to check for validity of parameters
 *
 * Revision 1.7  1992/02/18  23:23:29  se
 * changed according to changes in mat_types.c
 *
 * Revision 1.6  1992/02/17  23:13:27  se
 * now handles reding/writing of int/float/double data into any supported
 * format.
 *
 * Revision 1.5  1992/02/17  17:52:56  se
 * now calls installconverters() to convert between different internal
 * represenattions (int, float, double)
 *
 * Revision 1.4  1992/02/15  17:22:42  se
 * name change: mget -> mgetint for consistency reasons ...
 *
 * Revision 1.3  1992/02/14  19:22:46  se
 * added hooks for floating point spectra and matrixes
 *
 * Revision 1.2  1992/02/13  14:37:45  se
 * added rcsid[]
 *
 */


#define paramok(mat, buffer, level, line, col, num)			    \
					(				    \
					 mat &&				    \
					 buffer &&			    \
					 (u_int)level < mat->levels	&&  \
					 (u_int)line  < mat->lines	&&  \
					 (u_int)col   < mat->columns	&&  \
					 (u_int)num   <= mat->columns	&&  \
					 (u_int)(col + num) <= mat->columns \
					)

int mgetint (mat, buffer, level, line, col, num)
     MFILE *mat;
     int level, line, col, num;
     int *buffer;
{
  int (*f)();

  /* sanity checks */
  if (paramok(mat, buffer, level, line, col, num)) {

    if ((f = mat->mgeti4f)) return f (mat, buffer, level, line, col, num);
    matproc_init (mat);
    installconverters (mat);
    if ((f = mat->mgeti4f)) return f (mat, buffer, level, line, col, num);
  }
  return -1;
}

int mputint (mat, buffer, level, line, col, num)
     MFILE *mat;
     int level, line, col, num;
     int *buffer;
{
  int (*f)();

  /* sanity checks */
  if (paramok(mat, buffer, level, line, col, num)) {

    mat->status |= (MST_DIRTY | MST_DIMSFIXED);

    if ((f = mat->mputi4f)) return f (mat, buffer, level, line, col, num);
    if (mat->filetype == MAT_UNKNOWN) mat->filetype = MAT_STD_INT;
    matproc_init (mat);
    installconverters (mat);
    if ((f = mat->mputi4f)) return f (mat, buffer, level, line, col, num);
  }
  return -1;
}

/*------------------------------------------------------------------------*/

int mgetflt (mat, buffer, level, line, col, num)
     MFILE *mat;
     int level, line, col, num;
     float *buffer;
{
  int (*f)();

  /* sanity checks */
  if (paramok(mat, buffer, level, line, col, num)) {

    if ((f = mat->mgetf4f)) return f (mat, buffer, level, line, col, num);
    matproc_init (mat);
    installconverters (mat);
    if ((f = mat->mgetf4f)) return f (mat, buffer, level, line, col, num);
  }
  return -1;
}

int mputflt (mat, buffer, level, line, col, num)
     MFILE *mat;
     int level, line, col, num;
     float *buffer;
{
  int (*f)();

  /* sanity checks */
  if (paramok(mat, buffer, level, line, col, num)) {

    mat->status |= (MST_DIRTY | MST_DIMSFIXED);

    if ((f = mat->mputf4f)) return f (mat, buffer, level, line, col, num);
    if (mat->filetype == MAT_UNKNOWN) mat->filetype = MAT_STD_FLT;
    matproc_init (mat);
    installconverters (mat);
    if ((f = mat->mputf4f)) return f (mat, buffer, level, line, col, num);
  }
  return -1;
}

/*------------------------------------------------------------------------*/

int mgetdbl (mat, buffer, level, line, col, num)
     MFILE *mat;
     int level, line, col, num;
     double *buffer;
{
  int (*f)();

  /* sanity checks */
  if (paramok(mat, buffer, level, line, col, num)) {

    if ((f = mat->mgetf8f)) return f (mat, buffer, level, line, col, num);
    matproc_init (mat);
    installconverters (mat);
    if ((f = mat->mgetf8f)) return f (mat, buffer, level, line, col, num);
  }
  return -1;
}

int mputdbl (mat, buffer, level, line, col, num)
     MFILE *mat;
     int level, line, col, num;
     double *buffer;
{
  int (*f)();

  /* sanity checks */
  if (paramok(mat, buffer, level, line, col, num)) {

    mat->status |= (MST_DIRTY | MST_DIMSFIXED);

    if ((f = mat->mputf8f)) return f (mat, buffer, level, line, col, num);
    if (mat->filetype == MAT_UNKNOWN) mat->filetype = MAT_STD_DBL;
    matproc_init (mat);
    installconverters (mat);
    if ((f = mat->mputf8f)) return f (mat, buffer, level, line, col, num);
  }
  return -1;
}

