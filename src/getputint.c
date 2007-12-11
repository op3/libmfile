/*
 * getputint.c:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include "mfile.h"
#include "sys_endian.h"
#include "getputint.h"
#include "maccess.h"
#include <errno.h>
#include "config.h"

/* $Log: getputint.c,v $
 * Revision 1.9  1993/07/20  17:55:46  se
 * modified for use with access method functions
 *
 * Revision 1.8  1992/07/30  12:09:41  se
 * added copyright note
 *
 * Revision 1.7  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.6  92/07/08  06:21:53  06:21:53  se (Stefan Esser)
 * changed 2nd parameter to get and put from char* to void* and introduced
 * local char *buffer for actual accesses
 * 
 * Revision 1.5  1992/05/13  23:28:17  se
 * changed type of parameter 'buffer' to get and put back to char *
 * because of compiler complaints about unknown size of *buffer
 *
 * Revision 1.4  1992/03/31  01:55:01  se
 * changed type of buffer from char* to void*
 *
 * Revision 1.3  1992/02/15  17:22:42  se
 * new functions for high and low endian 8 byte values {get,put}_{l,h}e8()
 *
 * Revision 1.2  1992/02/13  14:37:45  se
 * added rcsid[]
 *
 */

#ifdef undef
int get(int fd, void *adr, u_int pos, u_int num) {

  int w;
  int done = 0;
  char *buffer = adr;

  errno = 0;
  
  if (lseek (fd, pos, SEEK_SET) != pos) {
    PERROR("lseek");
    return -1;
  }

  do {
    w = read(fd, buffer, num);
    if (w > 0){
      done	+= w;
      buffer	+= w;
      num	-= w;
    } else {
#ifndef OSK
      if (errno && errno != EINTR) {
#else
      if (errno) {
#endif
#ifdef undef
        int saveerrno = errno;
	(void) close(fd);
	errno = saveerrno;
	PERROR("get");		/* ERROR ABORT */
	exit (1);		
#endif
	return -1;
      } else {
        num =0;
      }
    }
  } while (num > 0);

  return done;
}


int put(int fd, void *adr, u_int pos, u_int num) {

  int w;
  int done = 0;
  char *buffer = adr;

  errno = 0;
  
  if (lseek (fd, pos, SEEK_SET) != pos) {
    PERROR("lseek");
    return -1;
  }

  do {
    w = write(fd, buffer, num);
    if (w > 0){
      done	+= w;
      buffer	+= w;
      num	-= w;
    } else {
#ifndef OSK
      if (errno && errno != EINTR) {
#else
      if (errno) {
#endif
#ifdef undef
        int saveerrno = errno;
	(void) close(fd);
	errno = saveerrno;
	PERROR ("get");		/* ERROR ABORT */
	exit (1);	
#endif
	return -1;
      } else {
        num =0;
      }
    }
  } while (num > 0);

  return done;
}
#endif /* undef */


u_int getle8(amp ap, int *buffer, u_int pos, u_int num) {
     
#ifdef LOWENDIAN
  int *iobuf = buffer;
#else
  u_int iobuf[2*MAT_COLMAX], *p;
#endif

  u_int n = num << 3;
  if (_get (ap, (char *)iobuf, pos, n) != n) return 0;
#ifndef LOWENDIAN
  p = iobuf;
  for (n = num; n ; n--) {
    register int t1 = *(p++);
    register int t2 = *(p++);
    *(buffer++) = GETLE4(t2);
    *(buffer++) = GETLE4(t1);
  }
#endif

  return num;
}
     
u_int putle8(amp ap, int *buffer, u_int pos, u_int num) {

  u_int n;
#ifdef LOWENDIAN
  int *iobuf = buffer;
#else
  u_int iobuf[2*MAT_COLMAX], *p = iobuf;

  for (n = num; n ; n--) {
    register int t1 = *(buffer++);
    register int t2 = *(buffer++);
    *(p++) = GETLE4(t2);
    *(p++) = GETLE4(t1);
  }
#endif
  n = num << 3;
  if (_put (ap, (char *)iobuf, pos, n) != n) return 0;
  return num;
}

u_int gethe8(amp ap, int *buffer, u_int pos, u_int num) {     

#ifndef LOWENDIAN
  int *iobuf = buffer;
#else
  u_int iobuf[2*MAT_COLMAX], *p;
#endif

  u_int n = num << 3;
  if (_get (ap, (char *)iobuf, pos, n) != n) return 0;
#ifdef LOWENDIAN
  p = iobuf;
  for (n = num; n ; n--) {
    register int t1 = *(p++);
    register int t2 = *(p++);
    *(buffer++) = GETHE4(t2);
    *(buffer++) = GETHE4(t1);
  }
#endif

  return num;
}
     
u_int puthe8(amp ap, int *buffer, u_int pos, u_int num) {

  u_int n;
#ifndef LOWENDIAN
  int *iobuf = buffer;
#else
  u_int iobuf[2*MAT_COLMAX], *p = iobuf;

  for (n = num; n ; n--) {
    register int t1 = *(buffer++);
    register int t2 = *(buffer++);
    *(p++) = GETHE4(t2);
    *(p++) = GETHE4(t1);
  }
#endif
  n = num << 3;
  if (_put (ap, (char *)iobuf, pos, n) != n) return 0;
  return num;
}

u_int getle4 (amp ap, int *buffer, u_int pos, u_int num) {
     
#ifdef LOWENDIAN
  int *iobuf = buffer;
#else
  u_int iobuf[MAT_COLMAX], *p;
#endif

  u_int n = num << 2;
  if (_get (ap, (char *)iobuf, pos, n) != n) return 0;
#ifndef LOWENDIAN
  for (p = iobuf, n = num; n ; n--) {
    register int t = *(p++);
    *(buffer++) = GETLE4(t);
  }
#endif

  return num;
}
     
u_int putle4 (amp ap, int *buffer, u_int pos, u_int num) {

  u_int n;
#ifdef LOWENDIAN
  int *iobuf = buffer;
#else
  u_int iobuf[MAT_COLMAX], *p;

  for (p = iobuf, n = num; n ; n--) {
    register int t = *(buffer++);
    *(p++) = GETLE4(t);
  }
#endif
  n = num << 2;
  if (_put (ap, (char *)iobuf, pos, n) != n) return 0;

  return num;
}

u_int gethe4(amp ap, int *buffer, u_int pos, u_int num) {

#ifndef LOWENDIAN
  int *iobuf = buffer;
#else
  u_int iobuf[MAT_COLMAX], *p;
#endif

  u_int n = num << 2;
  if (_get (ap, (char *)iobuf, pos, n) != n) return 0;
#ifdef LOWENDIAN
  for (p = iobuf, n = num; n ; n--) {
    register int t = *(p++);
    *(buffer++) = GETHE4(t);
  }
#endif
  return num;
}

u_int puthe4(amp ap, int *buffer, u_int pos, u_int num) {

  u_int n;
#ifndef LOWENDIAN
  int *iobuf = buffer;
#else
  u_int iobuf[MAT_COLMAX], *p;

  for (p = iobuf, n = num; n ; n--) {
    register int t = *(buffer++);
    *(p++) = GETHE4(t);
  }
#endif
  n = num << 2;
  if (_put (ap, (char *)iobuf, pos, n) != n) return 0;
  return num;
}

u_int getle2(amp ap, int *buffer, u_int pos, u_int num) {

  u_short iobuf[MAT_COLMAX], *p;
  u_int n = num << 1;

  if (_get (ap, (char *)iobuf, pos, n) != n) return 0;
  for (p = iobuf, n = num; n ; n--) {
    register int t = *(p++);
    *(buffer++) = (u_short) GETLE2(t);
  }

  return num;
}

u_int putle2(amp ap, int *buffer, u_int pos, u_int num) {

  u_short iobuf[MAT_COLMAX], *p;
  u_int n;

  for (p = iobuf, n = num; n ; n--) {
    register u_int t = *(buffer++);
    *(p++) = GETLE2(t);
  }
  n = num << 1;
  if (_put (ap, (char *)iobuf, pos, n) != n) return 0;

  return num;
}

u_int gethe2(amp ap, int *buffer, u_int pos, u_int num) {

  u_short iobuf[MAT_COLMAX], *p;
  u_int n = num << 1;

  if (_get (ap, (char *)iobuf, pos, n) != n) return 0;
  for (p = iobuf, n = num; n ; n--) {
    register int t = *(p++);
    *(buffer++) = (u_short) GETHE2(t);
  }

  return num;
}

u_int puthe2(amp ap, int *buffer, u_int pos, u_int num) {

  u_short iobuf[MAT_COLMAX], *p;
  u_int n;

  for (p = iobuf, n = num; n ; n--) {
    register u_int t = *(buffer++);
    *(p++) = GETHE2(t);
  }
  n = num << 1;
  if (_put (ap, (char *)iobuf, pos, n) != n) return 0;

  return num;
}

u_int getle2s(amp ap, int *buffer, u_int pos, u_int num) {

  short iobuf[MAT_COLMAX], *p;
  u_int n = num << 1;

  if (_get (ap, (char *)iobuf, pos, n) != n) return 0;
  for (p = iobuf, n = num; n ; n--) {
    register int t = *(p++);
    *(buffer++) = (short) GETLE2(t);
  }

  return num;
}

u_int gethe2s(amp ap, int *buffer, u_int pos, u_int num) {

  u_short iobuf[MAT_COLMAX], *p;
  u_int n = num << 1;

  if (_get (ap, (char *)iobuf, pos, n) != n) return 0;
  for (p = iobuf, n = num; n ; n--) {
    register int t = *(p++);
    *(buffer++) = (short) GETHE2(t);
  }

  return num;
}

