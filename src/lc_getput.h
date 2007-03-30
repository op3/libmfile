/*
 * lc_getput.c:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

/* $Id: lc_getput.h,v 1.5 1992/07/30 12:14:25 se Exp $
 *
 * $Log: lc_getput.h,v $
 * Revision 1.5  1992/07/30  12:14:25  se
 * add copyright note
 *
 * Revision 1.4  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.3  92/03/17  15:12:36  15:12:36  se (Stefan Esser)
 * complete rewrite to allow detection and signalling of write errors
 * lc_flush renamed to lc_flushcache, now returns int
 * tryreadcomprline replaced by readline, returns int
 * trywritecomprline replaced by writeline, returns int
 * deleted flushcacheline
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

extern int lc_get P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int lc_put P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int lc_flushcache P_((MFILE *mat));
