/*
 * trixi_minfo.c:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <stdlib.h>
#include <string.h>

#include "mfile.h"
#include "maccess.h"
#include "sys_endian.h"
#include "getputint.h"
#include "trixi_minfo.h"
#include "trixi_getput.h"

static char rcsid[] = "$Id: trixi_minfo.c,v 1.2 1993/07/20 18:00:16 se Exp $";

/* $Log: trixi_minfo.c,v $
 * Revision 1.2  1993/07/20  18:00:16  se
 * modified for use with access method module
 *
 * Revision 1.1  1992/07/30  12:09:41  se
 * Initial revision
 *
 *
 */

static char trixi_magic[] = "Trixi Save_matrix";

typedef struct {
  char code[40];
  char name[20];
  char date[20];
  int x_res;
  int y_res;
  int bpc;
  int blocksize;
  char comment[416];
} trixi_header;


static char *strbcat (s1, s2, n)
     char *s1;
     char *s2;
     int n;
{
  char c;
  char *p = s1 + strlen(s1);
  int i;

  i = strlen(s2);
  if (i > n) i = n;

  while (i-- && (c = *s2++) != '\0') *p++ = c;
  *p = '\0';

  i = strlen(s1);
  while (i-- && *--p == ' ');
  *++p = '\0';
}

void trixi_probe (mat)
     MFILE *mat;
{
  trixi_header trixi_h;
  char comment[512];

  if (_get (mat->ap, (char *)&trixi_h, 0, sizeof(trixi_h)) != sizeof(trixi_h))
    return;

  if (strncmp (trixi_h.code, trixi_magic, strlen(trixi_magic)) != 0) return;

  /* only short matrizes supported at this time */
  if (GETLE4(trixi_h.bpc) != 2) return;

  mat->status		|=MST_DIMSFIXED;
  mat->filetype		= MAT_TRIXI;
  mat->version		= 0;

  mat->levels		= 1;
  mat->lines		= GETLE4 (trixi_h.y_res);
  mat->columns		= GETLE4 (trixi_h.x_res);

  mat->mgeti4f		= trixi_get;
  mat->mputi4f		= NULL;
  mat->muninitf		= NULL;

  comment[0] = '\0';
  strbcat (comment, trixi_h.name, sizeof (trixi_h.name));
  strcat  (comment, " ");
  strbcat (comment, trixi_h.date, sizeof (trixi_h.date));
  strcat  (comment, " ");
  strbcat (comment, trixi_h.comment, sizeof (trixi_h.comment));

  mat->comment = (char *) malloc (strlen(comment) +1);
  strcpy (mat->comment, comment);
}

