/*
 * maccess.h:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

/* $Id: maccess.h,v 1.1 1993/07/20 18:01:01 se Exp $
 *
 * $Log: maccess.h,v $
 * Revision 1.1  1993/07/20  18:01:01  se
 * Initial revision
 *
 *
 */

#include "mfile.h"

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

/* ------------------------------------------------------------------------- */

#define _get(ap,buf,pos,num)	ap->get(ap,buf,pos,num)
#define _put(ap,buf,pos,num)	ap->put(ap,buf,pos,num)
#define _geta(ap,pos,num)	ap->geta(ap,pos,num)
#define _puta(ap,pos,num)	ap->puta(ap,pos,num)
#define _flush(ap)		ap->flush(ap)
#define _close(ap)		ap->close(ap)

/* ------------------------------------------------------------------------- */

typedef unsigned int acc_pos;

/*typedef struct accessmethod *amp;*/

typedef int tryaccessf P_((amp ap, char *name, char *mode));
typedef int flushf P_((amp ap));
typedef int closef P_((amp ap));
typedef int getf P_((amp ap, void *buffer, acc_pos offset, acc_pos nbytes));
typedef int putf P_((amp ap, void *buffer, acc_pos offset, acc_pos nbytes));
typedef void *getaf P_((amp ap, acc_pos offset, acc_pos nbytes));
typedef void *putaf P_((amp ap, acc_pos offset, acc_pos nbytes));

typedef struct accessmethod
{
  getf		*get;
  putf		*put;
  getaf		*geta;
  putaf		*puta;
  flushf	*flush;
  closef	*close;
  char		*name;
  void		*buffer;
  acc_pos	size;
  acc_pos	rd_offs;
  acc_pos	rd_bytes;
  acc_pos	wr_offs;
  acc_pos	wr_bytes;
  struct
  {
    long int	i;
    void	*p;
  } specinfo;
} accessmethod;

/* ------------------------------------------------------------------------- */

typedef struct maccessdescr {
  tryaccessf		*tryaccess;
  char			*name;
  struct maccessdescr	*next;
} maccessdescr;

extern maccessdescr *tryaccess_first;

/* ------------------------------------------------------------------------- */

amp tryaccess P_((char *name, char *mode, char *accessname));
int maddaccess P_((tryaccessf *taf, char *name));
