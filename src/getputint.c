/*
 * getputint.c
 */
/*
 * Copyright (c) 1992-2008, Stefan Esser <se@ikp.uni-koeln.de>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *	* Redistributions of source code must retain the above copyright notice, 
 *	  this list of conditions and the following disclaimer.
 * 	* Redistributions in binary form must reproduce the above copyright notice, 
 * 	  this list of conditions and the following disclaimer in the documentation 
 * 	  and/or other materials provided with the distribution.
 *    
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "mfile.h"
#include "sys_endian.h"
#include "getputint.h"
#include "maccess.h"
#include <errno.h>
//#include "config.h"


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

