/*
 * mate_getput.c:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include "mfile.h"
#include "getputint.h"
#include "mate_getput.h"

static char rcsid[] = "$Id: mate_getput.c,v 1.3 1993/07/20 17:58:06 se Exp $";

/* $Log: mate_getput.c,v $
 * Revision 1.3  1993/07/20  17:58:06  se
 * modified for use with access method module
 *
 * Revision 1.2  1992/07/30  12:09:41  se
 * added copyright note
 *
 * Revision 1.1  1992/03/12  17:05:43  se
 * Initial revision
 *
 */


#define fpos(s) (((level * mat->lines + line) * mat->columns + col) * (s))

int mate_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  int nread = getle4 (mat->ap, buffer, fpos (4) + 0x200, num);
  int i;

  for (i = 0; i < nread; i++) {
    buffer[i] &= 0xffffff;
  }
  return nread;
}
