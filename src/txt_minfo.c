/*
 * txt_minfo.c:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <ctype.h>

#include "mfile.h"
#include "maccess.h"
#include "getputint.h"
#include "txt_minfo.h"
#include "txt_getput.h"



/* $Log: txt_minfo.c,v $
 * Revision 1.10  1993/07/20  18:00:31  se
 * modified for use with access method module
 *
 * Revision 1.9  1992/12/31  10:56:23  se
 * txt_alloc no longer dumps core, if malloc failed
 *
 * Revision 1.8  1992/07/30  12:09:41  se
 * added copyright note
 *
 * Revision 1.7  1992/07/25  14:20:12  se
 * now includes string.h instead of strings.h
 * now uses malloc/memset instead of calloc
 *
 * Revision 1.6  1992/07/08  21:03:39  se
 * general cleanup for strictest GCC warning levels
 *
 * Revision 1.5  1992/07/08  18:57:51  se
 * added cast to (int *) of return value from calloc()
 *
 * Revision 1.4  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.3  92/07/08  06:26:50  06:26:50  se (Stefan Esser)
 * now #includes strings.h and ctype.h
 * 
 * Revision 1.2  1992/06/10  11:22:17  se
 * changes to reflect mat->specinfo now being union {int, int*}
 *
 * Revision 1.1  1992/05/13  23:39:40  se
 * Initial revision
 *
 */

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

static int txt_load P_((MFILE *mat));
static int txt_alloc P_((MFILE *mat));
static void txt_free P_((MFILE *mat));

#undef P_

#define PROBESIZE 8192

#define GETBUF		(nleft = _get (mat->ap, buffer, fpos, sizeof (buffer)),\
                         fpos += nleft,					      \
                         nleft--)

#define NEXTCHAR	(nleft-- ? *++p :				      \
			   (GETBUF > 0 ? (p=buffer, buffer[0]) : '\0'))


#define SKIPTOEOL	{ do c = NEXTCHAR; while (c && c != '\n');	  }

#define SKIPSPACE	{ while (c && isspace (c))	    c = NEXTCHAR; }

#define SKIPCOMMENT	{ if (c && (c == '#' /*|| c == '!'*/))		      \
			  { SKIPTOEOL;  continue; } }

#define GETSIGN(PUTC)	{ if (c == '-' || c == '+') { PUTC; c = NEXTCHAR; } }

#define GETDOT(PUTC)	{ if (c == '.')		    { PUTC; c = NEXTCHAR; } }

#define GETE(PUTC)	{ if (c == 'e' || c == 'E') { PUTC; c = NEXTCHAR; } }

#define GETINT(PUTC)	{ while (isdigit (c))	    { PUTC; c = NEXTCHAR; } }

#define GETNUMBER(PUTC)	{						      \
			  GETSIGN(PUTC);				      \
			  GETINT(PUTC);					      \
			  GETDOT(PUTC);					      \
			  GETINT(PUTC);					      \
			  GETE(PUTC);					      \
			  GETSIGN(PUTC);				      \
			  GETINT(PUTC);					      \
			}




void txt_probe(MFILE *mat) {

  char buffer[PROBESIZE];
  char *p = buffer;
  unsigned int fpos = 0;
  int nleft = 0;

  int numbers = 0;
  char c = NEXTCHAR;

  if (nleft > sizeof (TXT_MAGIC) 
      && strncmp (buffer, TXT_MAGIC, sizeof(TXT_MAGIC) -1) == 0)
  {
    msetfmt (mat, buffer + sizeof (TXT_MAGIC) -1);
    return;
  }

  for (;;)
  {
    SKIPSPACE;
    SKIPCOMMENT;
    if (c == '\0') break;
    if (c != '-' && c != '+' && !isdigit (c)) return;
    GETNUMBER ({});
    numbers++;
  }
  if (numbers) 
  {
    mat->filetype = MAT_TXT;
    mat->columns = numbers;
    mat->lines = 1;
    mat->levels = 1;
  }
}

static int txt_load(MFILE *mat) {

  char buffer[PROBESIZE];
  char *p = buffer;
  unsigned int fpos = 0;
  int nleft = 0;
  int maxnum = mat->levels * mat->lines * mat->columns;
  int n;

  char c = NEXTCHAR;

  double *dblp = (double *) mat->specinfo.p;
  if (dblp == NULL) return -1;

  n = 0;
  while (n < maxnum)
  {
    char buf[40];
    int i = 0;

    SKIPSPACE;
    SKIPCOMMENT;
    if (c == '\0') break;
    if (c != '-' && c != '+' && !isdigit (c)) return -1;

    GETNUMBER ({ if (i < sizeof(buf)) buf[i++] = c; });

    buf[i] = '\0';
    *dblp++ = atof (buf);
    n++;
  }
  return n;
}

static int txt_alloc(MFILE *mat) {

  unsigned arrsize = mat->levels * mat->lines * mat->columns * sizeof(double);
  mat->specinfo.p = (void *) malloc (arrsize);
  if (mat->specinfo.p == NULL) return -1;
  memset (mat->specinfo.p, 0, arrsize);

  return 0;
}

static void txt_free(MFILE *mat) {

  double *dblp = (double *) mat->specinfo.p;
  if (dblp) free (dblp);
  mat->specinfo.p = NULL;
}

void txt_init(MFILE *mat) {

  if (mat->status & MST_INITIALIZED) return;

  if (mat->version == 0) {
    mat->version = TXT_STD_VERSION;
  }

  if (txt_alloc (mat) == 0 && txt_load (mat) >= 0)
  {
    mat->mgetf8f		= txt_get;
    mat->mputf8f		= txt_put;
    mat->mflushf		= txt_flush;
    mat->muninitf		= txt_uninit;
  }
  else
  {
    txt_free (mat);
  }
}

int txt_uninit(MFILE *mat) {

  int status;

  status = txt_flush (mat);
  txt_free (mat);

  return status;
}  

