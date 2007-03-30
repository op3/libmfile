%module mfile
%{
#include <local/mfile.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bs_visus_lib.h"
%}

%include pointer.i   //include the SWIGG pointer library

int load_spec (char*, unsigned long*, int);
int save_spec (char*, unsigned long*, int);

int bs_shift(unsigned long *ref_spec,unsigned long *spec,int res,int NBSEG,int flag_C,int LOW_LIMIT_C,int flag_H,int HIGH_LIMIT,int LOW_LIMIT,int CMP_FAC,float *shift_koef_a,float *shift_koef_b,float *mean_least_square);

int bs_shift_default(unsigned long *ref_spec,unsigned long *spec,int res,float *shift_koef_a,float *shift_koef_b,float *mean_least_square);

int bs_shift_F(float *man_pos_ref_float,int M,unsigned long *spec,int res,int flag_C,int LOW_LIMIT_C,int CMP_FAC,float *shift_koef_a,float *shift_koef_b,float *mean_least_square);


int bs_shift_R(int *max_pos_ref,int M,unsigned long *ref_spec,unsigned long *spec,int res,int flag_C,int LOW_LIMIT_C,int flag_H,int HIGH_LIMIT,int LOW_LIMIT,int CMP_FAC,float *shift_koef_a,float *shift_koef_b,float *mean_least_square);

int bs_shift_M(int *man_pos,int M,unsigned long *ref_spec,unsigned long *spec,int res,int NBSEG,int flag_C,int LOW_LIMIT_C,int flag_H,int HIGH_LIMIT,int LOW_LIMIT,int CMP_FAC,float *shift_koef_a,float *shift_koef_b,float *mean_least_square);


%inline %{
int 
get_size (char* mtxfile) {
  MFILE* mtx;
  minfo info;

  mtx = mopen(mtxfile, "r");
  if (!mtx) {
    printf("Cannot open %s\n", mtxfile);
    exit(1);
  }

  mgetinfo(mtx, &info);

  mclose(mtx);

  return (info.columns);
}  

int add_spec (unsigned long *spec, unsigned long *add, int size) {
   int i;
   for (i = 0; i < size; i++) {
      spec[i] += add[i];
   }	   
}

int
recal_poly (long* spectrum, int size, double* pol, int polsize) 
{
  int i;
  long* helpspec;
  double cal;

  helpspec = (long*) calloc(size, sizeof(int));
  
  if (polsize < 2) {
    printf("Not a suitable Polynomsize, need at least two parameters\n");
    return(0);
  }
  
  cal = pol[0];
  for (i = 0; i < size; i++) {
    double nextcal;
    double width;
    double k;
    int calrnd;
    int nextcalrnd;
    int grad;

    k = i + .5;   /* using tv channels from c-.5 to c+.5 */

    nextcal = pol[polsize-1]; 
    for (grad = (polsize - 2); grad > -1; grad--) {
      double mycal = k * nextcal + pol[grad];
      nextcal = mycal;
    }

    calrnd = (int)(cal + .5);
    nextcalrnd = (int)(nextcal + .5);

    width = nextcal - cal;

    if ( (cal >= size) || (cal < 0) || (nextcal >= size) || (nextcal < 0)) {
      cal = nextcal;
      continue;
    }

    if (nextcal >= (calrnd + .5)) {
      /* Ueberschreitung einer Grenze */
      int j;
      double height1;
      
      height1 = spectrum[i] / width;
      /* proz. Anteil des Vorgrenzteils */
      helpspec[calrnd] += (long) ((calrnd + .5 - cal) * height1 + .5);
      for (j = calrnd + 1; j < nextcalrnd; j++) {
	/* Ueberschreitung mehrerer Grenzen */
	helpspec[j] += (long) (height1 + .5);
      }
      /* proz. Anteil des Nachgrenzteils zum naechsten Kanal */
      helpspec[nextcalrnd] +=
	(long) ((nextcal - (nextcalrnd - .5)) * height1 + .5);
    } else {
      helpspec[calrnd] += spectrum[i];
    }
    cal = nextcal;
  }

  for (i = 0; i < size; i++) {
    spectrum[i] = helpspec[i];
  }
  free(helpspec);
  return(1);
}

int
recal_pair (long* spectrum, int size, double a, double b, double c,
	    double d)
{
  double pol[2];
  int polsize = 2;

  if (c == a) {
    fprintf (stderr, "Pair-Recalibration-Error! Equal channels!\n");
    return 0;
  }

  pol[0] = ( c * b - d * a ) / ( c - a ) ;
  pol[1] = ( d - b ) / ( c - a ) ;

  if (!recal_poly (spectrum, size, pol, polsize)) {
    return(0);
  }
  return(1);
}
%}
