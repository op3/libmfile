/*
 * mat_types.h
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

typedef void mprobef(MFILE *mat);
typedef void minitf(MFILE *mat);
typedef int  mgetf(MFILE *mat, void* buf, int v, int l, int c, int n);
typedef int  mputf(MFILE *mat, void* buf, int v, int l, int c, int n);

void matproc_guessfiletype(MFILE *mat);
void matproc_init(MFILE *mat);
int matproc_filetype(const char *fmtname);
char *matproc_fmtname(int filetype);
int matproc_datatype(int filetype);
mgetf* matproc_getf(int filetype);
mputf* matproc_putf(int filetype);

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

