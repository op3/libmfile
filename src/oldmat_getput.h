/*
 * oldmat_getput.h:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

/* $Id: oldmat_getput.h,v 1.7 1993/04/22 15:26:28 se Exp $
 *
 * $Log: oldmat_getput.h,v $
 * Revision 1.7  1993/04/22  15:26:28  se
 * Support added for SIGNED 2 byte integer data
 *
 * Revision 1.6  1992/07/30  12:14:25  se
 * added copyright note
 *
 * Revision 1.5  1992/07/25  14:17:08  se
 * added prototypes for subroutines that read and write triagonal matrizes
 *
 * Revision 1.4  1992/06/10  11:28:14  se
 * *** empty log message ***
 *
 * Revision 1.3  1992/02/14  19:05:34  se
 * first implementation of (trivial) floating point spectra and matrices
 *
 * Revision 1.2  1992/02/13  16:19:08  se
 * RCS Id and Log entries added
 *
 */

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

/* oldmat_getput.c */
extern int le4_get P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int le4_put P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int le2_get P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int le2_put P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int he4_get P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int he4_put P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int he2_get P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int he2_put P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int le2s_get P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int he2s_get P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));

extern int le4t_get P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int le4t_put P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int le2t_get P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int le2t_put P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int he4t_get P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int he4t_put P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int he2t_get P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int he2t_put P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));

extern int lf4_get P_((MFILE *mat, float *buffer, u_int level, u_int line, u_int col, u_int num));
extern int lf4_put P_((MFILE *mat, float *buffer, u_int level, u_int line, u_int col, u_int num));
extern int hf4_get P_((MFILE *mat, float *buffer, u_int level, u_int line, u_int col, u_int num));
extern int hf4_put P_((MFILE *mat, float *buffer, u_int level, u_int line, u_int col, u_int num));
extern int lf8_get P_((MFILE *mat, double *buffer, u_int level, u_int line, u_int col, u_int num));
extern int lf8_put P_((MFILE *mat, double *buffer, u_int level, u_int line, u_int col, u_int num));
extern int hf8_get P_((MFILE *mat, double *buffer, u_int level, u_int line, u_int col, u_int num));
extern int hf8_put P_((MFILE *mat, double *buffer, u_int level, u_int line, u_int col, u_int num));
