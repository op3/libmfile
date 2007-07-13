/*
 * mat_types.c:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include "mfile.h"
#include "lc_minfo.h"
#include "lc_getput.h"
#include "oldmat_minfo.h"
#include "oldmat_getput.h"
#include "gf2_getput.h"
#include "gf2_minfo.h"
#include "mate_minfo.h"
#include "mate_getput.h"
#include "trixi_minfo.h"
#include "trixi_getput.h"
#include "txt_minfo.h"
#include "txt_getput.h"
#ifdef undef
#ifndef NO_SHM
#include "shm_minfo.h"
#include "shm_getput.h"
#endif /* NO_SHM */
#endif
#include "mat_types.h"

static char rcsid[] = "$Id: mat_types.c,v 1.13 1993/07/20 17:57:16 se Exp $";

/* $Log: mat_types.c,v $
 * Revision 1.13  1993/07/20  17:57:16  se
 * *** empty log message ***
 *
 * Revision 1.12  1993/04/22  15:26:28  se
 * Support added for SIGNED 2 byte integer data
 *
 * Revision 1.11  1992/12/31  10:59:40  se
 * no shared mem support if NO_SHM defined
 *
 * Revision 1.10  1992/07/30  12:09:41  se
 * added copyright note
 *
 * Revision 1.9  1992/07/25  14:13:18  se
 * added entries for triagonal matrizes MAT_LE4T/LE2T/HE4T/HE2T
 *
 * Revision 1.8  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.7  92/06/10  11:13:51  11:13:51  se (Stefan Esser)
 * structure of matprocs table changed
 * 
 * Revision 1.6  1992/05/13  23:34:41  se
 * added description entry for MAT_TXT matrizes
 *
 * Revision 1.5  1992/03/12  17:08:23  se
 * added line defining MAT_MATE to matprocs
 *
 * Revision 1.4  1992/02/19  17:07:35  se
 * general cleanup ...
 *
 * Revision 1.3  1992/02/19  00:04:34  se
 * mat_types now defines access functions instead of making the matproc[]
 * array available
 *
 * Revision 1.2  1992/02/13  14:37:45  se
 * added rcsid[]
 *
 */

#if defined(__STDC__) || defined(__cplusplus)
# define CONST const
#else
# define CONST /* */
#endif

static matprocs matproc [] =
{
/* formats that are easily recognized (eg. by magic number) first	     */
  { MAT_LC,   "lc",   MAT_D_I4S, (mgetf *)lc_get,   (mputf *)lc_put,  lc_probe,	  lc_init    },
#ifdef undef
#ifndef NO_SHM
  { MAT_SHM,  "shm",  MAT_D_I4S, (mgetf *)shm_get,  NULL,    shm_probe,	  NULL       },
#endif /* NO_SHM */
#endif
  { MAT_MATE, "mate", MAT_D_I4S, (mgetf *)mate_get, NULL,    mate_probe,	  NULL       },
  { MAT_TRIXI, "trixi", MAT_D_I2U, (mgetf *)trixi_get, NULL, trixi_probe,  NULL       },
  { MAT_GF2, "gf2", MAT_D_F4, (mgetf *)gf2_get, (mputf *)gf2_put, gf2_probe,  gf2_init       },
  { MAT_HGF2, "hgf2", MAT_D_F4, (mgetf *)gf2_get, (mputf *)gf2_put, gf2_probe,  gf2_init       },

/* formats that may need guessing ...					     */
  { MAT_LE4,  "le4",  MAT_D_I4S, (mgetf *)le4_get,  (mputf *)le4_put, oldmat_probe, oldmat_init},
  { MAT_HE4,  "he4",  MAT_D_I4S, (mgetf *)he4_get,  (mputf *)he4_put, NULL,	  oldmat_init},

  { MAT_LE2,  "le2",  MAT_D_I2U, (mgetf *)le2_get,  (mputf *)le2_put, NULL,	  oldmat_init},
  { MAT_HE2,  "he2",  MAT_D_I2U, (mgetf *)he2_get,  (mputf *)he2_put, NULL,	  oldmat_init},

  { MAT_LE2S, "le2s", MAT_D_I2S, (mgetf *)le2s_get, (mputf *)le2_put, NULL,	  oldmat_init},
  { MAT_HE2S, "he2s", MAT_D_I2S, (mgetf *)he2s_get, (mputf *)he2_put, NULL,	  oldmat_init},

  { MAT_LE4T, "le4t", MAT_D_I4S, (mgetf *)le4t_get, (mputf *)le4t_put, NULL,	  oldmat_init},
  { MAT_HE4T, "he4t", MAT_D_I4S, (mgetf *)he4t_get, (mputf *)he4t_put, NULL,	  oldmat_init},

  { MAT_LE2T, "le2t", MAT_D_I2U, (mgetf *)le2t_get, (mputf *)le2t_put, NULL,	  oldmat_init},
  { MAT_HE2T, "he2t", MAT_D_I2U, (mgetf *)he2t_get, (mputf *)he2t_put, NULL,	  oldmat_init},

  { MAT_LF4,  "lf4",  MAT_D_F4,  (mgetf *)lf4_get,  (mputf *)lf4_put, NULL,	  oldmat_init},
  { MAT_HF4,  "hf4",  MAT_D_F4,  (mgetf *)hf4_get,  (mputf *)hf4_put, NULL,	  oldmat_init},

  { MAT_LF8,  "lf8",  MAT_D_F8,  (mgetf *)lf8_get,  (mputf *)lf8_put, NULL,	  oldmat_init},
  { MAT_HF8,  "hf8",  MAT_D_F8,  (mgetf *)hf8_get,  (mputf *)hf8_put, NULL,	  oldmat_init},

/*
  { MAT_VAXF, "vaxf", MAT_D_F4,  vaxf_get, vaxf_put,NULL,	  oldmat_init},
  { MAT_VAXG, "vaxg", MAT_D_F8,  vaxg_get, vaxg_put,NULL,	  oldmat_init},
*/
  { MAT_TXT,  "txt",  MAT_D_F8,	 (mgetf *)txt_get,  (mputf *)txt_put, txt_probe,	  txt_init   },

/* MAT_INVALID must be the last entry (used internally !)		     */
  { MAT_INVALID,"???",MAT_D_INV, NULL,	    NULL,   NULL,	  NULL       },
};


void matproc_guessfiletype(MFILE *mat) {

  matprocs *p = matproc;
  
  while (p->filetype != MAT_INVALID) {
    mprobef *f = p->mprobe;
    if (f) {
      f (mat);
      if (mat->filetype != MAT_UNKNOWN) return;
    }
    p++;
  }
}


void matproc_init(MFILE *mat) {

  matprocs *p = matproc;
  int mft = mat->filetype;
  int pft;
  
  while ((pft = p->filetype) != MAT_INVALID) {
    if (pft == mft) {
      minitf *f = p->minit;
      if (f) f (mat);
      return;
    }
    p++;
  }
}

char *matproc_fmtname(int mft) {

  matprocs *p = matproc;
  int pft;

  while ((pft = p->filetype) != MAT_INVALID) {
    if (pft == mft) {
      return p->fmtname;
    }
    p++;
  }    
  return "???";
}


int matproc_filetype(CONST char *fmt) {

  matprocs *p = matproc;
  
  while (p->filetype != MAT_INVALID) {
    if (strcmp (p->fmtname, fmt) == 0) {
      return p->filetype;
    }
    p++;
  }
  return MAT_INVALID;
}


int matproc_datatype(int mft) {

  matprocs *p = matproc;
  int pft;

  while ((pft = p->filetype) != MAT_INVALID) {
    if (pft == mft) {
      return p->datatype;
    }
    p++;
  }    
  return MAT_D_INV;
}


mgetf* matproc_getf(int mft) {

  matprocs *p = matproc;
  int pft;

  while ((pft = p->filetype) != MAT_INVALID) {
    if (pft == mft) {
      return p->mget;
    }
    p++;
  }    

  return (mgetf*) NULL;
}


mputf* matproc_putf(int mft) {

  matprocs *p = matproc;
  int pft;

  while ((pft = p->filetype) != MAT_INVALID) {
    if (pft == mft) {
      return p->mput;
    }
    p++;
  }    

  return (mputf*) NULL;
}
