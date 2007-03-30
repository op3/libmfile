#include <stdlib.h>
#include <stdio.h>
#include <local/mfile.h> 
#include <math.h>

extern int bs_peaksearch(const unsigned long*, 
			 const int, 
			 const int, 
			 int**, 
			 int*);



int
main(int argc, char** argv)
{
  long in_spec[8192];
  long spektrum[8192];
  int* peaklist;
  int peak_nr;
  int i;

  if (!load_spec( "cutdir/686_0/686_0.cut", in_spec, 8192))      /* Spektrum1 laden */
    exit(1);
  for (i = 0; i < 8192; i++) {
    if (in_spec[i] < 0) {
      spektrum[i] = 0;
      } else {
      spektrum[i] = in_spec[i];
    }
  }
  printf( "Spektrum geladen\n");

  if (bs_peaksearch(spektrum, 8192, 1200, &peaklist, &peak_nr)) {
    printf ("%d Peaks gefittet\n", peak_nr);
    for (i = 1; i < peak_nr; i++) {
      printf("%d \n", (peaklist)[i]);
    }
  }
  return 1;
}



