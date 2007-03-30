/*
 * oldmat_getput.c:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <string.h>

#include "mfile.h"
#include "getputint.h"
#include "oldmat_getput.h"

static char rcsid[] = "$Id: oldmat_getput.c,v 1.9 1993/07/20 17:58:45 se Exp $";

/* $Log: oldmat_getput.c,v $
 * Revision 1.9  1993/07/20  17:58:45  se
 * modified for use with access method module
 *
 * Revision 1.8  1993/04/22  15:26:28  se
 * Support added for SIGNED 2 byte integer data
 *
 * Revision 1.7  1992/07/30  12:09:41  se
 * added copyright note
 *
 * Revision 1.6  1992/07/25  14:16:20  se
 * added subroutines to get and put data from triagonal matrizes
 *
 * Revision 1.5  1992/03/31  02:05:47  se
 * added casts to (int *) to calls of {get,put}{le,he}{4,8}()
 *
 * Revision 1.4  1992/02/17  23:15:32  se
 * added functions to read low and high endian 8 byte float matrixes
 *
 * Revision 1.3  1992/02/14  19:05:34  se
 * first implementation of (trivial) floating point spectra and matrices
 *
 * Revision 1.2  1992/02/13  14:37:45  se
 * added rcsid[]
 *
 */


#define fpos(s) (((level * mat->lines + line) * mat->columns + col) * (s))

#define tri_pos(l,c) (( (l * (l+1)) >> 1) + c)

#define fpos_t(s) ((level * tri_pos(mat->lines,0) + tri_pos(line, col)) * (s))

int le4_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  return getle4 (mat->ap, buffer, fpos (4), num);
}

int le4_put (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  return putle4 (mat->ap, buffer, fpos (4), num);
}

int le2_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  return getle2 (mat->ap, buffer, fpos (2), num);
}

int le2_put (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  return putle2 (mat->ap, buffer, fpos (2), num);
}

int le2s_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  return getle2s (mat->ap, buffer, fpos (2), num);
}

int he4_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  return gethe4 (mat->ap, buffer, fpos (4), num);
}

int he4_put (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  return puthe4 (mat->ap, buffer, fpos (4), num);
}

int he2_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  return gethe2 (mat->ap, buffer, fpos (2), num);
}

int he2s_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  return gethe2s (mat->ap, buffer, fpos (2), num);
}

int he2_put (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  return puthe2 (mat->ap, buffer, fpos (2), num);
}

/*----------------------------------------------------------------------*/

int le4t_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  int ndata, nzero;
  int res;

  ndata = line +1 - col;
  if (ndata < 0) ndata = 0;
  if (ndata > num) ndata = num;
  nzero = num - ndata;

  if ((res = getle4 (mat->ap, buffer, fpos_t (4), (unsigned) ndata)) < ndata) return res;
  memset (buffer +ndata, 0, nzero * sizeof(*buffer));

  return num;
}

int le4t_put (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  int ndata;
  int res;
  int i;

  ndata = line +1 - col;
  if (ndata < 0) ndata = 0;
  if (ndata > num) ndata = num;

  if ((res = putle4 (mat->ap, buffer, fpos_t (4), (unsigned) ndata)) < ndata) return res;

  for (i = ndata; i < num; i++) {
    if (*(buffer +i) != 0) return i;
  }

  return num;
}

int le2t_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  int ndata, nzero;
  int res;

  ndata = line +1 - col;
  if (ndata < 0) ndata = 0;
  if (ndata > num) ndata = num;
  nzero = num - ndata;

  if ((res = getle2 (mat->ap, buffer, fpos_t (2), (unsigned) ndata)) < ndata) return res;
  memset (buffer +ndata, 0, nzero * sizeof(*buffer));

  return num;
}

int le2t_put (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  int ndata;
  int res;
  int i;

  ndata = line +1 - col;
  if (ndata < 0) ndata = 0;
  if (ndata > num) ndata = num;

  if ((res = putle2 (mat->ap, buffer, fpos_t (2), (unsigned) ndata)) < ndata) return res;

  for (i = ndata; i < num; i++) {
    if (*(buffer +i) != 0) return i;
  }

  return num;
}

int he4t_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  int ndata, nzero;
  int res;

  ndata = line +1 - col;
  if (ndata < 0) ndata = 0;
  if (ndata > num) ndata = num;
  nzero = num - ndata;

  if ((res = gethe4 (mat->ap, buffer, fpos_t (4), (unsigned) ndata)) < ndata) return res;
  memset (buffer +ndata, 0, nzero * sizeof(*buffer));

  return num;
}

int he4t_put (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  int ndata;
  int res;
  int i;

  ndata = line +1 - col;
  if (ndata < 0) ndata = 0;
  if (ndata > num) ndata = num;

  if ((res = puthe4 (mat->ap, buffer, fpos_t (4), (unsigned) ndata)) < ndata) return res;

  for (i = ndata; i < num; i++) {
    if (*(buffer +i) != 0) return i;
  }

  return num;
}

int he2t_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  int ndata, nzero;
  int res;

  ndata = line +1 - col;
  if (ndata < 0) ndata = 0;
  if (ndata > num) ndata = num;
  nzero = num - ndata;

  if ((res = gethe2 (mat->ap, buffer, fpos_t (2), (unsigned) ndata)) < ndata) return res;
  memset (buffer +ndata, 0, nzero * sizeof(*buffer));

  return num;
}

int he2t_put (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     int *buffer;
{
  int ndata;
  int res;
  int i;

  ndata = line +1 - col;
  if (ndata < 0) ndata = 0;
  if (ndata > num) ndata = num;

  if ((res = puthe2 (mat->ap, buffer, fpos_t (2), (unsigned) ndata)) < ndata) return res;

  for (i = ndata; i < num; i++) {
    if (*(buffer +i) != 0) return i;
  }

  return num;
}

/*----------------------------------------------------------------------*/

int lf4_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     float *buffer;
{
  return getle4 (mat->ap, (int *)buffer, fpos (4), num);
}

int lf4_put (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     float *buffer;
{
  return putle4 (mat->ap, (int *)buffer, fpos (4), num);
}

int hf4_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     float *buffer;
{
  return gethe4 (mat->ap, (int *)buffer, fpos (4), num);
}

int hf4_put (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     float *buffer;
{
  return puthe4 (mat->ap, (int *)buffer, fpos (4), num);
}

/*----------------------------------------------------------------------*/

int lf8_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     double *buffer;
{
  return getle8 (mat->ap, (int *)buffer, fpos (4), num);
}

int lf8_put (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     double *buffer;
{
  return putle8 (mat->ap, (int *)buffer, fpos (4), num);
}

int hf8_get (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     double *buffer;
{
  return gethe8 (mat->ap, (int *)buffer, fpos (4), num);
}

int hf8_put (mat, buffer, level, line, col, num)
     MFILE *mat;
     u_int level, line, col, num;
     double *buffer;
{
  return puthe8 (mat->ap, (int *)buffer, fpos (4), num);
}

