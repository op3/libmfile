/*
 * txt_minfo.h:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include "mfile.h"

/* $Id: txt_minfo.h,v 1.3 1992/07/30 12:14:25 se Exp $
 *
 * $Log: txt_minfo.h,v $
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

#define TXT_STD_VERSION 1

#define TXT_MAGIC	"#:MatrixFormat:"

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

void txt_probe P_((MFILE *mat));
void txt_init P_((MFILE *mat));
/*int txt_putinfo P_((MFILE *mat, minfo *info));*/
int txt_uninit P_((MFILE *mat));
