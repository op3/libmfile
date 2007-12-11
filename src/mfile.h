/*
 * mfile.h:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

/* $Id: mfile.h,v 1.17 1993/07/20 18:01:23 se Exp $
 *
 * $Log: mfile.h,v $
 * Revision 1.17  1993/07/20  18:01:23  se
 * modified for use with access method module
 *
 * Revision 1.16  1993/04/22  15:26:28  se
 * Support added for SIGNED 2 byte integer data
 *
 * Revision 1.15  1992/12/31  11:00:15  se
 * now defines NO_SHM on systems that don't support SystemV Shared Memory
 * (OS9, VMS).
 * definition of MAT_SHM is now dependent on NO_SHM not being defined
 *
 * Revision 1.14  1992/07/30  12:14:25  se
 * added support of comment fields in MFILE and minfo structs
 * added copyright note
 *
 * Revision 1.13  1992/07/25  14:14:58  se
 * added definitions of MAT_LE2T/LE4T/HE2T/HE4T for triagonal matrizes
 *
 * Revision 1.12  1992/07/08  21:03:39  se
 * general cleanup for strictest GCC warning levels
 *
 * Revision 1.11  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.10  92/06/10  11:25:44  11:25:44  se (Stefan Esser)
 * mat->specinfo changed from int* to union {int i, int *p}
 * new defines for datatype names currently in use
 * 
 * Revision 1.9  1992/05/13  23:33:03  se
 * added MAT_TXT to mean a matrix or spectrum in an ASCII representation
 *
 * Revision 1.8  1992/03/17  15:26:26  se
 * new field mflushf in struct MFILE to keep pointer to format specific
 * cache flush subroutine
 * ,
 *
 * Revision 1.7  1992/03/12  17:09:34  se
 * added define for MAT_MATE
 * added undef of P_ because other headers may try to define it again
 *
 * Revision 1.6  1992/02/18  23:30:32  se
 * added definitions of MST_INTERN for only internally changeable bits of
 * mat->status, and MST_USER for user defined bits in this variable.
 *
 * Revision 1.5  1992/02/17  23:10:45  se
 * corrected typo ...
 *
 * Revision 1.4  1992/02/15  17:22:42  se
 * initspecinfo doesn't assume 4k.4k.lc anymore ...
 *
 * Revision 1.3  1992/02/14  19:30:43  se
 * added definition of m{get,put}{flt,dbl}() which read spectra and
 * matrixes into buffers of type float[] rsp. double[]
 *
 * Revision 1.2  1992/02/13  16:19:08  se
 * RCS Id and Log entries added
 *


File:

	/usr/include/local/mfile.h

Changelog:

	Stefan Esser, 30.12.91
		completely reworked version
		support for compressed 2 and 3 dim matrizes
		name of header changed from 'matrixio.h' to 'mfile.h'
		library now '/usr/lib/libmfile.a'

	Stefan Esser, 05.11.91
		new fields in MFILE for compressed matrix files
		line, col, num changed from int to u_int

	Stefan Esser, 23.07.90
		changed order of parameters in calls to mgetline/mputline

	Stefan Esser, 17.07.90
		changed to give more info on matrix structure

	Stefan Esser, 12.07.90
		first version of matrix file and data access functions
		(library '/usr/include/libmat.a')

*/

#ifndef _MATRIX_IO_INCLUDED
#define _MATRIX_IO_INCLUDED

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

#endif /* _MATRIX_IO_INCLUDED */

#undef P_
