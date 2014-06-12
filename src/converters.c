/*
 * conversters.c
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

#include <unistd.h>
#include <stdlib.h>
#include "mfile.h"
#include "converters.h"
#include "debug.h"

static void checkconvbuffer(unsigned int size);

static int conv_int_to_dbl(double *dst, const int    *src, int num);
static int conv_flt_to_dbl(double *dst, const float  *src, int num);
static int conv_int_to_flt(float  *dst, const int    *src, int num);
static int conv_dbl_to_flt(float  *dst, const double *src, int num);
static int conv_flt_to_int(int    *dst, const float  *src, int num);
static int conv_dbl_to_int(int    *dst, const double *src, int num);

static int mgetint_via_flt(MFILE *mat, int *b, int v, int l, int c, int n);
     
static int mgetint_via_dbl(MFILE *mat, int *b, int v, int l, int c, int n);
     
static int mgetflt_via_int(MFILE *mat, float *b, int v, int l, int c, int n);
static int mgetflt_via_dbl(MFILE *mat, float *b, int v, int l, int c, int n);
static int mgetdbl_via_int(MFILE *mat, double *b, int v, int l, int c, int n);
static int mgetdbl_via_flt(MFILE *mat, double *b, int v, int l, int c, int n);

static int mputint_via_flt(MFILE *mat, int *b, int v, int l, int c, int n);
static int mputint_via_dbl(MFILE *mat, int *b, int v, int l, int c, int n);
static int mputflt_via_int(MFILE *mat, float *b, int v, int l, int c, int n);
static int mputflt_via_dbl(MFILE *mat, float *b, int v, int l, int c, int n);
static int mputdbl_via_int(MFILE *mat, double *b, int v, int l, int c, int n);
static int mputdbl_via_flt(MFILE *mat, double *b, int v, int l, int c, int n);

/*------------------------------------------------------------------------*/


static int conv_int_to_dbl(double *dst, const int *src, int num) {

  int i;
  for (i = 0; i < num; i++) {
    *dst++ = (double) *src++;
  }    
  return num;
}

static int conv_flt_to_dbl(double *dst, const float *src, int num) {

  int i;
  for (i = 0; i < num; i++) {
    *dst++ = (double) *src++;
  }    
  return num;
}

static int conv_int_to_flt(float *dst, const int *src, int num) {

  int i;
  for (i = 0; i < num; i++) {
    *dst++ = (float) *src++;
  }    
  return num;
}

static int conv_dbl_to_flt(float *dst, const double *src, int num) {

  int i;
  for (i = 0; i < num; i++) {
    *dst++ = (float) *src++;
  }    
  return num;
}

static int conv_flt_to_int(int *dst, const float *src, int num) {

  int i;
  for (i = 0; i < num; i++) {
    float tmp = *src++;
    *dst++ = (int) (tmp > 0) ? tmp + 0.5 : tmp - 0.5;
  }    
  return num;
}

static int conv_dbl_to_int(int *dst, const double *src, int num) {

  int i;
  for (i = 0; i < num; i++) {
    double tmp = *src++;
    *dst++ = (int) (tmp > 0) ? tmp + 0.5 : tmp - 0.5;
  }    
  return num;
}


/*------------------------------------------------------------------------*/


static void *mgetconvbuf = NULL;
static unsigned int mgetconvbufsize = 0;


static void checkconvbuffer(unsigned int size) {

  if (mgetconvbufsize < size) {
    if (mgetconvbuf) free (mgetconvbuf);  
    mgetconvbuf = (void *)malloc (size);
    if(mgetconvbuf == NULL) {
      PERROR("malloc");
    }
    mgetconvbufsize = (mgetconvbuf != NULL) ? size : 0;
  }

}

static int mgetint_via_flt(MFILE *mat, int *b, int v, int l, int c, int n) {

  int num;

  checkconvbuffer (n * sizeof (float));
  num = mgetflt (mat, (float *)mgetconvbuf, v, l, c, n);

  return conv_flt_to_int (b, (float *)mgetconvbuf, num);

}

static int mgetint_via_dbl(MFILE *mat, int *b, int v, int l, int c, int n) {

  int num;

  checkconvbuffer (n * sizeof (double));
  num = mgetdbl (mat, (double *)mgetconvbuf, v, l, c, n);

  return conv_dbl_to_int (b, (double *)mgetconvbuf, num);

}

static int mgetflt_via_int(MFILE *mat, float *b, int v, int l, int c, int n) {

  int num;

  checkconvbuffer (n * sizeof (int));
  num = mgetint (mat, (int *)mgetconvbuf, v, l, c, n);

  return conv_int_to_flt (b, (int *)mgetconvbuf, num);

}

static int mgetflt_via_dbl(MFILE *mat, float *b, int v, int l, int c, int n) {

  int num;

  checkconvbuffer (n * sizeof (double));
  num = mgetdbl (mat, (double *)mgetconvbuf, v, l, c, n);

  return conv_dbl_to_flt (b, (double *)mgetconvbuf, num);

}

static int mgetdbl_via_int(MFILE *mat, double *b, int v, int l, int c, int n) {

  int num;

  checkconvbuffer (n * sizeof (int));
  num = mgetint (mat, (int *)mgetconvbuf, v, l, c, n);

  return conv_int_to_dbl (b, (int *)mgetconvbuf, num);

}

static int mgetdbl_via_flt(MFILE *mat, double *b, int v, int l, int c, int n) {

  int num;

  checkconvbuffer (n * sizeof (float));
  num = mgetflt (mat, (float *)mgetconvbuf, v, l, c, n);

  return conv_flt_to_dbl (b, (float *)mgetconvbuf, num);

}


/*------------------------------------------------------------------------*/


static int mputint_via_flt(MFILE *mat, int *b, int v, int l, int c, int n) {

  int num;

  checkconvbuffer (n * sizeof (float));
  num = conv_int_to_flt ((float *)mgetconvbuf, b, n);

  return mputflt (mat, (float *)mgetconvbuf, v, l, c, num);

}

static int mputint_via_dbl(MFILE *mat, int *b, int v, int l, int c, int n) {

  int num;

  checkconvbuffer (n * sizeof (double));
  num = conv_int_to_dbl ((double *)mgetconvbuf, b, n);

  return mputdbl (mat, (double *)mgetconvbuf, v, l, c, num);

}

static int mputflt_via_int(MFILE *mat, float *b, int v, int l, int c, int n) {

  int num;

  checkconvbuffer (n * sizeof (int));
  num = conv_flt_to_int ((int *)mgetconvbuf, b, n);

  return mputint (mat, (int *)mgetconvbuf, v, l, c, num);

}

static int mputflt_via_dbl(MFILE *mat, float *b, int v, int l, int c, int n) {

  int num;

  checkconvbuffer (n * sizeof (double));
  num = conv_flt_to_dbl ((double *)mgetconvbuf, b, n);

  return mputdbl (mat, (double *)mgetconvbuf, v, l, c, num);

}

static int mputdbl_via_int(MFILE *mat, double *b, int v, int l, int c, int n) {

  int num;

  checkconvbuffer (n * sizeof (int));
  num = conv_dbl_to_int ((int *)mgetconvbuf, b, n);

  return mputint (mat, (int *)mgetconvbuf, v, l, c, num);

}

static int mputdbl_via_flt(MFILE *mat, double *b, int v, int l, int c, int n) {

  int num;

  checkconvbuffer (n * sizeof (float));
  num = conv_dbl_to_flt ((float *)mgetconvbuf, b, n);

  return mputflt (mat, (float *)mgetconvbuf, v, l, c, num);

}


/*------------------------------------------------------------------------*/


#define setf(fp,fun) if (mat->fp == NULL) mat->fp = fun

void installconverters(MFILE *mat) {

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
