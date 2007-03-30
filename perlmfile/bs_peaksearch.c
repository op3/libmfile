#include <stdio.h>
#include <stdlib.h>
#include <float.h>

int count_max(const unsigned long spec[],const int res,double *background,int *high_limit,const int low_limit)
{
  int i;
  int cnt;
  double diff,sum;
  double peak_trigger;

  /* Mit peak_trigger kann man die Empfindlichkeit auf peaks einstellen. Je kleiner, desto empfindlicher. */

  peak_trigger=0.5;
  cnt=0;
  sum=0.0;
  for(i=low_limit;i<res-2;i++){
    diff=(double)spec[i+1]-(double)spec[i];
    if(diff>0.0){
      sum+=diff;
      cnt++;
    }
  }
  sum/=(double)cnt;
  
  cnt=0;
  for(i=low_limit;i<res-2;i++){
    diff=(double)spec[i+1]-(double)spec[i];
    if(diff>peak_trigger*sum){
      cnt++;
      while(((double)spec[i+1]-(double)spec[i])>0.0 && i<res-2){
	i++;
	*high_limit=i;
      }
    }
  }
  *background=peak_trigger*sum;
  return(cnt);
}


int give_max2(const unsigned long spec[],int peak_pos[],const int res, double background,const int low_limit)
{
  int i,cnt;
  double diff;

  cnt=0;
  for(i=low_limit;i<res-2;i++){
    diff=(double)spec[i+1]-(double)spec[i];
    if(diff>background){
      while(((double)spec[i+1]-(double)spec[i])>0.0 && i<res-2){
	i++;
      }
      printf("CNT: %d\n", cnt);
      peak_pos[cnt]=i;
      cnt++;
    }
  }
  return(cnt);
}

int bs_peaksearch(const unsigned long spec[],const int res,const int low_limit,int** peak_pos,int *cnt)
{
  double background;
  int high_limit;

  *cnt=count_max(spec,res,&background,&high_limit,low_limit);

  if(NULL==((*peak_pos) = (int*)malloc(sizeof(int)*(*cnt)))){
    fprintf(stderr,"Malloc-error\n");
  }
  
  printf("End malloc\n");

  if((*cnt)!=give_max2(spec,*peak_pos,res,background,low_limit)){
     exit(-1);
  }
  
  return(1);
}




