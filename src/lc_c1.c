/*
 * lc_c1.c
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

#include <sys/types.h>
#include "lc_c1.h"

#define encode(i)	(encodetmp = *(src +(i)),			\
			 diff = encodetmp - last, last = encodetmp,	\
			 diff >= 0 ? (diff << 1) : ~(diff << 1))

#define fitsinto(i,n)	(((i) >> n) == 0)

#define put_3_2(a,b,c)	(*dest++ =        (a) + ((b) << 2) + ((c) << 4))

#define put_2_3(a,b)	(*dest++ = 0x40 + (a) + ((b) << 3))

#define put_1_6(a)	(*dest++ = 0x80 + (a))

#define put_1_n(a)	do {	/* a has to be UNsigned int !!! */	\
			  *dest++ = 0xC0 | (a);				\
			  (a) >>= 6;					\
			  while (!fitsinto(a,7)) {			\
			    *dest++ = 0x80 | (a);			\
			    (a) >>= 7;					\
			  }						\
			  *dest++ = (a);				\
			} while (0)


/* liefert Anzahl Bytes nach Kompression oder -1 bei Fehler */

int lc1_compress(char *dest, int *src, int num) {

  int last = 0;
  char *p = dest;
  
  while (num) {
    int diff, encodetmp;
    u_int a;

    a = encode (0);
    if (fitsinto (a, 3) && (num >= 2)) {
      int last_a = last;
      u_int b = encode (1);

      if (fitsinto (a|b, 2) && (num >= 3)) {
	int last_b = last;
        u_int c = encode (2);
	
	if (fitsinto (/*a|b|*/ c, 2)) {
	  put_3_2 (a, b, c); src += 3; num -= 3; continue;
	}
	last = last_b;
      }
      if (fitsinto (/*a|*/ b, 3)) {
	put_2_3 (a, b); src += 2; num -= 2; continue;
      }
      last = last_a;
    }
    if (fitsinto (a, 6)) {
      put_1_6 (a); src++ ; num-- ; continue;
    }
    put_1_n (a); src++ ; num-- ; continue;
  }

  return (dest - p);
}


#define decode(i)	(last += ((i) & 1) ? ~((u_int)(i) >> 1) : (i) >> 1 )

#define bitextract(i,p,l) (((i) >> p) & ((1 << (l)) -1))

int lc1_uncompress(int *dest, char *src, int num) {

  int last = 0;
  int nleft = num;
  
  while (nleft) {
    u_int i;
    u_char t = *src++;
    switch (t >> 6) {
    case 0:				/* 3*2 */
      nleft -= 3;
      if (nleft < 0) return -1;
      {
	i = bitextract (t, 0, 2);
	*dest++ = decode (i);
	i = bitextract (t, 2, 2);
	*dest++ = decode (i);
	i = bitextract (t, 4, 2);
	*dest++ = decode (i);
      }
      break;
    case 1:				/* 2*3 */
      nleft -= 2;
      if (nleft < 0) return -1;
      {
	i = bitextract (t, 0, 3);
	*dest++ = decode (i);
	i = bitextract (t, 3, 3);
	*dest++ = decode (i);
      }
      break;
    case 2:				/* 1*6 */
      nleft--;
      {
	i = bitextract (t, 0, 6);
	*dest++ = decode (i);
      }
      break;
    case 3:				/* 1*N */
      nleft--;
      {
	int s = 6;
	i = bitextract (t, 0, 6);
	do {
	  t = *src++;
	  i += (bitextract (t, 0, 7) << s);
	  s += 7;
	} while (t & 0x80);
	*dest++ = decode (i);
      }
      break;
    }
  }

  return num;
}

unsigned int lc1_comprlinelenmax(unsigned int col) {

  return ((col * 5) +3) & -4;

}

