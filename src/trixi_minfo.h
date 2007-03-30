/*
 * trixi_minfo.h:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <sys/types.h>
#include "mfile.h"

/* $Id: trixi_minfo.h,v 1.1 1992/07/30 12:14:25 se Exp $
 *
 * $Log: trixi_minfo.h,v $
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

void trixi_probe P_((MFILE *mat));
