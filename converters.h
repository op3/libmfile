/*
 * converters.h:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <sys/types.h>

/* $Id: converters.h,v 1.2 1992/07/30 12:14:25 se Exp $
 *
 * $Log: converters.h,v $
 * Revision 1.2  1992/07/30  12:14:25  se
 * add copyright note
 *
 * Revision 1.1  1992/02/17  23:13:04  se
 * Initial revision
 *
 * Revision 1.1  1992/02/17  17:55:02  se
 * Initial revision
 *
 */

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

void installconverters P_((MFILE * mat));
