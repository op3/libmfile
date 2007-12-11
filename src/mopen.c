/*
 * mopen.c:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			xUniversity of Cologne, Germany
 *
 */

#include <stdlib.h>
#include <string.h>
/*#include <sys/file.h>*/
/*#include <sys/stat.h>*/

#include "mfile.h"
#include "maccess.h"
#include "mat_types.h"
#include "sys_endian.h"
#include "mopen.h"



/* $Log: mopen.c,v $
 * Revision 1.12  1993/07/20  17:58:23  se
 * modified for use with access method module
 *
 * Revision 1.11  1992/07/30  12:09:41  se
 * added copyright note
 * added support for comment field
 *
 * Revision 1.10  1992/07/08  21:03:39  se
 * general cleanup for strictest GCC warning levels
 *
 * Revision 1.9  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.8  92/06/10  11:19:02  11:19:02  se (Stefan Esser)
 * changes to reflect mat->specinfo now being union {int, int*}
 * 
 * Revision 1.7  1992/03/31  02:25:59  se
 * now includes string.h instead of strings.h
 *
 * Revision 1.6  1992/03/31  02:04:07  se
 * removed unneccessary definition of setdefaultfiletype()
 *
 * Revision 1.5  1992/03/17  15:28:12  se
 * added global entry mflush to write out all buffers of a dirty matrix
 * doesn't close the file, but tries to make sure that a close would succeed
 *
 * Revision 1.4  1992/02/18  23:43:25  se
 * uses access functions instead of directly accessing the configuration
 * array matproc[].
 *
 * Revision 1.3  1992/02/15  17:22:42  se
 * mopen now accepts matrix format specifier in mode string (eg. "w,1k.le4")
 * OS 9K compatibility
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

/*static void guessfiletype P_((MFILE *mat));*/
static void setmatdefaults P_((MFILE *mat));
static void openmatfile P_((MFILE *mat, char *name, char *mode));

#undef P_

/* include header files for all known format check routines */
#include "lc_minfo.h"
/*#include "oldmat_minfo.h"*/


static void setmatdefaults(MFILE *mat) {

  mat->ap		= NULL;
  mat->name		= NULL;
  mat->comment		= NULL;
  mat->version		= 0;
  mat->status		= 0;

  mat->filetype		= MAT_UNKNOWN;

  mat->levels		= 1;
  mat->lines		= 1;
  mat->columns		= 0;

  mat->mflushf		= NULL;
  mat->muninitf		= NULL;
  mat->mgeti4f		= NULL;
  mat->mgetf4f		= NULL;
  mat->mgetf8f		= NULL;
  mat->mputi4f		= NULL;
  mat->mputf4f		= NULL;
  mat->mputf8f		= NULL;

  mat->specinfo.p	= NULL;
}


static void openmatfile(MFILE *mat, char *name, char *mode) {

  char *accessmode = NULL;

  /* PROVISORISCH !!! 
   *  Test auf (_accessmode_) in Name und Mode noch einrichten !!!
   */

  mat->name = malloc (strlen (name) +1);
  if (mat->name) strcpy (mat->name, name);

  mat->ap = tryaccess (name, mode, accessmode);

  if (!mat->ap) mat->filetype = MAT_INVALID;
}  


MFILE *mopen(char *name, char *mode) {

  MFILE *mat;
  char *fmt;
  
  if (name == NULL || mode == NULL) return NULL;

  mat = (MFILE *) malloc (sizeof (*mat));
  if (mat == NULL) return NULL;

  setmatdefaults (mat);
  openmatfile (mat, name, mode);

  if (mat->filetype != MAT_INVALID) {
    mat->filetype = MAT_UNKNOWN;
  
    if (mat->ap->size != 0) matproc_guessfiletype (mat);
  }

  fmt = strchr (mode, ',');
  if (fmt) msetfmt (mat, fmt +1);

  if (mat->filetype == MAT_INVALID) {
    mclose (mat);
    mat = NULL;
  }
  
  return (mat);
}


int mclose(MFILE *mat) {

  int status = 0;
  
  if (mat) {
    if (mat->muninitf) status = mat->muninitf (mat);
    if (mat->ap)
    {
      if (_close (mat->ap) != 0) status = -1;
      free (mat->ap->name);
      free (mat->ap);
    }

    if (mat->name) free (mat->name);
    free (mat);
  }

  return status;
}

int mflush(MFILE *mat) {

  int status = 0;
  
  if (mat && mat->mflushf) status = mat->mflushf (mat);
  if (mat->ap)
  {
    if (_flush (mat->ap) != 0) status = -1;
  }

  return status;
}

