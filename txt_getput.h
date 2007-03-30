/*
 * txt_getput.h:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

/* $Id: txt_getput.h,v 1.3 1992/07/30 12:14:25 se Exp $
 *
 * $Log: txt_getput.h,v $
 * Revision 1.3  1992/07/30  12:14:25  se
 * added copyright note
 *
 * Revision 1.2  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.1  92/05/13  23:39:40  23:39:40  se (Stefan Esser)
 * Initial revision
 * 
 */

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

extern int txt_get P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int txt_put P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
extern int txt_flush P_((MFILE *mat));
