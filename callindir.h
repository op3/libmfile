/*
 * callindir.c:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

/* $Id: callindir.h,v 1.6 1992/07/30 12:13:48 se Exp $
 *
 * $Log: callindir.h,v $
 * Revision 1.6  1992/07/30  12:13:48  se
 * add copyright note
 *
 * Revision 1.5  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.4  92/02/15  17:22:42  17:22:42  se (Stefan Esser)
 * name change: mget -> mgetint for consistency reasons ...
 * 
 * Revision 1.3  1992/02/14  19:22:46  se
 * added hooks for floating point spectra and matrixes
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

/* callindir.c */
extern int mgetint P_((MFILE *mat, int *buffer, int level, int line, int col, int num));
extern int mputint P_((MFILE *mat, int *buffer, int level, int line, int col, int num));
extern int mgetflt P_((MFILE *mat, float *buffer, int level, int line, int col, int num));
extern int mputflt P_((MFILE *mat, float *buffer, int level, int line, int col, int num));
extern int mgetdbl P_((MFILE *mat, double *buffer, int level, int line, int col, int num));
extern int mputdbl P_((MFILE *mat, double *buffer, int level, int line, int col, int num));

