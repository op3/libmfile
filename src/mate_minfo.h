/*
 * mate_minfo.h:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <sys/types.h>
#include "mfile.h"

/* $Id: mate_minfo.h,v 1.3 1992/07/30 12:14:25 se Exp $
 *
 * $Log: mate_minfo.h,v $
 * Revision 1.3  1992/07/30  12:14:25  se
 * add copyright note
 *
 * Revision 1.2  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.1  92/03/12  17:05:43  17:05:43  se (Stefan Esser)
 * Initial revision
 * 
 */

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

void mate_probe P_((MFILE *mat));
