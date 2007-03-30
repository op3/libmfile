/*
 * mate_getput.h:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

/* $Id: mate_getput.h,v 1.2 1992/07/30 12:14:25 se Exp $
 *
 * $Log: mate_getput.h,v $
 * Revision 1.2  1992/07/30  12:14:25  se
 * add copyright note
 *
 * Revision 1.1  1992/03/12  17:05:43  se
 * Initial revision
 *
 */

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

int mate_get P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
