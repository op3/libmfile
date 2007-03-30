/*
 * txt_getput.c:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <stdlib.h>
#include "mfile.h"
#include "maccess.h"
#include "txt_minfo.h"
#include "txt_getput.h"

static char rcsid[] = "$Id: txt_getput.c,v 1.5 1993/07/20 18:00:20 se Exp $";

/* $Log: txt_getput.c,v $
 * Revision 1.5  1993/07/20  18:00:20  se
 * modified for use with access method module
 *
 * Revision 1.4  1992/07/30  12:09:41  se
 * added copyright note
 *
 * Revision 1.3  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.2  92/06/10  11:22:04  11:22:04  se (Stefan Esser)
 * changes to reflect mat->specinfo now being union {int, int*}
 * 
 * Revision 1.1  1992/05/13  23:39:40  se
 * Initial revision
 *
 */


int txt_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  double *dblp = (double *) mat->specinfo.p;

/*  if (dblp == NULL) return -1; */

  int idx = ((level * mat->lines) + line) * mat->columns + col;

  memcpy (buffer, dblp +idx, num * sizeof (double));
  return num;
}

int txt_put (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  double *dblp = (double *) mat->specinfo.p;

/*  if (dblp == NULL) return -1; */

  int idx = ((level * mat->lines) + line) * mat->columns + col;

  memcpy (dblp +idx, buffer, num * sizeof (double));
  return num;
}

int txt_flush (mat)
     MFILE *mat;
{
  if ((mat->status & MST_DIRTY) != 0) 
  {
    double *dblp = (double *) mat->specinfo.p;
    int maxnum = mat->levels * mat->lines * mat->columns;
    int i;
    /* PROVISORISCH !!! */
    FILE *outf = (FILE*) mat->ap->specinfo.p;

    if (mat->version == 1)
    {
      fprintf (outf, "%s%s\n", TXT_MAGIC, mgetfmt (mat, NULL));
    }
    for (i = 0; i < maxnum; i++) 
    {
      if (fprintf (outf, "%G\n", *dblp++) < 0) return -1;
    }
    if (fflush (outf) != 0) return -1;
    mat->status &= ~MST_DIRTY;
  }
  return 0;
}
