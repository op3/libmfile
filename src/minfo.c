/*
 * minfo.c:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <ctype.h>
#include <stddef.h>
#include <memory.h>

#include "mfile.h"
#include "mat_types.h"



/* $Log: minfo.c,v $
 * Revision 1.11  1993/01/20  13:03:44  se
 * mtxttoinfo now always sets version if filetype specified
 *
 * Revision 1.10  1992/07/30  12:09:41  se
 * added copyright note
 * added support for comment field
 *
 * Revision 1.9  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.8  92/07/08  06:25:35  06:25:35  se (Stefan Esser)
 * now includes stddef.h
 * 
 * Revision 1.7  1992/06/10  11:15:17  se
 * corrected test for compatible formats in msetinfo (|| changed to &&)
 *
 * Revision 1.6  1992/05/13  23:35:28  se
 * changed format string parser to accept no spaces within format strings,
 * but to accept a format string which contains any text after a valid
 * format string, if ithe two are seperated by white space
 *
 * Revision 1.5  1992/03/31  02:03:07  se
 * msetfmt now returns 0 if called with fmt==NULL
 *
 * Revision 1.4  1992/02/18  23:32:04  se
 * uses access functions instead of directly accessing the configuration
 * array matproc[].
 *
 * Revision 1.3  1992/02/15  17:22:42  se
 * *** empty log message ***
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

static char *putnum P_((char *p, u_int n, int put1));
static char *putfmt P_((char *p, int filetype));
static char *minfototxt P_((char *fmt, minfo *info));

#undef P_

int mgetinfo(MFILE *mat, minfo *info) {

  if (info == NULL) return -1;

  if (mat == NULL) {
    info->filetype = MAT_INVALID;
    info->version  = 0;
    info->levels   = 0;
    info->lines    = 0;
    info->columns  = 0;
    info->status   = 0;
    info->name     = NULL;
    info->comment  = NULL;
    return -1;
  }

  info->filetype = mat->filetype;
  info->version	 = mat->version;
  info->levels	 = mat->levels;
  info->lines	 = mat->lines;
  info->columns	 = mat->columns;
  info->status   = mat->status;
  info->name     = mat->name;
  info->comment  = mat->comment;

  return 0;
}


int msetinfo(MFILE *mat, minfo *info) {

  /* check parameters and set filetype = MATINVALID in case of error */
  /* should call 'mat->msetinfof()' for sanity checks ... */

  if (mat == NULL || info == NULL)		return -1;

  if (mat->status & MST_DIMSFIXED) {
    if (mat->filetype	== info->filetype && 
	mat->version	== info->version  &&
	mat->levels	== info->levels	  &&
	mat->lines	== info->lines	  &&
	mat->columns	== info->columns) {
      return 0;
    } else {
      return -1;
    }
  }
  
  mat->filetype	= info->filetype;
  mat->version	= info->version;
  mat->levels	= info->levels;
  mat->lines	= info->lines;
  mat->columns	= info->columns;
  mat->status	= (mat->status & MST_INTERN) | (info->status & MST_USER);

  return 0;
}


int mtxttoinfo(char *fmt, minfo *info) {

  int lev=0;
  int lin=0;
  int col=0;
  int typ=MAT_UNKNOWN;
  int ver=0;

#define skipspace(p)	while (isspace(*p)) (p++)
#define getnum(p,n)	while (isdigit(*p)) n=n*10 + ((*p++) - '0')

  if (fmt == NULL) return 0;
  if (info == NULL) return -1;

  skipspace (fmt);

  while (isdigit (*fmt)) {
    if (lev != 0) return -1;
    lev = lin;
    lin = col;

    col = 0;
    getnum (fmt, col);
    if (*fmt == 'k') { 
      fmt++;
      col *= 1024; 
    }      
    if (col == 0) return -1;

    if (*fmt == '.') {
      fmt++;
    } else {
      break;
    }
  }
  if (isalpha (*fmt)) {
    char fmtname[8];
    char c;
    int i=0;
    
    while (i < (sizeof (fmtname) -1) && (c = *fmt) && c != ':') {
      fmtname[i] = c;
      fmt++;
      i++;
    }
    fmtname[i] = '\0';

    typ = matproc_filetype (fmtname);
    if (typ == MAT_INVALID) return -1;

    if (*fmt == ':') {
      fmt++;
      getnum (fmt, ver);
    }
  }
  if (*fmt != '\0'&& !isspace (*fmt)) return -1;

  if (typ != MAT_UNKNOWN) {
    info->filetype = typ;
    info->version = ver;
  }

/*  if (info->filetype == MAT_UNKNOWN) info->filetype == MAT_STD; */

  if (col != 0) {
    info->levels  = lev ? lev : 1;
    info->lines   = lin ? lin : 1;
    info->columns = col ? col : 1;
  }
  return 0;
#undef skipspace
#undef getnum
}



static char *putnum(char *p, u_int n, int put1) {

  if (n != 1 || put1) {
    char numbuf[32];
    char *revp = numbuf;
  
    if (n && (n % 1024 == 0)) { n /= 1024; *revp++ = 'k'; }

    while (n) { char d = n % 10; *revp++ = d + '0'; n /= 10; }

    while (revp != numbuf) { *p++ = *--revp; }
  }
  *p = '\0';

  return p;
}


static char *putfmt(char *p, int filetype) {

  char *cp;

  cp = matproc_fmtname (filetype);
  while (*cp) *p++ = *cp++;
  *p = '\0';

  return p;
}


static char *minfototxt(char *fmt, minfo *info) {

  static char txtbuf[127];
  char *p;

  if (fmt == NULL) fmt = txtbuf;
  p = fmt;

  if (info == NULL) {
    p = putfmt (p, MAT_INVALID);
  } else {
    p = putnum (p, info->levels, 0);
    if (p != fmt) *p++ = '.';
    p = putnum (p, info->lines, p != fmt);
    if (p != fmt) *p++ = '.';
    p = putnum (p, info->columns, 1);
    if (p != fmt) *p++ = '.';

    p = putfmt (p, info->filetype);

    if (info->version) {
      *p++ = ':';
      p = putnum (p, info->version, 1);
    }
  }
  
  return fmt;
}


int msetfmt(MFILE *mat, char *format) {

  minfo info;
  if (mat && format) {
    if (mgetinfo (mat, &info) != 0) return -1;
    if (mtxttoinfo (format, &info) != 0) return -1;
    return msetinfo (mat, &info);
  } else {
    memset (&info, 0, sizeof (minfo));
    return mtxttoinfo (format, &info);
  }
}


char* mgetfmt(MFILE *mat, char *format) {

  minfo info;

  mgetinfo (mat, &info);
  return minfototxt (format, &info);
}
