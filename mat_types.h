/*
 * mat_types.h:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include "mfile.h"

/* $Id: mat_types.h,v 1.8 1993/07/20 18:01:16 se Exp $
 *
 * $Log: mat_types.h,v $
 * Revision 1.8  1993/07/20  18:01:16  se
 * modified for use with access method module
 *
 * Revision 1.7  1992/07/30  12:14:25  se
 * add copyright note
 *
 * Revision 1.6  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.5  92/07/08  06:24:31  06:24:31  se (Stefan Esser)
 * removed parameter declaration from definitions of getf and putf
 * 
 * Revision 1.4  1992/06/10  11:22:38  se
 * struct matproc now contains additional fields:
 * getf(), putf() are pointers to get/put functions,
 * datatype contains info on int/float and range of the data
 *
 * Revision 1.3  1992/02/19  00:04:34  se
 * mat_types now defines access functions instead of making the matproc[]
 * array available
 *
 * Revision 1.2  1992/02/13  16:19:08  se
 * RCS Id and Log entries added
 *
 */

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
# define CONST const
#else
# define P_(s) ()
# define CONST /* */
#endif

typedef void mprobef P_((MFILE *mat));
typedef void minitf P_((MFILE *mat));
typedef int  mgetf P_((MFILE *mat, void* buf, int v, int l, int c, int n));
typedef int  mputf P_((MFILE *mat, void* buf, int v, int l, int c, int n));

void matproc_guessfiletype P_((MFILE *mat));
void matproc_init P_((MFILE *mat));
int matproc_filetype P_((CONST char *fmtname));
char *matproc_fmtname P_((int filetype));
int matproc_datatype P_((int filetype));
mgetf* matproc_getf P_((int filetype));
mputf* matproc_putf P_((int filetype));

typedef char fmtnametype[7];

typedef struct 
{
  int		filetype;
  fmtnametype	fmtname;
  int		datatype;
  mgetf		*mget;
  mputf		*mput;
  mprobef	*mprobe;
  minitf	*minit;
} matprocs;

