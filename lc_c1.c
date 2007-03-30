/*
 * lc_c1.c:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <sys/types.h>
#include "lc_c1.h"

static char rcsid[] = "$Id: lc_c1.c,v 1.8 1992/07/30 12:09:41 se Exp $";

/* $Log: lc_c1.c,v $
 * Revision 1.8  1992/07/30  12:09:41  se
 * added copyright note
 *
 * Revision 1.7  1992/07/08  21:03:39  se
 * general cleanup for strictest GCC warning levels
 *
 * Revision 1.6  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.5  92/06/30  14:36:08  14:36:08  se (Stefan Esser)
 * changed some types from unsigned to signed char and int
 * 
 * Revision 1.4  1992/03/31  01:57:06  se
 * changed some occurances of | (bitwise OR) into + (ADD)
 *
 * Revision 1.3  1992/03/17  15:06:45  se
 * changed type of lc_compress() to return int instead of u_int.
 * return codes are -1 in case of error, nuber of bytes after compression else
 *
 * Revision 1.2  1992/02/13  14:37:45  se
 * added rcsid[]
 *
 */


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

int lc1_compress (dest, src, num)
     char *dest;
     int *src;
     int num;
{
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

int lc1_uncompress (dest, src, num) 
     int *dest;
     char *src;
     int num;
{
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

unsigned int lc1_comprlinelenmax (col)
     unsigned int col;
{
  return ((col * 5) +3) & -4;
}

