/*
 * specio.c:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include "mfile.h"

static char rcsid[] = "$Id: specio.c,v 1.7 1998/11/09 18:21:10 root Exp $";

int load_spec(const char *name, unsigned long *buf, int num) {

  int n;
  MFILE *mat;

  mat = mopen ((char *)name, "r");
  n = mget (mat, (void *)buf, 0, 0, 0, num);
  if (mclose (mat) != 0) return -1;

  return n;
}

int save_spec(const char *name, unsigned long *buf, int num) {

  int n;
  MFILE *mat;
  minfo info;
  
  mat = mopen ((char *)name, "w");

  mgetinfo (mat, &info);
  info.filetype = MAT_LC;
  info.levels   = 1;
  info.lines    = 1;
  info.columns  = num;
  msetinfo (mat, &info);

  n = mput (mat, (void *)buf, 0, 0, 0, num);
  if (mclose (mat) != 0) return -1;

  return n;
}

