/*
 * gf2_minfo.h:	        derived by Nigel Warr (April 2003) from
 *                      oldmat_minfo.h which is:
 *                      Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 *  This file provides support for Radware gf2 files.
 */

#include "endian.h"

#define GF2_STD_VERSION 2

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif

/* gf2_minfo.c */
extern void gf2_probe P_((MFILE *mat));
extern void gf2_init P_((MFILE *mat));
extern int gf2_uninit P_((MFILE *mat));
