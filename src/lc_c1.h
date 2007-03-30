/*
 * lc_c1.h:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

/* $Id: lc_c1.h,v 1.7 1992/07/30 12:14:25 se Exp $
 *
 * $Log: lc_c1.h,v $
 * Revision 1.7  1992/07/30  12:14:25  se
 * add copyright note
 *
 * Revision 1.6  1992/07/08  21:03:39  se
 * general cleanup for strictest GCC warning levels
 *
 * Revision 1.5  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.4  92/06/30  14:36:08  14:36:08  se (Stefan Esser)
 * changed some types from unsigned to signed char and int
 * 
 * Revision 1.3  1992/03/17  15:06:45  se
 * changed type of lc_compress() to return int instead of u_int.
 * return codes are -1 in case of error, nuber of bytes after compression else
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

extern int lc1_compress P_((char *dest, int *src, int num));
extern int lc1_uncompress P_((int *dest, char *src, int num));
extern unsigned int lc1_comprlinelenmax P_((unsigned int col));
