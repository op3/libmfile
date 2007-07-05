/*
 * sys_endian.h:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

/* $Id: sys_endian.h,v 1.4 1992/07/30 12:14:25 se Exp $
 *
 * $Log: sys_endian.h,v $
 * Revision 1.4  1992/07/30  12:14:25  se
 * add copyright note
 *
 * Revision 1.3  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.2  92/02/13  16:19:08  16:19:08  se (Stefan Esser)
 * RCS Id and Log entries added
 * 
 */

#if defined (i386) || defined (i486) || defined (MIPSEL) || defined (vax)
#  define LOWENDIAN 1
#endif

#define SWAB2(i)    (((i >> 8) & 0xFF) | ((i << 8) & 0xFF00))
#define SWAB4(i)    (((i >> 24) & 0xFF) | ((i >>  8) & 0xFF00) | ((i <<  8) & 0xFF0000) | ((i << 24)))
#ifdef LOWENDIAN
#  define GETHE2(i) SWAB2(i)
#  define GETHE4(i) SWAB4(i)
#  define GETLE2(i) (i)
#  define GETLE4(i) (i)
#else
#  define GETHE2(i) (i)
#  define GETHE4(i) (i)
#  define GETLE2(i) SWAB2(i)
#  define GETLE4(i) SWAB4(i)
#endif

