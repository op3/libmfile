/*
 * getputint.h
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


#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

/*
int   get    P_((int fd, void *buffer, u_int pos, u_int num));
int   put    P_((int fd, void *buffer, u_int pos, u_int num));
*/

/* signed low endian 8 byte matrix file */
u_int getle8 P_((amp ap, int *buffer, u_int pos, u_int num));
u_int putle8 P_((amp ap, int *buffer, u_int pos, u_int num));

/* signed high endian 8 byte matrix file */
u_int gethe8 P_((amp ap, int *buffer, u_int pos, u_int num));
u_int puthe8 P_((amp ap, int *buffer, u_int pos, u_int num));

/* signed low endian 4 byte matrix file */
u_int getle4 P_((amp ap, int *buffer, u_int pos, u_int num));
u_int putle4 P_((amp ap, int *buffer, u_int pos, u_int num));

/* signed high endian 4 byte matrix file */
u_int gethe4 P_((amp ap, int *buffer, u_int pos, u_int num));
u_int puthe4 P_((amp ap, int *buffer, u_int pos, u_int num));

/* unsigned low endian 2 byte matrix file */
u_int getle2 P_((amp ap, int *buffer, u_int pos, u_int num));
u_int putle2 P_((amp ap, int *buffer, u_int pos, u_int num));

/* unsigned high endian 2 byte matrix file */
u_int gethe2 P_((amp ap, int *buffer, u_int pos, u_int num));
u_int puthe2 P_((amp ap, int *buffer, u_int pos, u_int num));

/* signed low endian 2 byte matrix file */
u_int getle2s P_((amp ap, int *buffer, u_int pos, u_int num));

/* signed high endian 2 byte matrix file */
u_int gethe2s P_((amp ap, int *buffer, u_int pos, u_int num));
