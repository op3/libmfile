/*
 * mopen.h:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <sys/types.h>
#include "mfile.h"

/* $Id: mopen.h,v 1.5 1992/07/30 12:14:25 se Exp $
 *
 * $Log: mopen.h,v $
 * Revision 1.5  1992/07/30  12:14:25  se
 * added copyright note
 *
 * Revision 1.4  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.3  92/03/17  15:29:51  15:29:51  se (Stefan Esser)
 * added prototye for mflush
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

extern MFILE *mopen P_((char *name, char *mode));
extern int mflush P_((MFILE *mat));
extern int mclose P_((MFILE *mat));
extern int mgetinfo P_((MFILE *mat, minfo *info));
extern int msetinfo P_((MFILE *mat, minfo *info));
extern void initspecinfo P_((MFILE *mat));
