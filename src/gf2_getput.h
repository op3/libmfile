/*
 * gf2_getput.h:	derived by Nigel Warr (April 2003) from
 *                      oldmat_getput.c which is:
 *                      Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 * 
 *  This file provides support for Radware gf2 files.
 *
 */

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

/* gf2_getput.c */
extern int gf2_get P_((MFILE *mat, float *buffer, u_int level, u_int line, u_int col, u_int num));
extern int gf2_put P_((MFILE *mat, float *buffer, u_int level, u_int line, u_int col, u_int num));
extern void gf2_probe P_((MFILE *mat));
extern void gf2_init P_((MFILE *mat));
extern int gf2_uninit P_((MFILE *mat));
