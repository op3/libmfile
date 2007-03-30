/*
 * trixi_getput.h:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

/* $Id: trixi_getput.h,v 1.1 1992/07/30 12:14:25 se Exp $
 *
 * $Log: trixi_getput.h,v $
 * Revision 1.1  1992/07/30  12:14:25  se
 * Initial revision
 *
 *
 */

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

int trixi_get P_((MFILE *mat, int *buffer, u_int level, u_int line, u_int col, u_int num));
