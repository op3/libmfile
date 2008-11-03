/*
 * mfile.h
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
#ifndef _MATRIX_IO_INCLUDED
#define _MATRIX_IO_INCLUDED

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#if defined (OSK) || defined (VMS_POSIX)
#define NO_SHM
#endif

/* matrix file type names						*/

#define MAT_UNKNOWN	(0)
#define MAT_INVALID	(-1)

#define MAT_LC		(1)	/* line compressed matrix file 		*/
#define MAT_LE2		(2)	/* old 2 byte VAX matrix file		*/
#define MAT_LE4		(3)	/* old 4 byte VAX matrix file		*/
#define MAT_HE2		(4)	/* old 2 byte HighEndian matrix file	*/
#define MAT_HE4		(5)	/* old 4 byte HighEndian matrix file	*/
#ifndef NO_SHM
#define MAT_SHM		(6)	/* shared mem spectra			*/
#endif /* NO_SHM */
#define MAT_LF4		(7)	/* low endian 4 byte IEEE float		*/
#define MAT_LF8		(8)	/* low endian 8 byte IEEE float		*/
#define MAT_HF4		(9)	/* high endian 4 byte IEEE float	*/
#define MAT_HF8		(10)	/* high endian 8 byte IEEE float	*/
#define MAT_VAXF	(11)	/* VAX F format 4 byte float		*/
#define MAT_VAXG	(12)	/* VAX G format 8 byte float		*/
#define MAT_MATE	(13)	/* PC-Mate spectra  format		*/
#define MAT_TXT		(14)	/* ASCII spectra, Integer or Double	*/
#define MAT_LE2T	(15)	/* symm triagonal LE2 matrix file	*/
#define MAT_LE4T	(16)	/* symm triagonal LE4 matrix file	*/
#define MAT_HE2T	(17)	/* symm triagonal HE2 matrix file	*/
#define MAT_HE4T	(18)	/* symm triagonal HE4 matrix file	*/
#define MAT_TRIXI	(19)	/* trixi save_matrix format		*/
#define MAT_LE2S	(20)	/* signed LE2 matrix file		*/
#define MAT_HE2S	(21)	/* signed HE2 matrix file		*/

#define MAT_GF2         (22)    /* Radware gf2 format                   */
#define MAT_HGF2        (23)    /* Big endian Radware gf2 format        */
#define MAT_FMTLAST	(23)	/* last format currently assigned	*/

#define MAT_STD_INT	MAT_LC	/* default integer matrix format	*/
#define MAT_STD_FLT	MAT_LF4	/* only for testing purpises ...	*/
#define MAT_STD_DBL	MAT_LF8 /* only for testing purpises ...	*/

/* 'natural' data format names						*/

#define MAT_D_SIZE	(0xff)	/* bits reserved for elem size info	*/

#define MAT_D_INT_U	(0x100)	/* signed int				*/
#define MAT_D_INT_S	(0x200)	/* unsigned int				*/
#define MAT_D_FLOAT	(0x400)	/* floating point			*/

#define MAT_D_INV	(0)
#define MAT_D_I2U	(MAT_D_INT_U + 2)
#define MAT_D_I2S	(MAT_D_INT_S + 2)
#define MAT_D_I4S	(MAT_D_INT_S + 4)
#define MAT_D_F4	(MAT_D_FLOAT + 4)
#define MAT_D_F8	(MAT_D_FLOAT + 8)

/* implementation limits						*/

#define MAT_COLMAX	(1<<16)	/* for old matrix file formats		*/

#define MAT_DATA	0
#define MAT_DATAERR	1
#define MAT_BG		2
#define MAT_BGERR	3

/* MFILE.status bit definitions						*/

#define MST_DIMSFIXED	0x0001	/* changing lines/colums prohibited	*/
#define MST_DIRTY	0x0002	/* file image must be updated		*/
#define MST_INITIALIZED	0x0004	/* function pointers and specinfo init.	*/

#define MST_INTERN	(MST_DIMSFIXED | MST_DIRTY | MST_INITIALIZED)
#define MST_USER	( 0 )


typedef struct minfo {
  int		filetype;		/* MAT_LC, MAT_LE2, ...		*/
  u_int		levels;
  u_int		lines;
  u_int 	columns;
  u_int		version;
  u_int		status;
  char		*name;
  char		*comment;
} minfo;

typedef struct accessmethod *amp;

typedef struct matfile {
  amp		ap;
  char		*name;
  char		*comment;
  int		mode;
  u_int		version;
  u_int		status;
  int		filetype;
  u_int		levels;
  u_int		lines;
  u_int		columns;
  int		(*mflushf)();
  int		(*muninitf)();
  int		(*mgeti4f)();
  int		(*mgetf4f)();
  int		(*mgetf8f)();
  int		(*mputi4f)();
  int		(*mputf4f)();
  int		(*mputf8f)();
  union {
    void	*p;
    int		i;
  } specinfo;
} MFILE;



#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

MFILE *mopen P_((char *name, char *mode));
int mclose P_((MFILE *mat));
int mflush P_((MFILE *mat));

int mgetinfo P_((MFILE *mat, minfo *info));
int msetinfo P_((MFILE *mat, minfo *info));

/* format: [[[LEVELS '.'] LINES '.'] COLUMNS '.' [MATTYPE [ ':' VERSION]] */
/* valid examples: 4k.4k  (4k*4k matrix of unspecified element type)	  */
/*		   8k.le4 (8K low endian 4 byte spectrum)		  */
/*		   lc:1   (line compressed file (version = 1)		  */
int msetfmt P_((MFILE *mat, char *format));
char* mgetfmt P_((MFILE *mat, char *format));

/* lev: [0..(levels-1)], lin: [0..(lines-1)], col: [0..(columns-1)] */
int mgetint P_((MFILE* mat, int buf[], int lev, int lin, int col, int num));
int mputint P_((MFILE* mat, int buf[], int lev, int lin, int col, int num));

int mgetflt P_((MFILE* mat, float buf[], int lev, int lin, int col, int num));
int mputflt P_((MFILE* mat, float buf[], int lev, int lin, int col, int num));

int mgetdbl P_((MFILE* mat, double buf[], int lev, int lin, int col, int num));
int mputdbl P_((MFILE* mat, double buf[], int lev, int lin, int col, int num));

#define mget(mat,buf,lev,lin,col,num) mgetint(mat,buf,lev,lin,col,num)
#define mput(mat,buf,lev,lin,col,num) mputint(mat,buf,lev,lin,col,num)

int load_spec P_((char *name, int *buf, int num));
int save_spec P_((char *name, int *buf, int num));

#ifdef LIBMAT_BC
/* don't use, ONLY for compatibility with old source files !!! */
#define MATVAX2		MAT_LE2
#define MATVAX4		MAT_LE4
#define MATHE2		MAT_HE2
#define MATHE4		MAT_LE4
#define MAXCOL		MAT_COLMAX


#define mgetline(m,b,l,c,n)		mget(m,b,MAT_DATA,l,c,n)
#define mputline(m,b,l,c,n)		mput(m,b,MAT_DATA,l,c,n)

#define mgeterrline(m,b,l,c,n)		mget(m,b,MAT_DATAERR,l,c,n)
#define mputerrline(m,b,l,c,n)		mput(m,b,MAT_DATAERR,l,c,n)

#define mgetbgline(m,b,l,c,n)		mget(m,b,MAT_BG,l,c,n)
#define mputbgline(m,b,l,c,n)		mput(m,b,MAT_BG,l,c,n)

#define mgetbgerrline(m,b,l,c,n)	mget(m,b,MAT_BGERR,l,c,n)
#define mputbgerrline(m,b,l,c,n)	mput(m,b,MAT_BGERR,l,c,n)
#endif /* LIBMAT_BC */

#ifdef __cplusplus
}
#endif  /* C++ */

#endif /* _MATRIX_IO_INCLUDED */

#undef P_
