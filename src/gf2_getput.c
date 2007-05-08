/*
 * gf2_getput.c:	derived by Nigel Warr (April 2003) from
 *                      oldmat_getput.c which is:
 *                      Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 *  This file provides support for Radware gf2 files.
 */

#include <string.h>

#include "mfile.h"
#include "getputint.h"
#include "gf2_getput.h"
#include "gf2_minfo.h"
#include "mat_types.h"

static char rcsid[] = "$Id: gf2_getput.c,v 1.0 2003/04/16 08:28:21 se Exp $";

#define fpos(s) (36 + ((level * mat->lines + line) * mat->columns + col) * (s))

/*----------------------------------------------------------------------*/

/* Get from gf2 format file */
int gf2_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     float *buffer;
{
   if (mat->filetype == MAT_GF2)
     return getle4 (mat->ap, (int *)buffer, fpos (4), num);
   else
     return gethe4 (mat->ap, (int *)buffer, fpos (4), num);
}

/* Put to gf2 format file */
int gf2_put (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     float *buffer;
{

   return putle4 (mat->ap, (int *)buffer, fpos(4), num);
}

