/*
 * lc_c2.c:		Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include "lc_c2.h"

static char rcsid[] = "$Id: lc_c2.c,v 1.5 1992/07/30 12:09:41 se Exp $";

/* $Log: lc_c2.c,v $
 * Revision 1.5  1992/07/30  12:09:41  se
 * added copyright note
 *
 * Revision 1.4  1992/07/25  14:09:29  se
 * decode macro now correctly casts all references to its parameter to
 * unsigned (results in logical instead of arith. shifts)
 *
 * Revision 1.3  1992/07/08  21:03:39  se
 * general cleanup for strictest GCC warning levels
 *
 * Revision 1.2  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.1  92/06/30  14:37:03  14:37:03  se (Stefan Esser)
 * Initial revision
 * 
 *
 */


#define encode(i)	((encodetmp = i) >= 0 ? (encodetmp << 1) : ~(encodetmp << 1))

#define fitsinto(i,n)	(((i) >> n) == 0)

#define put_3_2(a,b,c)	(*dest++ =        (a) + ((b) << 2) + ((c) << 4))

#define put_2_3(a,b)	(*dest++ = 0x40 + (a) + ((b) << 3))

#define put_tag_n(T,a)	do {						\
			  unsigned int t = a;				\
			  if (t <= 59) { *dest++ = T + t; }		\
			  else {					\
			    char *tag = dest++;				\
			    int bytes = 0;				\
			    t -= 60;					\
			    *dest++ = (char) t;				\
			    while (t = t >>8) {				\
			      *dest++ = (char) --t;			\
			      bytes++;					\
			    }						\
			    *tag = T + 60 + bytes;			\
			  }						\
			} while (0)


#define put_1_n(a)	   put_tag_n (0x80, a)

#define put_same_diff(s,d) put_tag_n (0xc0, (s << 1) + d)


/* liefert Anzahl Bytes nach Kompression oder -1 bei Fehler */

int lc2_compress (dest, src, num)
     char *dest;
     int *src;
     int num;
{
  int last = 0;
  char *p = dest;
  
  while (num) {
    unsigned int d = *src - last;
    int same = 0;
    int i = 1;

    if (d < 2) {
      while (i < num && *(src +i) == last) i++;
    }
    same = i -1;

    if (same >= 3) {

      put_same_diff((same -3), d);
      src += i;
      num -= i;

    } else {

      int encodetmp;
      unsigned int a;
      int s0 = *src;

      a = encode (s0 -last);

      if (fitsinto (a, 3) && (num >= 2)) {
        int s1 = *(src+1);
        unsigned int b = encode (s1 -last);

        if (fitsinto (a|b, 2) && (num >= 3)) {
	  int s2 = *(src+2);
	  unsigned int c = encode (s2 -last);
	
	  if (fitsinto (/*a|b|*/ c, 2)) {
	    put_3_2 (a, b, c); src += 3; num -= 3; last = s2; continue;
	  }
	}
	if (fitsinto (/*a|*/ b, 3)) {
	  put_2_3 (a, b); src += 2; num -= 2; last = s1; continue;
	}
      }
      put_1_n (a); src++ ; num-- ; last = s0; continue;
    }
  }
  return (dest - p);
}


#define decode(i)	( ((i) & 1) ? ~((unsigned int)(i) >> 1)		\
				    :  ((unsigned int)(i) >> 1) )

#define bitextract(i,p,l) (((i) >> p) & ((1 << (l)) -1))

int lc2_uncompress (dest, src, num) 
     int *dest;
     char *src;
     int num;
{
  int last = 0;
  int nleft = num;
  
  while (nleft) {
    unsigned int i;
    unsigned char t = *src++;

    if (t & 0x80) {

      int n = t & 0x3f;
      if (n > 59) {
	int bytes = n - 59;
	int i;
	n = 59;
	for (i = 0; i < bytes; i++) {
	  unsigned char b = *src++;
	  int s = i << 3;
	  n += (b + 1) << s;
	}
      }
      if (t & 0x40) {
        int diff = bitextract (n, 0, 1);
	int same = (n >> 1) +3;
	*dest++ = last + diff;
	nleft -= same;
	if (nleft <= 0) return -1;
	while (same--) *dest++ = last;
      } else {
        *dest++ = (last += decode(n));
      }
      nleft--;

    } else if (t & 0x40) {

      nleft -= 2;
      if (nleft < 0) return -1;
      {
	i = bitextract (t, 0, 3);
	*dest++ = last + decode (i);
	i = bitextract (t, 3, 3);
	*dest++ = (last += decode (i));
      }

    } else {

      nleft -= 3;
      if (nleft < 0) return -1;
      {
	i = bitextract (t, 0, 2);
	*dest++ = last + decode (i);
	i = bitextract (t, 2, 2);
	*dest++ = last + decode (i);
	i = bitextract (t, 4, 2);
	*dest++ = (last += decode (i));
      }
    }

  }
  return num;
}

unsigned int lc2_comprlinelenmax (col)
     unsigned int col;
{
  return ((col * 5) +3) & -4;
}

