/*
 * maccess.h
 */
/*
 * Copyright (c) 1992-2008, Stefan Esser <se@ikp.uni-koeln.de>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *	* Redistributions of source code must retain the above copyright notice, 
 *	  this list of conditions and the following disclaimer.
 * 	* Redistributions in binary form must reproduce the above copyright notice, 
 * 	  this list of conditions and the following disclaimer in the documentation 
 * 	  and/or other materials provided with the distribution.
 *    
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
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
