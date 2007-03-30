/*
 * conversters.c:	Copyright 1992 by 
 *			Stefan Esser
 *			Institut for Nuclear Physics
 *			University of Cologne, Germany
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include "mfile.h"
#include "converters.h"

static char rcsid[] = "$Id: converters.c,v 1.9 1992/07/30 12:09:41 se Exp $";

/* $Log: converters.c,v $
 * Revision 1.9  1992/07/30  12:09:41  se
 * added copyright note
 *
 * Revision 1.8  1992/07/25  14:07:55  se
 * now includes stdlib.h instead of malloc.h
 *
 * Revision 1.7  1992/07/08  21:10:38  se
 * changed parameter of checkconvbuffer from int to unsigned
 *
 * Revision 1.6  1992/07/08  21:03:39  se
 * general cleanup for strictest GCC warning levels
 *
 * Revision 1.5  1992/07/08  18:44:34  se
 * reestablished traditional function declarations for compatibility with
 * non ANSI compilers
 *
 * Revision 1.4  92/04/02  20:56:34  20:56:34  se (Stefan Esser)
 * typo in definition of rcsid corrected
 * 
 * Revision 1.3  1992/03/31  01:54:22  se
 * now includes malloc.h
 *
 * Revision 1.2  1992/02/18  23:21:35  se
 * changed installconverters to only install a converter if no function
 * had previously installed for this format
 *
 * Revision 1.1  1992/02/17  23:11:31  se
 * Initial revision
 *
 */

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#define CONST const
#else
# define P_(s) ()
#define CONST /* */
#endif

static void checkconvbuffer P_((unsigned int size));

static int conv_int_to_dbl P_((double *dst, CONST int    *src, int num));
static int conv_flt_to_dbl P_((double *dst, CONST float  *src, int num));
static int conv_int_to_flt P_((float  *dst, CONST int    *src, int num));
static int conv_dbl_to_flt P_((float  *dst, CONST double *src, int num));
static int conv_flt_to_int P_((int    *dst, CONST float  *src, int num));
static int conv_dbl_to_int P_((int    *dst, CONST double *src, int num));

static int mgetint_via_flt P_((MFILE *mat, int *b, int v, int l, int c, int n));
     
static int mgetint_via_dbl P_((MFILE *mat, int *b, int v, int l, int c, int n));
     
static int mgetflt_via_int P_((MFILE *mat, float *b, int v, int l, int c, int n));
static int mgetflt_via_dbl P_((MFILE *mat, float *b, int v, int l, int c, int n));
static int mgetdbl_via_int P_((MFILE *mat, double *b, int v, int l, int c, int n));
static int mgetdbl_via_flt P_((MFILE *mat, double *b, int v, int l, int c, int n));

static int mputint_via_flt P_((MFILE *mat, int *b, int v, int l, int c, int n));
static int mputint_via_dbl P_((MFILE *mat, int *b, int v, int l, int c, int n));
static int mputflt_via_int P_((MFILE *mat, float *b, int v, int l, int c, int n));
static int mputflt_via_dbl P_((MFILE *mat, float *b, int v, int l, int c, int n));
static int mputdbl_via_int P_((MFILE *mat, double *b, int v, int l, int c, int n));
static int mputdbl_via_flt P_((MFILE *mat, double *b, int v, int l, int c, int n));

#undef P_

/*------------------------------------------------------------------------*/


static int conv_int_to_dbl (dst, src, num)
     double *dst;
     CONST int *src;
     int num;
{
  int i;
  for (i = 0; i < num; i++) {
    *dst++ = (double) *src++;
  }    
  return num;
}

static int conv_flt_to_dbl (dst, src, num)
     double *dst;
     CONST float *src;
     int num;
{
  int i;
  for (i = 0; i < num; i++) {
    *dst++ = (double) *src++;
  }    
  return num;
}

static int conv_int_to_flt (dst, src, num)
     float *dst;
     CONST int *src;
     int num;
{
  int i;
  for (i = 0; i < num; i++) {
    *dst++ = (float) *src++;
  }    
  return num;
}

static int conv_dbl_to_flt (dst, src, num)
     float *dst;
     CONST double *src;
     int num;
{
  int i;
  for (i = 0; i < num; i++) {
    *dst++ = (float) *src++;
  }    
  return num;
}

static int conv_flt_to_int (dst, src, num)
     int *dst;
     CONST float *src;
     int num;
{
  int i;
  for (i = 0; i < num; i++) {
    float tmp = *src++;
    *dst++ = (int) (tmp > 0) ? tmp + 0.5 : tmp - 0.5;
  }    
  return num;
}

static int conv_dbl_to_int (dst, src, num)
     int *dst;
     CONST double *src;
     int num;
{
  int i;
  for (i = 0; i < num; i++) {
    double tmp = *src++;
    *dst++ = (int) (tmp > 0) ? tmp + 0.5 : tmp - 0.5;
  }    
  return num;
}


/*------------------------------------------------------------------------*/


static void *mgetconvbuf = NULL;
static int mgetconvbufsize = 0;


static void checkconvbuffer (size)
     unsigned int size;
{
  if (mgetconvbufsize < size) {
    if (mgetconvbuf) free (mgetconvbuf);  
    mgetconvbuf = (void *)malloc (size);
    mgetconvbufsize = (mgetconvbuf != NULL) ? size : -1;
  }
}

static int mgetint_via_flt (mat, b, v, l, c, n)
     MFILE *mat;
     int *b;
     int v, l, c;
     int n;
{
  int num;
  checkconvbuffer (n * sizeof (float));
  num = mgetflt (mat, (float *)mgetconvbuf, v, l, c, n);
  return conv_flt_to_int (b, (float *)mgetconvbuf, num);
}

static int mgetint_via_dbl (mat, b, v, l, c, n)
     MFILE *mat;
     int *b;
     int v, l, c;
     int n;
{
  int num;
  checkconvbuffer (n * sizeof (double));
  num = mgetdbl (mat, (double *)mgetconvbuf, v, l, c, n);
  return conv_dbl_to_int (b, (double *)mgetconvbuf, num);
}

static int mgetflt_via_int (mat, b, v, l, c, n)
     MFILE *mat;
     float *b;
     int v, l, c;
     int n;
{
  int num;
  checkconvbuffer (n * sizeof (int));
  num = mgetint (mat, (int *)mgetconvbuf, v, l, c, n);
  return conv_int_to_flt (b, (int *)mgetconvbuf, num);
}

static int mgetflt_via_dbl (mat, b, v, l, c, n)
     MFILE *mat;
     float *b;
     int v, l, c;
     int n;
{
  int num;
  checkconvbuffer (n * sizeof (double));
  num = mgetdbl (mat, (double *)mgetconvbuf, v, l, c, n);
  return conv_dbl_to_flt (b, (double *)mgetconvbuf, num);
}

static int mgetdbl_via_int (mat, b, v, l, c, n)
     MFILE *mat;
     double *b;
     int v, l, c;
     int n;
{
  int num;
  checkconvbuffer (n * sizeof (int));
  num = mgetint (mat, (int *)mgetconvbuf, v, l, c, n);
  return conv_int_to_dbl (b, (int *)mgetconvbuf, num);
}

static int mgetdbl_via_flt (mat, b, v, l, c, n)
     MFILE *mat;
     double *b;
     int v, l, c;
     int n;
{
  int num;
  checkconvbuffer (n * sizeof (float));
  num = mgetflt (mat, (float *)mgetconvbuf, v, l, c, n);
  return conv_flt_to_dbl (b, (float *)mgetconvbuf, num);
}


/*------------------------------------------------------------------------*/


static int mputint_via_flt (mat, b, v, l, c, n)
     MFILE *mat;
     int *b;
     int v, l, c;
     int n;
{
  int num;
  checkconvbuffer (n * sizeof (float));
  num = conv_int_to_flt ((float *)mgetconvbuf, b, n);
  return mputflt (mat, (float *)mgetconvbuf, v, l, c, num);
}

static int mputint_via_dbl (mat, b, v, l, c, n)
     MFILE *mat;
     int *b;
     int v, l, c;
     int n;
{
  int num;
  checkconvbuffer (n * sizeof (double));
  num = conv_int_to_dbl ((double *)mgetconvbuf, b, n);
  return mputdbl (mat, (double *)mgetconvbuf, v, l, c, num);
}

static int mputflt_via_int (mat, b, v, l, c, n)
     MFILE *mat;
     float *b;
     int v, l, c;
     int n;
{
  int num;
  checkconvbuffer (n * sizeof (int));
  num = conv_flt_to_int ((int *)mgetconvbuf, b, n);
  return mputint (mat, (int *)mgetconvbuf, v, l, c, num);
}

static int mputflt_via_dbl (mat, b, v, l, c, n)
     MFILE *mat;
     float *b;
     int v, l, c;
     int n;
{
  int num;
  checkconvbuffer (n * sizeof (double));
  num = conv_flt_to_dbl ((double *)mgetconvbuf, b, n);
  return mputdbl (mat, (double *)mgetconvbuf, v, l, c, num);
}

static int mputdbl_via_int (mat, b, v, l, c, n)
     MFILE *mat;
     double *b;
     int v, l, c;
     int n;
{
  int num;
  checkconvbuffer (n * sizeof (int));
  num = conv_dbl_to_int ((int *)mgetconvbuf, b, n);
  return mputint (mat, (int *)mgetconvbuf, v, l, c, num);
}

static int mputdbl_via_flt (mat, b, v, l, c, n)
     MFILE *mat;
     double *b;
     int v, l, c;
     int n;
{
  int num;
  checkconvbuffer (n * sizeof (float));
  num = conv_dbl_to_flt ((float *)mgetconvbuf, b, n);
  return mputflt (mat, (float *)mgetconvbuf, v, l, c, num);
}


/*------------------------------------------------------------------------*/


#define setf(fp,fun) if (mat->fp == NULL) mat->fp = fun

void installconverters (mat)
     MFILE *mat;
{
  if (mat->mgeti4f) {
    setf (mgetf4f, mgetflt_via_int);
    setf (mgetf8f, mgetdbl_via_int);
  } else if (mat->mgetf4f) {
    setf (mgeti4f, mgetint_via_flt);
    setf (mgetf8f, mgetdbl_via_flt);
  } else if (mat->mgetf8f) {
    setf (mgeti4f, mgetint_via_dbl);
    setf (mgetf4f, mgetflt_via_dbl);
  }
  if (mat->mputi4f) {
    setf (mputf4f, mputflt_via_int);
    setf (mputf8f, mputdbl_via_int);
  } else if (mat->mputf4f) {
    setf (mputi4f, mputint_via_flt);
    setf (mputf8f, mputdbl_via_flt);
  } else if (mat->mputf8f) {
    setf (mputi4f, mputint_via_dbl);
    setf (mputf4f, mputflt_via_dbl);
  }
}

#undef setf
