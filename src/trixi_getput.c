/*
 * trixi_getput.c:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <string.h>

#include "mfile.h"
#include "getputint.h"
#include "trixi_getput.h"

static char rcsid[] = "$Id: trixi_getput.c,v 1.2 1993/07/20 18:00:12 se Exp $";

/* $Log: trixi_getput.c,v $
 * Revision 1.2  1993/07/20  18:00:12  se
 * modified for use with access method module
 *
 * Revision 1.1  1992/07/30  12:09:41  se
 * Initial revision
 *
 *
 */


#define fpos(s) (((level * mat->lines + line) * mat->columns + col) * (s) +512)

int trixi_get(MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num) {

  return getle2 (mat->ap, buffer, fpos (2), num);
}
