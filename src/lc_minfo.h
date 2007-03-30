/*
 * lc_minfo.h:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <sys/types.h>
#include "mfile.h"

/* $Id: lc_minfo.h,v 1.8 1992/07/30 12:14:25 se Exp $
 *
 * $Log: lc_minfo.h,v $
 * Revision 1.8  1992/07/30  12:14:25  se
 * add copyright note
 *
 * Revision 1.7  1992/07/25  14:11:09  se
 * LC_STD_VERSION set to LC_C2_VERSION (was LC_C1_VERSION)
 *
 * Revision 1.6  1992/07/08  21:03:39  se
 * general cleanup for strictest GCC warning levels
 *
 * Revision 1.5  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.4  92/06/30  14:50:48  14:50:48  se (Stefan Esser)
 * removed definition of LC_C3_VERSION
 * 
 * Revision 1.3  1992/03/17  15:24:52  se
 * struct lc_minfo changed to reflect comprf and uncomprf now return int
 *
 * Revision 1.2  1992/02/13  16:19:08  se
 * RCS Id and Log entries added
 *
 */

#define MAGIC_LC	0x80FFFF10

#define LC_C1_VERSION	(1)
#define LC_C2_VERSION	(2)

#define LC_MAX_VERSION	LC_C2_VERSION

#define LC_STD_VERSION	LC_C2_VERSION

typedef struct {
  u_int		magic;
  u_int		version;
  u_int		levels, lines, columns;
  u_int		poslentablepos;
  u_int		freepos, freelistpos;
  u_int		used, free;
  u_int		status;
} lc_header;

typedef struct {
  u_int		pos,len;
} lc_poslen;

typedef struct {
  int		version;
  u_int		freepos, freelistpos;
  int		*linebuf;
  void		*comprlinebuf;
  u_int		cachedlinedirty;
  u_int		cachedline;
  u_int		cachedcomprline;
  u_int		comprlinelen;
  u_int		poslentablepos;
  lc_poslen	*poslentableptr;
  int		(*comprf)();
  int		(*uncomprf)();
} lc_minfo;

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

void lc_probe P_((MFILE *mat));
void lc_init P_((MFILE *mat));
int lc_putinfo P_((MFILE *mat, minfo *info));
int lc_uninit P_((MFILE *mat));
