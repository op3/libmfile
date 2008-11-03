/*
 * lc_minfo.h
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
#include <sys/types.h>
#include "mfile.h"

#define MAGIC_LC	0x80FFFF10

#define LC_C1_VERSION	(1)
#define LC_C2_VERSION	(2)

#define LC_MAX_VERSION	LC_C2_VERSION

#define LC_STD_VERSION	LC_C2_VERSION

typedef struct {
  u_int		magic;
  u_int		version;
  u_int		levels, lines, columns;
  u_int		poslentablepos;
  u_int		freepos, freelistpos;
  u_int		used, free;
  u_int		status;
} lc_header;

typedef struct {
  u_int		pos,len;
} lc_poslen;

typedef struct {
  int		version;
  u_int		freepos, freelistpos;
  int		*linebuf;
  void		*comprlinebuf;
  u_int		cachedlinedirty;
  u_int		cachedline;
  u_int		cachedcomprline;
  u_int		comprlinelen;
  u_int		poslentablepos;
  lc_poslen	*poslentableptr;
  int		(*comprf)();
  int		(*uncomprf)();
} lc_minfo;

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

void lc_probe P_((MFILE *mat));
void lc_init P_((MFILE *mat));
int lc_putinfo P_((MFILE *mat, minfo *info));
int lc_uninit P_((MFILE *mat));
