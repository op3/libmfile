/*
 * matop_minfo.h:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

/* $Id: oldmat_minfo.h,v 1.4 1992/07/30 12:14:25 se Exp $
 *
 * $Log: oldmat_minfo.h,v $
 * Revision 1.4  1992/07/30  12:14:25  se
 * added copyright note
 *
 * Revision 1.3  1992/06/10  11:30:35  se
 * changed definition of MAGIC_OLDMAT into reference to external variable
 *
 * Revision 1.2  1992/02/13  16:19:08  se
 * RCS Id and Log entries added
 *
 */

#include "sys_endian.h"

extern char MAGIC_OLDMAT[];

#define OLDMAT_STD_VERSION 1

typedef char oldmat_header[64];

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

/* oldmat_minfo.c */
void oldmat_probe P_((MFILE *mat));
void oldmat_init P_((MFILE *mat));
int oldmat_uninit P_((MFILE *mat));
int oldmat_putinfo P_((MFILE *mat, minfo *info));
