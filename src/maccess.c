/*
 * maccess.c:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include "mfile.h"
#include "sys_endian.h"
#include "maccess.h"
#include "disk_access.h"
#ifndef NO_SHM
#include "shm_access.h"
#endif

#include <errno.h>
#include <malloc.h>
#include <string.h>



/* $Log: maccess.c,v $
 * Revision 1.1  1993/07/20  17:56:58  se
 * Initial revision
 *
 *
 */

/* ------------------------------------------------------------------------- */

static amp newaccessmethod();
static amp initaccessmethod P_((amp ap, char *name));
static void *dummy_geta P_((amp ap, acc_pos offset, acc_pos nbytes));
static void *dummy_puta P_((amp ap, acc_pos offset, acc_pos nbytes));
static int dummy_flush P_((amp ap));
static int dummy_close P_((amp ap));
static int get_via_geta P_((amp ap, void *buffer, acc_pos offset, acc_pos nbytes));
static int put_via_puta P_((amp ap, void *buffer, acc_pos offset, acc_pos nbytes));
static void *geta_via_get P_((amp ap, acc_pos offset, acc_pos nbytes));
static void *puta_via_put P_((amp ap, acc_pos offset, acc_pos nbytes));

/* ------------------------------------------------------------------------- */

#ifdef NO_SHM
static maccessdescr disk_access = { disk_tryaccess, NULL, NULL };
maccessdescr *tryaccess_first   = &disk_access;
#else
static maccessdescr disk_access = { disk_tryaccess, NULL, NULL };
static maccessdescr shm_access  = { shm_tryaccess, "shm", &disk_access };
maccessdescr *tryaccess_first   = &shm_access;
#endif

/* ------------------------------------------------------------------------- */

static int dummy_flush(amp ap) {
  return 0;
}

static int dummy_close(amp ap) {
  return 0;
}

static void *dummy_geta(amp ap, acc_pos offset, acc_pos nbytes) {
  return (void*) NULL;
}

static void *dummy_puta(amp ap, acc_pos offset, acc_pos nbytes) {
  return (void*) NULL;
}

/* ------------------------------------------------------------------------- */

static int get_via_geta(amp ap, void *buffer, acc_pos offset, acc_pos nbytes) {

  void* src = _geta (ap, offset, nbytes);
  if (src)
  {
    memcpy (buffer, src, nbytes);
    return nbytes;
  }

  return 0;
}

static int put_via_puta(amp ap, void *buffer, acc_pos offset, acc_pos nbytes) {

  void* dest = _puta (ap, offset, nbytes);
  if (dest)
  {
    memcpy (dest, buffer, nbytes);
    return nbytes;
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
/* PROVISORISCH !!! noch nicht implementiert */

static void *geta_via_get(amp ap, acc_pos offset, acc_pos nbytes) {
  return (void*) NULL;
}

static void *puta_via_put(amp ap, acc_pos offset, acc_pos nbytes) {
  return (void*) NULL;
}

/* ------------------------------------------------------------------------- */

int maddaccess (     tryaccessf *tryaccess, char *name) {

  maccessdescr *p = (maccessdescr*) malloc (sizeof (maccessdescr));
  if (p)
  {
    p -> tryaccess	= tryaccess;
    p -> name		= name;
    p -> next		= tryaccess_first;
    tryaccess_first	= p;
    return 0;
  }

  return -1;
}

/* ------------------------------------------------------------------------- */

static amp newaccessmethod()
{
  amp ap = (amp) malloc (sizeof(accessmethod));
  if (ap) memset (ap, 0, sizeof(*ap));
  return ap;
}

static amp initaccessmethod(amp ap, char *name) {

  if (name)
  {
    ap->name = malloc (strlen(name) +1);
    if (ap->name) strcpy (ap->name, name);
  }

  if (!ap->geta) ap->geta = ap->get ? geta_via_get : dummy_geta;
  if (!ap->get)  ap->get  = get_via_geta;

  if (!ap->puta) ap->puta = ap->put ? puta_via_put : dummy_puta;
  if (!ap->put)  ap->put  = put_via_puta;

  if (!ap->flush) ap->flush = dummy_flush;
  if (!ap->close) ap->close = dummy_close;

  return ap;
}


amp tryaccess(char *name, char *mode, char *accessname) {

  amp ap = newaccessmethod();

  if (ap)
  {
    maccessdescr *p = tryaccess_first;

    if (accessname && !*accessname) accessname = NULL;
    while (p)
    {
      if (!accessname 
	  || (p->name && p->name[0] && (strcmp(accessname, p->name) == 0) ))
      {
	if (p->tryaccess(ap, name, mode) == 0) 
	  return initaccessmethod(ap, name);
      }
      p = p->next;
    }
  }
  free (ap);

  return NULL;
}
