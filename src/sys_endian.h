/*
 * sys_endian.h:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include "config.h"

/* 
 * This is now handled via autoconf. See configure.ac: AC_C_BIGENDIAN and config.h 
 * */
/*
#if defined (i386) || defined (i486) || defined (MIPSEL) || defined (vax)
#  define LOWENDIAN 1
#endif
*/

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

