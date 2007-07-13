/*
 * mate_minfo.c:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			xUniversity of Cologne, Germany
 *
 */

#include <stdlib.h>
#include <string.h>
/* #include <memory.h>*/

#include "mfile.h"
#include "maccess.h"
#include "sys_endian.h"
#include "getputint.h"
#include "mate_minfo.h"
#include "mate_getput.h"



/* $Log: mate_minfo.c,v $
 * Revision 1.6  1993/07/20  17:58:17  se
 * modified for use with access method module
 *
 * Revision 1.5  1992/07/30  12:09:41  se
 * added copyright note
 * added support for comment field
 *
 * Revision 1.4  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.3  92/06/30  14:49:52  14:49:52  se (Stefan Esser)
 * added cast in parameter list of get
 * 
 * Revision 1.2  1992/03/31  02:02:11  se
 * removed unreferenced variable channels
 *
 * Revision 1.1  1992/03/12  17:05:43  se
 * Initial revision
 *
 */

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
# define CONST const
#else
# define P_(s) ()
# define CONST /* */
#endif

static int match P_((CONST char *str, CONST char *pattern));

#undef P_

typedef struct {
  char dummy1[7];
  char name[15];
  char date[12];
  char time[12];
  char dummy2[210];
  char dummy3[10];
  short channels;
} mate_header;

static int match (str, pattern)
     CONST char *str;
     CONST char *pattern;
{
  char p;

  while (p = *pattern++) {
    char c = *str++;
    switch (p) {
case '\0':
      return 0;
case ' ':
case ':':
      if (c != p) return -1;
      break;
case '9':
      if ((c < '0' || c > '9') && c != ' ') return -1;
      break;
case 'Z':
      if (c < 'A' || c > 'Z') return -1;
      break;
case '?':
      break;
default:
      return -1;
      break;
    }
  }
  return 0;
}

static char *strbcat(char *s1, char *s2, int n) {

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

void mate_probe(MFILE *mat) {

  mate_header mate_h;
  char comment[128];

  if (_get (mat->ap, (char *)&mate_h, 0, sizeof(mate_h)) != sizeof(mate_h))
    return;

  if (match (mate_h.date, "ZZZ 99 9999") != 0) return;
  if (match (mate_h.time, "99:99:99 ZZ") != 0) return;

  mat->status		|=MST_DIMSFIXED;
  mat->filetype		= MAT_MATE;
  mat->version		= 0;

  mat->levels		= 1;
  mat->lines		= 1;
  mat->columns		= GETLE2 (mate_h.channels);

  mat->mgeti4f		= mate_get;
  mat->mputi4f		= NULL;
  mat->muninitf		= NULL;

  comment[0] = '\0';
  strbcat (comment, mate_h.name, sizeof (mate_h.name));
  strcat  (comment, " ");
  strbcat (comment, mate_h.date, sizeof (mate_h.date));
  strcat  (comment, " ");
  strbcat (comment, mate_h.time, sizeof (mate_h.time));

  mat->comment = (char *) malloc (strlen(comment) +1);
  strcpy (mat->comment, comment);

}

