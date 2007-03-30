/*
 * shm_getput.h:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

/* $Id: shm_getput.h,v 1.4 1992/07/30 12:14:25 se Exp $
 *
 * $Log: shm_getput.h,v $
 * Revision 1.4  1992/07/30  12:14:25  se
 * added copyright note
 *
 * Revision 1.3  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.2  92/02/13  16:19:08  16:19:08  se (Stefan Esser)
 * RCS Id and Log entries added
 * 
 */

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

extern int shm_get P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
/*
extern int shm_put P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
*/
