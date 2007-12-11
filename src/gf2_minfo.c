/*
 * gf2_minfo.c:	        derived by Nigel Warr (April 2003) from
 *                      oldmat_minfo.c which is:
 *                      Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 *  This file provides support for Radware gf2 files.
 */

#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "mfile.h"
#include "maccess.h"
#include "getputint.h"
#include "gf2_minfo.h"
#include "gf2_getput.h"
#include "mat_types.h"
#include "sys_config.h"

unsigned int gf2_swap32(unsigned int in) {

    unsigned int out = 0;

    out |= (in & 0x000000FF) << 24;
    out |= (in & 0x0000FF00) << 8;
    out |= (in & 0x00FF0000) >> 8;
    out |= (in & 0xFF000000) >> 24;

    return(out);
}

/* Probe for gf2 format files */
void gf2_probe(MFILE *mat) {

    u_int gf2_header[9];

    if (_get(mat->ap, gf2_header, 0, sizeof(gf2_header)) != sizeof(gf2_header))
        return;

    if (gf2_header[0] == 0x00000018) {
        mat->filetype = MAT_GF2;
        mat->columns = gf2_header[3];
        mat->lines = gf2_header[4];
    } else if (gf2_header[0] == 0x18000000) {
        mat->filetype = MAT_HGF2;
        mat->columns = gf2_swap32(gf2_header[3]);
        mat->lines = gf2_swap32(gf2_header[4]);
    } else
        return;
    mat->levels = 1;

    return;
}

/* Initialise gf2 format */
void gf2_init(MFILE *mat) {

    if (0 < mat->columns && mat->columns <= MAT_COLMAX) {
        int filetype = mat->filetype;
        int datatype = matproc_datatype (filetype);
        int elemsize = datatype & MAT_D_SIZE;
        mgetf* getfLocal   = matproc_getf (filetype);
        mputf* putfLocal   = matproc_putf (filetype);

        mat->specinfo.i = elemsize;
        mat->version = GF2_STD_VERSION;

        mat->mgetf4f		= getfLocal;
        mat->mputf4f		= putfLocal;
        mat->muninitf		= gf2_uninit;
    }
}

/* End gf2 format */
int gf2_uninit(MFILE *mat) {

    int status;
    u_int num = mat->columns;
    int gf2_header[9];
    char *ptr;
    unsigned int elemsize = mat->specinfo.i;
    unsigned int matsize  = mat->levels * mat->lines * mat->columns * elemsize;

    gf2_header[0] = 24;			/* recla */
    gf2_header[1] = 0;			/* Name - 8 bytes */
    gf2_header[2] = 0;
    gf2_header[3] = num;			/* Size 1 (always len) */
    gf2_header[4] = 1;			/* size 2 (always 1) */
    gf2_header[5] = 1;			/* ired 1 (always 1) */
    gf2_header[6] = 1;			/* ired 2 (always 1) */
    gf2_header[7] = 1;			/* rec 1b (always 1) */
    gf2_header[8] = num * sizeof(float);	/* rec 2a (always len * sizeof(float)) */

    if ((mat->status & MST_DIRTY) == 0)
        return 0;

    if (mat->version == 2) {

        if (matsize == 0)
            return 0;

        /* Get matrix name without path */
        ptr = rindex(mat->name, DIR_SEPARATOR_CHAR);
        if (ptr == NULL) {
            ptr = mat->name;
        } else
            ptr++; /* This is the beginnig of the matrix name */

        memcpy((char *)(gf2_header + 1), ptr, (strlen(ptr) > 8) ? 8 : strlen(ptr));

        /* Write gf2 header */

        status = putle4(mat->ap, gf2_header, 0, 9);
        if (status != 9)
            return -1;

        /* Write gf2 trailer */

        status = putle4 (mat->ap, gf2_header + 8, num * 4 + 36, 1);
        if (status != 1)
            return -1;

    }
    return 0;
}

