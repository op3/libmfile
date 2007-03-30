#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <local/mfile.h>


#define GIVE_MAX2
#define REL_ABW_POS_GEW

float X=2.0;
int NBSEG=7;
int LOW_LIMIT=400;
int LOW_LIMIT_C=400;
int HIGH_LIMIT;
int CMP_FAC = 2;

#define cmp_no(n) ((n) * CMP_FAC)


int get_spectrum_resolution(char *name)
{
  MFILE *tst;
  minfo info;
  if(NULL==(tst=mopen(name,"r"))){
    fprintf(stderr,"Could not open spectrum '%s'\n",name);
    return(-1);
  }
  mgetinfo(tst,&info);
  mclose(tst);
  return(info.columns);
}


int bs_max(const int a,const int b)
{ 
  if(a>b){
    return a;
  } else {
    return b;
  }
}

#ifdef GIVE_MAX2

int count_max(const unsigned long spec[],const int res,double *background,int *high_limit)
{
  int i;
  int cnt;
  double diff,sum;

  cnt=0;
  sum=0.0;
  for(i=1;i<res-2;i++){
    diff=(double)spec[i+1]-(double)spec[i];
    if(diff>0.0){
      sum+=diff;
      cnt++;
    }
  }
  sum/=(double)cnt;
  
  cnt=0;
  for(i=LOW_LIMIT;i<res-2;i++){
    diff=(double)spec[i+1]-(double)spec[i];
    if(diff>2.5*sum){
      cnt++;
      while(((double)spec[i+1]-(double)spec[i])>0.0 && i<res-2){
	i++;
	*high_limit=i;
      }
    }
  }
  *background=2.5*sum;
  return(cnt);
}


int give_max2(const unsigned long spec[],int peak_pos[],const int res, double background)
{
  int i,cnt;
  double diff;

  cnt=0;
  for(i=LOW_LIMIT;i<res-2;i++){
    diff=(double)spec[i+1]-(double)spec[i];
    if(diff>background){
      while(((double)spec[i+1]-(double)spec[i])>0.0 && i<res-2){
	i++;
      }
      peak_pos[cnt]=i;
      cnt++;
    }
  }
  return(cnt);
}

#endif

int creat_jumped_spec(unsigned long spec[],const int res)
{
  int x,jump=20;
  float time=0.7;
  float *spec_tmp;

  if(NULL==(spec_tmp=malloc(sizeof(float)*res))){
    fprintf(stderr,"Malloc-error in creat_jumped_spec\n");
  }

  for(x=0;x<jump;x++){
    spec_tmp[x]=0.0;
  }

  for(x=0;x<res-jump;x++){
    spec_tmp[x+jump]=(float)spec[x]*time;
  }

  for(x=0;x<res;x++){
    spec[x]+=(unsigned long)spec_tmp[x];
  }
  return(1);
}


#ifdef REL_ABW_POS5

float rel_abw_pos5(const int max_pos_ref[],const int max_pos_spec[],float a_b_gut[],int k_l_gut[],const int cnt_spec_comb,const int size_k)
{
  int i,k,g;
  float stda;
  float ret=1000.0;
  float d_tmp;
  float a,b;
  
  for(k=0;k<cnt_spec_comb/size_k;k++){
    g=k*size_k;
    b=(float)(max_pos_ref[size_k-1]-max_pos_ref[0])/(float)(max_pos_spec[g+size_k-1]-max_pos_spec[g]);
    a=(float)max_pos_ref[0]-(float)max_pos_spec[g]*b;
    stda=0.0;
    for(i=1;i<size_k-1;i++){
      d_tmp=(float)max_pos_spec[g+i]*b+(float)(a-max_pos_ref[i]);
      if(d_tmp<0.0){
	d_tmp*=-1.0;
      } 
      stda+=d_tmp/size_k;
    }
    if(stda<ret){
      ret=stda;
      a_b_gut[0]=a;
      a_b_gut[1]=b;
      k_l_gut[0]=k;
      k_l_gut[1]=k*size_k;
    }
  }
  return(ret);
}

#endif


#ifdef REL_ABW_POS_GEW

float rel_abw_pos5(const int max_pos_ref[],const int max_pos_spec[],float a_b_gut[],int k_l_gut[],const int cnt_spec_comb,const int size_k)
{
  int i,k,g;
  float stda;
  float ret=1000.0;
  float d_tmp;
  float a,b;
  
  for(k=0;k<cnt_spec_comb/size_k;k++){
    g=k*size_k;
    b=(float)(max_pos_ref[size_k-1]-max_pos_ref[0])/(float)(max_pos_spec[g+size_k-1]-max_pos_spec[g]);
    a=(float)max_pos_ref[0]-(float)max_pos_spec[g]*b;
    stda=0.0;
    for(i=1;i<size_k-1;i++){
      d_tmp=(float)max_pos_spec[g+i]*b+(float)(a-max_pos_ref[i]);
      if(d_tmp<0.0){
	d_tmp*=-1.0;
      } 
      stda+=d_tmp/size_k;
    }
    if(b>=1.0){
      stda*=b;
    }else{
      stda/=b;
    }
    if(stda<ret){
      ret=stda;
      a_b_gut[0]=a;
      a_b_gut[1]=b;
      k_l_gut[0]=k;
      k_l_gut[1]=k*size_k;
    }
  }
  return(ret);
}

#endif

float intensity_check(const unsigned long ref_spec[],const unsigned long spec[],const int max_pos_ref_kl[],const int max_pos_spec_kl[],const int size_k)
{
  int i,x_ref,x_spec;
  float ret,ret_tmp,norm;

  x_ref=max_pos_ref_kl[0];
  x_spec=max_pos_spec_kl[0];

  norm=(float)ref_spec[x_ref]/(float)spec[x_spec];

  ret=0.0;
  for(i=1;i<size_k;i++){
    x_ref=max_pos_ref_kl[i];
    x_spec=max_pos_spec_kl[i];
    ret_tmp=((float)ref_spec[x_ref]-(float)spec[x_spec]*norm)/(float)ref_spec[x_ref];
    if(ret_tmp<0.0){
      ret_tmp*=-1.0;
    }
    ret+=ret_tmp/(float)size_k;
  }
  return(ret);
}


double give_fwhm(const unsigned long spec[],const int max_pos_spec)
{

  int lch,rch;
  double a,b;
  double bg,half_max,xl,xr;

  lch=max_pos_spec;
  a=(double)spec[lch];
    
  while(spec[lch-1]<spec[lch]){
    a=(double)spec[lch-1];
    lch--;
  }
  
  rch=max_pos_spec;
  b=(double)spec[rch];

  while(spec[rch+1]<spec[rch]){
    b=(double)spec[rch+1];
    rch++;
  }

  bg=(a+b)/2.0;
  half_max=((double)spec[max_pos_spec]-bg)/2.0+bg;

  lch=max_pos_spec;
    
  while(spec[lch]>=half_max){
    lch--;
  }

  rch=max_pos_spec;

  while(spec[rch]>=half_max){
    rch++;
  }
  
  b=(double)spec[lch+1]-(double)spec[lch];
  a=(double)spec[lch]-b*lch;
 
  xl=(half_max-a)/b;

  b=(double)spec[rch]-(double)spec[rch-1];
  a=(double)spec[rch]-b*(rch);
  
  xr=(half_max-a)/b;

  return(xr-xl);
}

double give_background(const unsigned long spec[],const int max_pos_spec,const double background)
{

  int lch,rch;
  double a,b;
  double bg,diff,min;

  lch=max_pos_spec;
  a=(double)spec[lch];
    
  while(spec[lch-1]<spec[lch] && lch>LOW_LIMIT+2){
    a=(double)spec[lch-1];
    lch--;
  }
  
  rch=max_pos_spec;
  b=(double)spec[rch];

  while(spec[rch+1]<spec[rch] && rch<HIGH_LIMIT-2){
    b=(double)spec[rch+1];
    rch++;
  }

  if(a>b){
    min=b;
    diff=a-b;
  } else {
    min=a;
    diff=b-a;
  }

  if(diff>background){
    bg=min;
  } else {
    bg=(a+b)/2.0;
  }
  return(bg+(double)max_pos_spec*0.0000001);
}


int sort_biggest(const unsigned long spec[],const int peak_pos[],int cnt,int max_pos[],const int number,const double bg)
{
  int i,j;
  int x,xb,xc,xtmp,min;

  if(number>cnt){
    for(i=0;i<cnt;i++){
      if(peak_pos[i]>LOW_LIMIT){
	max_pos[i]=peak_pos[i];
      }
    }
    for(i=cnt;i<number;i++){
      max_pos[i]=0;
    }
    return(1);
  }

  max_pos[0]=peak_pos[0];
  for(i=0;i<cnt;i++){
    xtmp=peak_pos[i];
    x=max_pos[0];
    if((double)spec[xtmp]-give_background(spec,xtmp,bg)>(double)spec[x]-give_background(spec,x,bg)){
      max_pos[0]=xtmp;
    }
  }
  
  min=peak_pos[0];
  for(i=0;i<cnt;i++){
    xtmp=peak_pos[i];
    if((double)spec[xtmp]-give_background(spec,xtmp,bg)<(double)spec[min]-give_background(spec,min,bg)){
      min=xtmp;
    }
  }

  xc=max_pos[0];
  for(i=1;i<number;i++){
    xb=max_pos[i-1];
    if(i>1){
      xc=max_pos[i-2];
    }
    max_pos[i]=min;
    for(j=0;j<cnt;j++){
      xtmp=peak_pos[j];
      x=max_pos[i];
      if((double)spec[xtmp]-give_background(spec,xtmp,bg)>=spec[x]-give_background(spec,x,bg) && spec[xtmp]-give_background(spec,xtmp,bg)<=spec[xb]-give_background(spec,xb,bg) && spec[xtmp]-give_background(spec,xtmp,bg)<spec[xc]-give_background(spec,xc,bg) && xtmp!=xb){
	max_pos[i]=xtmp;
      }
    }
  }
  return(1);
}

int sort_array(int arr[],const int size)
{
  int *tmp;
  int i,j;

  if(NULL==(tmp=malloc(sizeof(int)*size))){
    fprintf(stderr,"Malloc-error\n");
  }
  
  tmp[0]=arr[0];
  for(i=0;i<size;i++){
    if(arr[i]<tmp[0]){
	tmp[0]=arr[i];
      }
  }

  for(j=1;j<size;j++){
    tmp[j]=200000;
    for(i=0;i<size;i++){
      if(arr[i]<tmp[j] && arr[i]>tmp[j-1]){
	tmp[j]=arr[i];
      }
    }
  }
  for(i=0;i<size;i++){
    arr[i]=tmp[i];
  }
  return(1);
}
  
#ifdef GIVE_COMB

int give_comb(const int max_pos_spec[],int max_pos_spec_comb[])
{

  int i,j,k,l,m;
  int cnt;

  cnt=0;
  for(i=0;i<2*NBSEG-4;i++){
    for(j=i+1;j<2*NBSEG-3;j++){
      for(k=j+1;k<2*NBSEG-2;k++){
	for(l=k+1;l<2*NBSEG-1;l++){
	  for(m=l+1;m<2*NBSEG;m++){
	    max_pos_spec_comb[cnt]=max_pos_spec[i];
	    max_pos_spec_comb[cnt+1]=max_pos_spec[j];
	    max_pos_spec_comb[cnt+2]=max_pos_spec[k];
	    max_pos_spec_comb[cnt+3]=max_pos_spec[l];
	    max_pos_spec_comb[cnt+4]=max_pos_spec[m];
	    cnt+=5;
	  }
	}
      }
    }
  }
  return(cnt);
}

#endif

int give_comb_k(const int max_pos_spec[],int max_pos_spec_comb[],const int size_k,const int size_n)
{
  int i,j,k,l,m,n,o;
  int cnt;

  
  if(size_k>size_n){
    fprintf(stderr,"Wrong size_k!\n");
    exit(0);
  }

  if(size_k==4){
    cnt=0;
    for(i=0;i<size_n-3;i++){
      for(j=i+1;j<size_n-2;j++){
	for(k=j+1;k<size_n-1;k++){
	  for(l=k+1;l<size_n;l++){
	    max_pos_spec_comb[cnt]=max_pos_spec[i];
	    max_pos_spec_comb[cnt+1]=max_pos_spec[j];
	    max_pos_spec_comb[cnt+2]=max_pos_spec[k];
	    max_pos_spec_comb[cnt+3]=max_pos_spec[l];
	    cnt+=4;
	  }
	}
      }
    }
    return(cnt);
  }


  if(size_k==5){
    cnt=0;
    for(i=0;i<size_n-4;i++){
      for(j=i+1;j<size_n-3;j++){
	for(k=j+1;k<size_n-2;k++){
	  for(l=k+1;l<size_n-1;l++){
	    for(m=l+1;m<size_n;m++){
	      max_pos_spec_comb[cnt]=max_pos_spec[i];
	      max_pos_spec_comb[cnt+1]=max_pos_spec[j];
	      max_pos_spec_comb[cnt+2]=max_pos_spec[k];
	      max_pos_spec_comb[cnt+3]=max_pos_spec[l];
	      max_pos_spec_comb[cnt+4]=max_pos_spec[m];
	      cnt+=5;
	    }
	  }
	}
      }
    }
  return(cnt);
  }

  if(size_k==6){
    cnt=0;
    for(i=0;i<size_n-5;i++){
      for(j=i+1;j<size_n-4;j++){
	for(k=j+1;k<size_n-3;k++){
	  for(l=k+1;l<size_n-2;l++){
	    for(m=l+1;m<size_n-1;m++){
	      for(n=m+1;n<size_n;n++){
		max_pos_spec_comb[cnt]=max_pos_spec[i];
		max_pos_spec_comb[cnt+1]=max_pos_spec[j];
		max_pos_spec_comb[cnt+2]=max_pos_spec[k];
		max_pos_spec_comb[cnt+3]=max_pos_spec[l];
		max_pos_spec_comb[cnt+4]=max_pos_spec[m];
		max_pos_spec_comb[cnt+5]=max_pos_spec[n];
		cnt+=6;
	      }
	    }
	  }
	}
      }
    }
  return(cnt);
  }

  if(size_k==7){
    cnt=0;
    for(i=0;i<size_n-6;i++){
      for(j=i+1;j<size_n-5;j++){
	for(k=j+1;k<size_n-4;k++){
	  for(l=k+1;l<size_n-3;l++){
	    for(m=l+1;m<size_n-2;m++){
	      for(n=m+1;n<size_n-1;n++){
		for(o=n+1;o<size_n;o++){
		  max_pos_spec_comb[cnt]=max_pos_spec[i];
		  max_pos_spec_comb[cnt+1]=max_pos_spec[j];
		  max_pos_spec_comb[cnt+2]=max_pos_spec[k];
		  max_pos_spec_comb[cnt+3]=max_pos_spec[l];
		  max_pos_spec_comb[cnt+4]=max_pos_spec[m];
		  max_pos_spec_comb[cnt+5]=max_pos_spec[n];
		  max_pos_spec_comb[cnt+6]=max_pos_spec[o];
		  cnt+=7;
		}
	      }
	    }
	  }
	}
      }
    }
    return(cnt);
  } 
}


int give_n_over_k(const int n,const int k)
{
  int i;
  double z,nn;
  double n_k;
  

  z=1.0;
  for(i=k+1;i<=n;i++){
    z*=i;
  }

  nn=1.0;
  for(i=1;i<=n-k;i++){
    nn*=i;
  }
  n_k=z/nn;
  return((int)n_k);
}
  
float give_center(const unsigned long spec[],const int max_pos_spec,const double background)
{
  int i,lch,rch;
  float y,x_y;
  float a,b;
  float bg,diff,min;

  lch=max_pos_spec;
  a=(float)spec[lch];
    
  while(spec[lch-1]<spec[lch]){
    a=(float)spec[lch-1];
    lch--;
  }
  
  rch=max_pos_spec;
  b=(float)spec[rch];

  while(spec[rch+1]<spec[rch]){
    b=(float)spec[rch+1];
    rch++;
  }

  if(a>b){
    min=b;
    diff=a-b;
  } else {
    min=a;
    diff=b-a;
  }

  if(diff>background){
    bg=min;
  } else {
    bg=(a+b)/2.0;
  }

  x_y=0.0;
  y=0.0;
  for(i=lch;i<=rch;i++){
    x_y+=(float)(i*(spec[i]-bg));
    y+=(float)spec[i]-bg;
  }
  return(x_y/y);
}

int give_a_b_prec(const float pos0_ref,const float pos1_ref,const float pos0,const float pos1,float a_b_gut[])
{
  a_b_gut[1]=(pos1_ref-pos0_ref)/(pos1-pos0);
  a_b_gut[0]=pos0_ref-pos0*a_b_gut[1];
  return(1);
}

int jump_test(int max_pos_spec_comb_kl[],int peak_pos_spec[],const int cnt_spec,const int size_k)
{
  int i,j;
  int ret=0;

  for(j=0;j<size_k;j++){
    for(i=0;i<cnt_spec;i++){
      if(max_pos_spec_comb_kl[j]==peak_pos_spec[i]){
	peak_pos_spec[i]=0;
	ret++;
	break;
      }
    }
  }
  return(ret);
}

int manual_cal(int man_pos_spec[],const int next,const int man_pos[],const int peak_pos_spec[],const float a_b_gut[],const int M,const int cnt_spec)
{
  int i,j,k;
  float *diff;
  float sum,sum_tmp;

  if(NULL==(diff=malloc(sizeof(float)*next))){
    fprintf(stderr,"Malloc-error\n");
  }

  for(j=0;j<M;j++){
    sum=1000.0;
    for(i=0;i<cnt_spec-next;i++){
      sum_tmp=0.0;
      for(k=0;k<next;k++){
	diff[k]=(float)man_pos[j]-a_b_gut[0]-(float)peak_pos_spec[i+k]*a_b_gut[1];
	if(diff[k]<0.0){
	  diff[k]*=-1.0;
	}
	sum_tmp+=diff[k];
      }
      if(sum_tmp<sum){
	sum=sum_tmp;
	for(k=0;k<next;k++){
	  man_pos_spec[j*next+k]=peak_pos_spec[k+i];
	}
      }
    }
  }
  return(1);
}

int give_comb_man(int man_pos_spec_comb[],const int man_pos_spec[],const int next,const int M)
{
  int i,j,k,l,m,n;
  int cnt;

  

  cnt=0;

  if(M==4){
    for(i=0;i<next;i++){
      for(j=next;j<2*next;j++){
	for(k=2*next;k<3*next;k++){
	  for(l=3*next;l<4*next;l++){
	    man_pos_spec_comb[cnt]=man_pos_spec[i];
	    man_pos_spec_comb[cnt+1]=man_pos_spec[j];
	    man_pos_spec_comb[cnt+2]=man_pos_spec[k];
	    man_pos_spec_comb[cnt+3]=man_pos_spec[l];
	    cnt+=4;
	  }
	}
      }
    }
  }

  if(M==5){
    for(i=0;i<next;i++){
      for(j=next;j<2*next;j++){
	for(k=2*next;k<3*next;k++){
	  for(l=3*next;l<4*next;l++){
	    for(m=4*next;m<5*next;m++){
	      man_pos_spec_comb[cnt]=man_pos_spec[i];
	      man_pos_spec_comb[cnt+1]=man_pos_spec[j];
	      man_pos_spec_comb[cnt+2]=man_pos_spec[k];
	      man_pos_spec_comb[cnt+3]=man_pos_spec[l];
	      man_pos_spec_comb[cnt+4]=man_pos_spec[m];
	      cnt+=5;
	    }
	  }
	}
      }
    }
  }

  if(M==6){
    for(i=0;i<next;i++){
      for(j=next;j<2*next;j++){
	for(k=2*next;k<3*next;k++){
	  for(l=3*next;l<4*next;l++){
	    for(m=4*next;m<5*next;m++){
	      for(n=5*next;n<6*next;n++){
		man_pos_spec_comb[cnt]=man_pos_spec[i];
		man_pos_spec_comb[cnt+1]=man_pos_spec[j];
		man_pos_spec_comb[cnt+2]=man_pos_spec[k];
		man_pos_spec_comb[cnt+3]=man_pos_spec[l];
		man_pos_spec_comb[cnt+4]=man_pos_spec[m];
		man_pos_spec_comb[cnt+5]=man_pos_spec[n];
		cnt+=6;
	      }
	    }
	  }
	}
      }
    }
  }
  return(cnt);
}

int a_hoch_b(const int a,const int b)
{
  int i,z;

  if(b==0){
    return(a);
  }else{
    z=a;
    for(i=1;i<b;i++){
      z*=a;
    }
    return(z);
  }
}

float rel_jump_test(const int max_pos_ref[],const int peak_pos_spec[],const int size_k,const int size_n,int max_pos_spec_kl[],float a_b_gut[])
{
  int i,j,k,l,m,n,o,p;
  int *max_pos_spec_tmp;
  float a_b_gut_fkt[2];
  float rel;
  int kl_dumm[2];
  
  if(NULL==(max_pos_spec_tmp=malloc(sizeof(int)*size_k))){
    fprintf(stderr,"Malloc-error in rel_jump_test");
  }
 
  if(size_k==5){
    for(i=0;i<size_n-4;i++){
      for(j=i+1;j<size_n-3;j++){
	for(k=j+1;k<size_n-2;k++){
	  for(l=k+1;l<size_n-1;l++){
	    for(m=l+1;m<size_n;m++){
	      max_pos_spec_tmp[0]=peak_pos_spec[i];
	      max_pos_spec_tmp[1]=peak_pos_spec[j];
	      max_pos_spec_tmp[2]=peak_pos_spec[k];
	      max_pos_spec_tmp[3]=peak_pos_spec[l];
	      max_pos_spec_tmp[4]=peak_pos_spec[m];

	      rel=rel_abw_pos5(max_pos_ref,max_pos_spec_tmp,a_b_gut_fkt,kl_dumm,size_k,size_k);

	      if(rel<0.3){
		a_b_gut[0]=a_b_gut_fkt[0];
		a_b_gut[1]=a_b_gut_fkt[1];
		for(p=0;p<size_k;p++){
		  max_pos_spec_kl[p]=max_pos_spec_tmp[p];
		}
		return(rel);
	      }
	    }
	  }
	}
      }
    }
  }

  if(size_k==6){
    for(i=0;i<size_n-5;i++){
      for(j=i+1;j<size_n-4;j++){
	for(k=j+1;k<size_n-3;k++){
	  for(l=k+1;l<size_n-2;l++){
	    for(m=l+1;m<size_n-1;m++){
	      for(n=m+1;n<size_n;n++){
		max_pos_spec_tmp[0]=peak_pos_spec[i];
		max_pos_spec_tmp[1]=peak_pos_spec[j];
		max_pos_spec_tmp[2]=peak_pos_spec[k];
		max_pos_spec_tmp[3]=peak_pos_spec[l];
		max_pos_spec_tmp[4]=peak_pos_spec[m];
		max_pos_spec_tmp[5]=peak_pos_spec[n];

		rel=rel_abw_pos5(max_pos_ref,max_pos_spec_tmp,a_b_gut_fkt,kl_dumm,size_k,size_k);

		if(rel<0.5){
		  a_b_gut[0]=a_b_gut_fkt[0];
		  a_b_gut[1]=a_b_gut_fkt[1];
		  for(p=0;p<size_k;p++){
		    max_pos_spec_kl[p]=max_pos_spec_tmp[p];
		  }
		  return(rel);
		}
	      }
	    }
	  }
	}
      }
    }
  }

  if(size_k==7){
    for(i=0;i<size_n-6;i++){
      for(j=i+1;j<size_n-5;j++){
	for(k=j+1;k<size_n-4;k++){
	  for(l=k+1;l<size_n-3;l++){
	    for(m=l+1;m<size_n-2;m++){
	      for(n=m+1;n<size_n-1;n++){
		for(o=n+1;o<size_n;o++){
		  max_pos_spec_tmp[0]=peak_pos_spec[i];
		  max_pos_spec_tmp[1]=peak_pos_spec[j];
		  max_pos_spec_tmp[2]=peak_pos_spec[k];
		  max_pos_spec_tmp[3]=peak_pos_spec[l];
		  max_pos_spec_tmp[4]=peak_pos_spec[m];
		  max_pos_spec_tmp[5]=peak_pos_spec[n];
		  max_pos_spec_tmp[6]=peak_pos_spec[o];

		  rel=rel_abw_pos5(max_pos_ref,max_pos_spec_tmp,a_b_gut_fkt,kl_dumm,size_k,size_k);

		  if(rel<1.0){
		    a_b_gut[0]=a_b_gut_fkt[0];
		    a_b_gut[1]=a_b_gut_fkt[1];
		    for(p=0;p<size_k;p++){
		      max_pos_spec_kl[p]=max_pos_spec_tmp[p];
		    }
		    return(rel);
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  return(10.0);
}


void usage(void)
{
  fprintf(stderr,
	  "Usage: bs_vis3 <ref_spec> <test_spec> [adjust] [plot-options]\n"
	  "[adjust]:    Syntax            Description\n"
	  "          -N[number]   number of peaks\n" 
	  "          -L[number]   low energie cut in <ref_spec>\n"
	  "          -C[number]   low energie cut in <test_spec>\n"
	  "          -R[number]   give peaks + <ref_spec>\n"
	  "          -H[number]   high energie cut in <ref_spec> \n"
	  "          -M[number]   add coulex peaks for shift-corrections\n"
	  "          -F[number]   instead of <ref_spec> give file with peaks\n"
	  "          -T[number]   -N*number of peaks of <test_spec>, default=3\n"
	  "[plot-options]: -plt_ref -plt -plt_vec\n");
}


int bs_shift(unsigned long *ref_spec,unsigned long *spec,int res,int NBSEG,int flag_C,int LOW_LIMIT_C,int flag_H,int HIGH_LIMIT,int LOW_LIMIT,int CMP_FAC,float *shift_koef_a,float *shift_koef_b,float *mean_least_square)
{
  int *peak_pos_ref;
  int *peak_pos_spec;
  int *max_pos_ref,*max_pos_ref_comb;
  int *max_pos_spec,*max_pos_spec_comb;
  int *man_pos_spec,*man_pos_spec2,*man_pos_spec_comb;
  float *man_pos_ref_float;
  int i,j,x,cnt;
  int cnt_ref,cnt_spec,cnt_spec_comb,cnt_ref_comb;
  int plt=0;
  double clear;
  double background_ref,background_spec;
  int *right_peaks;
  char *arg;
  int high_limit;
  float a_b_gut[2],a_b_gut_tmp[2];
  float pos_ref[2],pos[2];
  int k_l_gut[2],kl,kl_ref;
  int flag_M=0,flag_F=0;
  float rel,rel_tmp,dx;
  int size_k;
  float jp_tst;
  int man_pos[6];
  int M,next=3;
  int size_man;
  float rel_man,a_b_gut_man[2];
  int k_l_gut_man[2];
  int kl_man;
  float int_diff;
  int cnt_spec_jp;
  double *vc;
  double dE,dE_ref;
  int *dop_ref,*dop_spec;
  int shifted_ref,shifted_spec;
  FILE *ref_F;

  if(flag_H==0){
    HIGH_LIMIT=res;
  }

  if(flag_M!=2){
    cnt_ref=count_max(ref_spec,res,&background_ref,&high_limit);

    if(cnt_ref<NBSEG){
      printf("ERROR: only %d peak(s) found in ref\n",cnt_ref);
      return(-10);
    }

    if(NULL==(peak_pos_ref=malloc(sizeof(int)*cnt_ref))){
      fprintf(stderr,"Malloc-error\n");
    }
  
    if(flag_H){
      for(i=0;i<cnt_ref;i++){
	peak_pos_ref[i]=0;
    }
      ref_spec[0]=0;
      high_limit=HIGH_LIMIT;
    }
  
    cnt_ref=give_max2(ref_spec,peak_pos_ref,high_limit,background_ref);
  
    if(cnt_ref<NBSEG){
      printf("ERROR: only %d peaks found in ref\n",cnt_ref);
      return(-10);
    }
  }
  
  if(flag_C){
    LOW_LIMIT=LOW_LIMIT_C;
  }

  cnt_spec=count_max(spec,res,&background_spec,&high_limit);
  
  if(NULL==(peak_pos_spec=malloc(sizeof(int)*cnt_spec))){
    fprintf(stderr,"Malloc-error\n");
  }

  for(i=0;i<cnt_spec;i++){
    peak_pos_spec[i]=0;
  }
  spec[0]=0;
  
  cnt_spec=give_max2(spec,peak_pos_spec,high_limit,background_spec);

  /* zum Anschauen der Peak-Liste 

  for(i=0;i<cnt_spec;i++){
    x=peak_pos_spec[i];
    printf("%d %ld\n",x,spec[x]);
  }

  */


  if(cnt_spec<NBSEG){
    cnt_spec=NBSEG;
  }
 

  if(cnt_spec>30){
    next=7;
  }else if(cnt_spec>20 && cnt_spec<30){
    next=6;
  }else if(cnt_spec>10 && cnt_spec<20){
    next=5;
  }else{
    next=3;
  }

  
  if(flag_M!=2){
    size_k=NBSEG;

    if(NULL==(max_pos_ref=malloc(sizeof(int)*size_k))){
      fprintf(stderr,"Malloc-error\n");
    }
     
    if(1!=sort_biggest(ref_spec,peak_pos_ref,cnt_ref,max_pos_ref,size_k,background_ref)){
      fprintf(stderr,"sort_biggest-error\n");
    }

    if(1!=sort_array(max_pos_ref,size_k)){
      fprintf(stderr,"sort_array-error\n");
    }
  }
 

  if(plt==1){
    if(flag_F==0){
      for(i=0;i<NBSEG;i++){
	x=max_pos_ref[i];
	clear=(double)ref_spec[x]-give_background(ref_spec,x,background_ref);
	printf("%d %.1lf\n",x,clear);
      }
      return(-10);
    }
    if(flag_F==1){
      for(i=0;i<NBSEG;i++){
	x=max_pos_ref[i];
	printf("%d %d\n",x,2000-x);
      }
    }
  }
     
  if(NULL==(max_pos_spec=malloc(sizeof(int)*cmp_no(size_k)))){
    fprintf(stderr,"Malloc-error\n");
  }
  
  if(1!=sort_biggest(spec,peak_pos_spec,cnt_spec,max_pos_spec,
		     cmp_no(size_k),background_spec)){
    fprintf(stderr,"sort_biggest-error\n");
  }

  if(1!=sort_array(max_pos_spec,cmp_no(size_k))){
    fprintf(stderr,"sort_array-error\n");
  }

  if(plt==2){
    for(i=0;i<cmp_no(NBSEG);i++){
      x=max_pos_spec[i];
      clear=(double)spec[x]-give_background(spec,x,background_spec);
      printf("%d %.1lf\n",x,clear);
    }
    return(-10);
  }
  
  cnt_spec_comb=size_k*give_n_over_k(cmp_no(size_k),size_k);
  
  if(NULL==(max_pos_spec_comb=malloc(sizeof(int)*cnt_spec_comb))){
    fprintf(stderr,"Malloc-error\n");
  }
  
  if(cnt_spec_comb!=(cnt=give_comb_k(max_pos_spec,max_pos_spec_comb,
				     size_k,cmp_no(size_k)))){
    fprintf(stderr,"give_comb_k-error\n");
  }
   
  rel=rel_abw_pos5(max_pos_ref,max_pos_spec_comb,a_b_gut,k_l_gut,
		   cnt_spec_comb,size_k);

  if(rel<X){
    kl=k_l_gut[1];
    if(flag_F==0){
      pos_ref[0]=give_center(ref_spec,max_pos_ref[0],background_ref);
      pos_ref[1]=give_center(ref_spec,max_pos_ref[size_k-1],background_ref);
      pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
      pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],background_spec);
      give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
    }
    if(flag_F==1){
      pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
      pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],background_spec);
      pos_ref[0]=man_pos_ref_float[0];
      pos_ref[1]=man_pos_ref_float[size_k-1];
      give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
    }
  }
 
  if(rel>=X){
    for(j=1;size_k>4;j++){
      size_k=NBSEG-j;
      cnt_ref_comb=size_k*give_n_over_k(NBSEG,size_k);

      if(NULL==(max_pos_ref_comb=malloc(sizeof(int)*cnt_ref_comb))){
	fprintf(stderr,"Malloc-error\n");
      }

      cnt_spec_comb=size_k*give_n_over_k(cmp_no(size_k),size_k);

      if(NULL==(max_pos_spec_comb=malloc(sizeof(int)*cnt_spec_comb))){
	fprintf(stderr,"Malloc-error\n");
      }
      
      if(cnt_ref_comb!=(cnt=give_comb_k(max_pos_ref,max_pos_ref_comb,
					size_k,NBSEG))){
	fprintf(stderr,"give_comb_k-error\n");
      }

      if(NULL==(max_pos_spec=malloc(sizeof(int)*cmp_no(size_k)))){
	fprintf(stderr,"Malloc-error\n");
      }
      
      if(1!=sort_biggest(spec,peak_pos_spec,cnt_spec,max_pos_spec,
			 cmp_no(size_k),background_spec)){
	fprintf(stderr,"sort_biggest-error\n");
      }
      
      if(1!=sort_array(max_pos_spec,cmp_no(size_k))){
	fprintf(stderr,"sort_array-error\n");
      }

      if(cnt_spec_comb!=(cnt=give_comb_k(max_pos_spec,max_pos_spec_comb,
					 size_k,cmp_no(size_k)))){
	fprintf(stderr,"give_comb_k-error\n");
      }
    
      for(i=0;i<cnt_ref_comb;i+=size_k){
	rel_tmp=rel_abw_pos5(&max_pos_ref_comb[i],max_pos_spec_comb,
			     a_b_gut_tmp,k_l_gut,cnt_spec_comb,size_k);
	if(rel_tmp<rel){
	  rel=rel_tmp;
	  kl_ref=i;
	  kl=k_l_gut[1];
	  a_b_gut[0]=a_b_gut_tmp[0];
	  a_b_gut[1]=a_b_gut_tmp[1];
	}
      }
      if(rel<X){
	if(flag_F==0){
	  pos_ref[0]=give_center(ref_spec,max_pos_ref_comb[kl_ref],
				 background_ref);
	  pos_ref[1]=give_center(ref_spec,max_pos_ref_comb[kl_ref+size_k-1],
				 background_ref);
	  pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
	  pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],
			     background_spec);
	  give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
	  break;
	}
	if(flag_F==1){
	  pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
	  pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],
			     background_spec);
	  for(i=kl_ref;i<kl_ref+size_k;i++){
	    if(max_pos_ref_comb[kl_ref]==(int)man_pos_ref_float[i]){
	      pos_ref[0]=man_pos_ref_float[i];
	    }
	    if(max_pos_ref_comb[kl_ref+size_k-1]==(int)man_pos_ref_float[i]){
	      pos_ref[1]=man_pos_ref_float[i];
	    }
	  }
	  give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
	  break;
	}
      }
    }
  }
  
  if(plt==0){
    *shift_koef_a=a_b_gut[0];
    *shift_koef_b=a_b_gut[1];
    *mean_least_square=rel;

    if(size_k==NBSEG){

      return(1);
    }

    if(size_k<NBSEG && size_k>4){
	
      return(-(NBSEG-size_k));

      /* Hier gibt die Funktion an, welche Peaks sie nicht gefunden hat.

      j=kl_ref;
      for(i=0;i<NBSEG;i++){
	if(max_pos_ref[i]!=max_pos_ref_comb[j]){
	  printf("%d  ",max_pos_ref[i]);
	  j--;
	}
	j++;
      }

      */
    }

    if(size_k==4){
      if(rel<X-1.0){

	return(-4);
	

      }
      if(rel>X-1.0){

	return(-10);
      }
    }
  }
}


int bs_shift_default(unsigned long *ref_spec,unsigned long *spec,int res,float *shift_koef_a,float *shift_koef_b,float *mean_least_square)
{
  int rueckgabewert;
  int flag_C=0,flag_H=0;

  rueckgabewert= bs_shift(ref_spec,spec,res,NBSEG,flag_C,LOW_LIMIT_C,flag_H,res,LOW_LIMIT,CMP_FAC,shift_koef_a,shift_koef_b,mean_least_square);

  return(rueckgabewert);
}

int bs_shift_F(float *man_pos_ref_float,int M,unsigned long *spec,int res,int flag_C,int LOW_LIMIT_C,int CMP_FAC,float *shift_koef_a,float *shift_koef_b,float *mean_least_square)
{

  unsigned long *ref_spec;
  int *peak_pos_ref;
  int *peak_pos_spec;
  int *max_pos_ref,*max_pos_ref_comb;
  int *max_pos_spec,*max_pos_spec_comb;
  int *man_pos_spec,*man_pos_spec2,*man_pos_spec_comb;
  int res2,res3;
  int i,j,x,cnt;
  int cnt_ref,cnt_spec,cnt_spec_comb,cnt_ref_comb;
  int plt=0;
  double clear;
  double background_ref,background_spec;
  int *right_peaks;
  char *arg;
  int high_limit;
  float a_b_gut[2],a_b_gut_tmp[2];
  float pos_ref[2],pos[2];
  int k_l_gut[2],kl,kl_ref;
  int flag_M=2,flag_F=1,flag_H=0;
  float rel,rel_tmp,dx;
  int size_k;
  float jp_tst;
  int man_pos[6];
  int next=3;
  int size_man;
  float rel_man,a_b_gut_man[2];
  int k_l_gut_man[2];
  int kl_man;
  float int_diff;
  int cnt_spec_jp;
  double *vc;
  double dE,dE_ref;
  int *dop_ref,*dop_spec;
  int shifted_ref,shifted_spec;
  FILE *ref_F;
 
  res2=res;
  res3=res;

  if(flag_M==2){
    NBSEG=M;
  }

  if(flag_H==0){
    HIGH_LIMIT=res;
  }

  if(flag_M==2){
    if(NULL==(max_pos_ref=malloc(sizeof(int)*M))){
      fprintf(stderr,"Malloc-error\n");
    }
  }
  
  if(flag_C){
    LOW_LIMIT=LOW_LIMIT_C;
  }

  cnt_spec=count_max(spec,res3,&background_spec,&high_limit);
  
  if(NULL==(peak_pos_spec=malloc(sizeof(int)*cnt_spec))){
    fprintf(stderr,"Malloc-error\n");
  }

  for(i=0;i<cnt_spec;i++){
    peak_pos_spec[i]=0;
  }
  spec[0]=0;
  
  cnt_spec=give_max2(spec,peak_pos_spec,high_limit,background_spec);

  if(cnt_spec<NBSEG){
    cnt_spec=NBSEG;
  }
 
  if(cnt_spec>30){
    next=7;
  }else if(cnt_spec>20 && cnt_spec<30){
    next=6;
  }else if(cnt_spec>10 && cnt_spec<20){
    next=5;
  }else{
    next=3;
  }

  if(flag_M==2){
    size_k=M;
  }
  
  if(NULL==(max_pos_spec=malloc(sizeof(int)*cmp_no(size_k)))){
    fprintf(stderr,"Malloc-error\n");
  }
  
  if(1!=sort_biggest(spec,peak_pos_spec,cnt_spec,max_pos_spec,
		     cmp_no(size_k),background_spec)){
    fprintf(stderr,"sort_biggest-error\n");
  }

  if(1!=sort_array(max_pos_spec,cmp_no(size_k))){
    fprintf(stderr,"sort_array-error\n");
  }
  
  cnt_spec_comb=size_k*give_n_over_k(cmp_no(size_k),size_k);
  
  if(NULL==(max_pos_spec_comb=malloc(sizeof(int)*cnt_spec_comb))){
    fprintf(stderr,"Malloc-error\n");
  }
  
  if(cnt_spec_comb!=(cnt=give_comb_k(max_pos_spec,max_pos_spec_comb,
				     size_k,cmp_no(size_k)))){
    fprintf(stderr,"give_comb_k-error\n");
  }
   
  rel=rel_abw_pos5(max_pos_ref,max_pos_spec_comb,a_b_gut,k_l_gut,
		   cnt_spec_comb,size_k);

  if(rel<X){
    kl=k_l_gut[1];
    if(flag_F==0){
      pos_ref[0]=give_center(ref_spec,max_pos_ref[0],background_ref);
      pos_ref[1]=give_center(ref_spec,max_pos_ref[size_k-1],background_ref);
      pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
      pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],background_spec);
      give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
    }
    if(flag_F==1){
      pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
      pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],background_spec);
      pos_ref[0]=man_pos_ref_float[0];
      pos_ref[1]=man_pos_ref_float[size_k-1];
      give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
    }
  }
 
  if(rel>=X){
    for(j=1;size_k>4;j++){
      size_k=NBSEG-j;
      cnt_ref_comb=size_k*give_n_over_k(NBSEG,size_k);

      if(NULL==(max_pos_ref_comb=malloc(sizeof(int)*cnt_ref_comb))){
	fprintf(stderr,"Malloc-error\n");
      }

      cnt_spec_comb=size_k*give_n_over_k(cmp_no(size_k),size_k);

      if(NULL==(max_pos_spec_comb=malloc(sizeof(int)*cnt_spec_comb))){
	fprintf(stderr,"Malloc-error\n");
      }
      
      if(cnt_ref_comb!=(cnt=give_comb_k(max_pos_ref,max_pos_ref_comb,
					size_k,NBSEG))){
	fprintf(stderr,"give_comb_k-error\n");
      }

      if(NULL==(max_pos_spec=malloc(sizeof(int)*cmp_no(size_k)))){
	fprintf(stderr,"Malloc-error\n");
      }
      
      if(1!=sort_biggest(spec,peak_pos_spec,cnt_spec,max_pos_spec,
			 cmp_no(size_k),background_spec)){
	fprintf(stderr,"sort_biggest-error\n");
      }
      
      if(1!=sort_array(max_pos_spec,cmp_no(size_k))){
	fprintf(stderr,"sort_array-error\n");
      }

      if(cnt_spec_comb!=(cnt=give_comb_k(max_pos_spec,max_pos_spec_comb,
					 size_k,cmp_no(size_k)))){
	fprintf(stderr,"give_comb_k-error\n");
      }
    
      for(i=0;i<cnt_ref_comb;i+=size_k){
	rel_tmp=rel_abw_pos5(&max_pos_ref_comb[i],max_pos_spec_comb,
			     a_b_gut_tmp,k_l_gut,cnt_spec_comb,size_k);
	if(rel_tmp<rel){
	  rel=rel_tmp;
	  kl_ref=i;
	  kl=k_l_gut[1];
	  a_b_gut[0]=a_b_gut_tmp[0];
	  a_b_gut[1]=a_b_gut_tmp[1];
	}
      }
      if(rel<X){
	if(flag_F==0){
	  pos_ref[0]=give_center(ref_spec,max_pos_ref_comb[kl_ref],
				 background_ref);
	  pos_ref[1]=give_center(ref_spec,max_pos_ref_comb[kl_ref+size_k-1],
				 background_ref);
	  pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
	  pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],
			     background_spec);
	  give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
	  break;
	}
	if(flag_F==1){
	  pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
	  pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],
			     background_spec);
	  for(i=kl_ref;i<kl_ref+size_k;i++){
	    if(max_pos_ref_comb[kl_ref]==(int)man_pos_ref_float[i]){
	      pos_ref[0]=man_pos_ref_float[i];
	    }
	    if(max_pos_ref_comb[kl_ref+size_k-1]==(int)man_pos_ref_float[i]){
	      pos_ref[1]=man_pos_ref_float[i];
	    }
	  }
	  give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
	  break;
	}
      }
    }
  }
  
  if(plt==0){
    *shift_koef_a=a_b_gut[0];
    *shift_koef_b=a_b_gut[1];
    *mean_least_square=rel; 

    if(size_k==NBSEG){

      return(1);
    }

    if(size_k<NBSEG && size_k>4){
	
      return(-(NBSEG-size_k));

      /* Hier gibt die Funktion an, welche Peaks sie nicht gefunden hat.

      j=kl_ref;
      for(i=0;i<NBSEG;i++){
	if(max_pos_ref[i]!=max_pos_ref_comb[j]){
	  printf("%d  ",max_pos_ref[i]);
	  j--;
	}
	j++;
      }

      */

    }

    if(size_k==4){
      if(rel<X-1.0){
	
	return(-4);

      }
      if(rel>X-1.0){
	
	return(-10);

      }
    } 
  }
}

int bs_shift_M(int *man_pos,int M,unsigned long *ref_spec,unsigned long *spec,int res,int NBSEG,int flag_C,int LOW_LIMIT_C,int flag_H,int HIGH_LIMIT,int LOW_LIMIT,int CMP_FAC,float *shift_koef_a,float *shift_koef_b,float *mean_least_square)
{
  int *peak_pos_ref;
  int *peak_pos_spec;
  int *max_pos_ref,*max_pos_ref_comb;
  int *max_pos_spec,*max_pos_spec_comb;
  int *man_pos_spec,*man_pos_spec2,*man_pos_spec_comb;
  float *man_pos_ref_float;
  int res2,res3;
  int i,j,x,cnt;
  int cnt_ref,cnt_spec,cnt_spec_comb,cnt_ref_comb;
  int plt=0;
  double clear;
  double background_ref,background_spec;
  int *right_peaks;
  char *arg;
  int high_limit;
  float a_b_gut[2],a_b_gut_tmp[2];
  float pos_ref[2],pos[2];
  int k_l_gut[2],kl,kl_ref;
  int flag_M=1,flag_F=0;
  float rel,rel_tmp,dx;
  int size_k;
  float jp_tst;
  int next=3;
  int size_man;
  float rel_man,a_b_gut_man[2];
  int k_l_gut_man[2];
  int kl_man;
  float int_diff;
  int cnt_spec_jp;
  double *vc;
  double dE,dE_ref;
  int *dop_ref,*dop_spec;
  int shifted_ref,shifted_spec;
  FILE *ref_F;

  if(flag_H==0){
    HIGH_LIMIT=res;
  }

  if(flag_M!=2){
    cnt_ref=count_max(ref_spec,res2,&background_ref,&high_limit);

    if(cnt_ref<NBSEG){
      
      return(-10);
    }

    if(NULL==(peak_pos_ref=malloc(sizeof(int)*cnt_ref))){
      fprintf(stderr,"Malloc-error\n");
    }
  
    if(flag_H){
      for(i=0;i<cnt_ref;i++){
	peak_pos_ref[i]=0;
    }
      ref_spec[0]=0;
      high_limit=HIGH_LIMIT;
    }
  
    cnt_ref=give_max2(ref_spec,peak_pos_ref,high_limit,background_ref);
  
    if(cnt_ref<NBSEG){
      printf("ERROR: only %d peaks found in ref\n",cnt_ref);
      return(-10);
    }
  }
  
  if(flag_C){
    LOW_LIMIT=LOW_LIMIT_C;
  }

  cnt_spec=count_max(spec,res3,&background_spec,&high_limit);
  
  if(NULL==(peak_pos_spec=malloc(sizeof(int)*cnt_spec))){
    fprintf(stderr,"Malloc-error\n");
  }

  for(i=0;i<cnt_spec;i++){
    peak_pos_spec[i]=0;
  }
  spec[0]=0;
  
  cnt_spec=give_max2(spec,peak_pos_spec,high_limit,background_spec);

  if(cnt_spec<NBSEG){
    cnt_spec=NBSEG;
  }
 

  if(cnt_spec>30){
    next=7;
  }else if(cnt_spec>20 && cnt_spec<30){
    next=6;
  }else if(cnt_spec>10 && cnt_spec<20){
    next=5;
  }else{
    next=3;
  }

  
  if(flag_M!=2){
    size_k=NBSEG;

    if(NULL==(max_pos_ref=malloc(sizeof(int)*size_k))){
      fprintf(stderr,"Malloc-error\n");
    }
     
    if(1!=sort_biggest(ref_spec,peak_pos_ref,cnt_ref,max_pos_ref,size_k,background_ref)){
      fprintf(stderr,"sort_biggest-error\n");
    }

    if(1!=sort_array(max_pos_ref,size_k)){
      fprintf(stderr,"sort_array-error\n");
    }
  }
     
  if(NULL==(max_pos_spec=malloc(sizeof(int)*cmp_no(size_k)))){
    fprintf(stderr,"Malloc-error\n");
  }
  
  if(1!=sort_biggest(spec,peak_pos_spec,cnt_spec,max_pos_spec,
		     cmp_no(size_k),background_spec)){
    fprintf(stderr,"sort_biggest-error\n");
  }

  if(1!=sort_array(max_pos_spec,cmp_no(size_k))){
    fprintf(stderr,"sort_array-error\n");
  }

  
  cnt_spec_comb=size_k*give_n_over_k(cmp_no(size_k),size_k);
  
  if(NULL==(max_pos_spec_comb=malloc(sizeof(int)*cnt_spec_comb))){
    fprintf(stderr,"Malloc-error\n");
  }
  
  if(cnt_spec_comb!=(cnt=give_comb_k(max_pos_spec,max_pos_spec_comb,
				     size_k,cmp_no(size_k)))){
    fprintf(stderr,"give_comb_k-error\n");
  }
   
  rel=rel_abw_pos5(max_pos_ref,max_pos_spec_comb,a_b_gut,k_l_gut,
		   cnt_spec_comb,size_k);

  if(rel<X){
    kl=k_l_gut[1];
    if(flag_F==0){
      pos_ref[0]=give_center(ref_spec,max_pos_ref[0],background_ref);
      pos_ref[1]=give_center(ref_spec,max_pos_ref[size_k-1],background_ref);
      pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
      pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],background_spec);
      give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
    }
    if(flag_F==1){
      pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
      pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],background_spec);
      pos_ref[0]=man_pos_ref_float[0];
      pos_ref[1]=man_pos_ref_float[size_k-1];
      give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
    }
  }
 
  if(rel>=X){
    for(j=1;size_k>4;j++){
      size_k=NBSEG-j;
      cnt_ref_comb=size_k*give_n_over_k(NBSEG,size_k);

      if(NULL==(max_pos_ref_comb=malloc(sizeof(int)*cnt_ref_comb))){
	fprintf(stderr,"Malloc-error\n");
      }

      cnt_spec_comb=size_k*give_n_over_k(cmp_no(size_k),size_k);

      if(NULL==(max_pos_spec_comb=malloc(sizeof(int)*cnt_spec_comb))){
	fprintf(stderr,"Malloc-error\n");
      }
      
      if(cnt_ref_comb!=(cnt=give_comb_k(max_pos_ref,max_pos_ref_comb,
					size_k,NBSEG))){
	fprintf(stderr,"give_comb_k-error\n");
      }

      if(NULL==(max_pos_spec=malloc(sizeof(int)*cmp_no(size_k)))){
	fprintf(stderr,"Malloc-error\n");
      }
      
      if(1!=sort_biggest(spec,peak_pos_spec,cnt_spec,max_pos_spec,
			 cmp_no(size_k),background_spec)){
	fprintf(stderr,"sort_biggest-error\n");
      }
      
      if(1!=sort_array(max_pos_spec,cmp_no(size_k))){
	fprintf(stderr,"sort_array-error\n");
      }

      if(cnt_spec_comb!=(cnt=give_comb_k(max_pos_spec,max_pos_spec_comb,
					 size_k,cmp_no(size_k)))){
	fprintf(stderr,"give_comb_k-error\n");
      }
    
      for(i=0;i<cnt_ref_comb;i+=size_k){
	rel_tmp=rel_abw_pos5(&max_pos_ref_comb[i],max_pos_spec_comb,
			     a_b_gut_tmp,k_l_gut,cnt_spec_comb,size_k);
	if(rel_tmp<rel){
	  rel=rel_tmp;
	  kl_ref=i;
	  kl=k_l_gut[1];
	  a_b_gut[0]=a_b_gut_tmp[0];
	  a_b_gut[1]=a_b_gut_tmp[1];
	}
      }
      if(rel<X){
	if(flag_F==0){
	  pos_ref[0]=give_center(ref_spec,max_pos_ref_comb[kl_ref],
				 background_ref);
	  pos_ref[1]=give_center(ref_spec,max_pos_ref_comb[kl_ref+size_k-1],
				 background_ref);
	  pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
	  pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],
			     background_spec);
	  give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
	  break;
	}
	if(flag_F==1){
	  pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
	  pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],
			     background_spec);
	  for(i=kl_ref;i<kl_ref+size_k;i++){
	    if(max_pos_ref_comb[kl_ref]==(int)man_pos_ref_float[i]){
	      pos_ref[0]=man_pos_ref_float[i];
	    }
	    if(max_pos_ref_comb[kl_ref+size_k-1]==(int)man_pos_ref_float[i]){
	      pos_ref[1]=man_pos_ref_float[i];
	    }
	  }
	  give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
	  break;
	}
      }
    }
  }
  
  if(plt==0){
    *shift_koef_a=a_b_gut[0];
    *shift_koef_b=a_b_gut[1];
    *mean_least_square=rel; 


    if(size_k==NBSEG){

      return(1);
    }

    if(size_k<NBSEG && size_k>4){
	
      return(-(NBSEG-size_k));

      /* Hier gibt die Funktion an, welche Peaks sie nicht gefunden hat.

      j=kl_ref;
      for(i=0;i<NBSEG;i++){
	if(max_pos_ref[i]!=max_pos_ref_comb[j]){
	  printf("%d  ",max_pos_ref[i]);
	  j--;
	}
	j++;
      }

      */

    }

    if(size_k==4){
      if(rel<X-1.0){
	
	return(-4);

      }
      if(rel>X-1.0){
	
	return(-10);
      }
    }
    if(flag_M==1){

      if(NULL==(man_pos_spec=malloc(sizeof(int)*M*next))){
	fprintf(stderr,"Malloc-error\n");
      }
      if(1!=manual_cal(man_pos_spec,next,man_pos,peak_pos_spec,a_b_gut,M,cnt_spec)){
	fprintf(stderr,"manual_cal-error\n");
	return(-10);
      }

      if(NULL==(man_pos_spec2=malloc(sizeof(int)*M*3))){
	fprintf(stderr,"Malloc-error\n");
      }

      for(i=0;i<M;i++){
	if(1!=sort_biggest(spec,&man_pos_spec[i*next],next,&man_pos_spec2[i*3],3,background_spec)){
	  fprintf(stderr,"sort_biggest-error\n");
	}
      }
     
      free(man_pos_spec);

      size_man=M*a_hoch_b(3,M);

      if(NULL==(man_pos_spec_comb=malloc(sizeof(int)*size_man))){
	fprintf(stderr,"Malloc-error\n");
      }

      if(size_man!=(cnt=give_comb_man(man_pos_spec_comb,man_pos_spec2,3,M))){
	fprintf(stderr,"give_comb_man-error\n");
      }  

      rel_man=rel_abw_pos5(man_pos,man_pos_spec_comb,a_b_gut_man,k_l_gut_man,size_man,M);
       
      kl_man=k_l_gut_man[1];
      pos_ref[0]=give_center(ref_spec,man_pos[0],background_ref);
      pos_ref[1]=give_center(ref_spec,man_pos[M-1],background_ref);
      pos[0]=give_center(spec,man_pos_spec_comb[kl_man],background_spec);
      pos[1]=give_center(spec,man_pos_spec_comb[kl_man+M-1],background_spec);
      give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut_man);
      
      *shift_koef_a=a_b_gut_man[0];
      *shift_koef_b=a_b_gut_man[1];
      *mean_least_square=rel_man; 
    }
  }
}


int bs_shift_R(int *max_pos_ref,int M,unsigned long *ref_spec,unsigned long *spec,int res,int flag_C,int LOW_LIMIT_C,int flag_H,int HIGH_LIMIT,int LOW_LIMIT,int CMP_FAC,float *shift_koef_a,float *shift_koef_b,float *mean_least_square)
{
  int *peak_pos_ref;
  int *peak_pos_spec;
  int *max_pos_ref_comb;
  int *max_pos_spec,*max_pos_spec_comb;
  int *man_pos_spec,*man_pos_spec2,*man_pos_spec_comb;
  float *man_pos_ref_float;
  int res2,res3;
  int i,j,x,cnt;
  int cnt_ref,cnt_spec,cnt_spec_comb,cnt_ref_comb;
  int plt=0;
  double clear;
  double background_ref,background_spec;
  int *right_peaks;
  char *arg;
  int high_limit;
  float a_b_gut[2],a_b_gut_tmp[2];
  float pos_ref[2],pos[2];
  int k_l_gut[2],kl,kl_ref;
  int flag_M=2,flag_F=0;
  float rel,rel_tmp,dx;
  int size_k;
  float jp_tst;
  int man_pos[6];
  int next=3;
  int size_man;
  float rel_man,a_b_gut_man[2];
  int k_l_gut_man[2];
  int kl_man;
  float int_diff;
  int cnt_spec_jp;
  double *vc;
  double dE,dE_ref;
  int *dop_ref,*dop_spec;
  int shifted_ref,shifted_spec;
  FILE *ref_F;

  if(flag_M==2){
    NBSEG=M;
  }

  if(flag_H==0){
    HIGH_LIMIT=res;
  }

  if(flag_M==2){
    if(NULL==(max_pos_ref=malloc(sizeof(int)*M))){
      fprintf(stderr,"Malloc-error\n");
    }
  }
  
  if(flag_C){
    LOW_LIMIT=LOW_LIMIT_C;
  }

  cnt_spec=count_max(spec,res3,&background_spec,&high_limit);
  
  if(NULL==(peak_pos_spec=malloc(sizeof(int)*cnt_spec))){
    fprintf(stderr,"Malloc-error\n");
  }

  for(i=0;i<cnt_spec;i++){
    peak_pos_spec[i]=0;
  }
  spec[0]=0;
  
  cnt_spec=give_max2(spec,peak_pos_spec,high_limit,background_spec);

  if(cnt_spec<NBSEG){
    cnt_spec=NBSEG;
  }
 

  if(cnt_spec>30){
    next=7;
  }else if(cnt_spec>20 && cnt_spec<30){
    next=6;
  }else if(cnt_spec>10 && cnt_spec<20){
    next=5;
  }else{
    next=3;
  }
  
  if(flag_M==2){
    size_k=M;
  }
   
  if(NULL==(max_pos_spec=malloc(sizeof(int)*cmp_no(size_k)))){
    fprintf(stderr,"Malloc-error\n");
  }
  
  if(1!=sort_biggest(spec,peak_pos_spec,cnt_spec,max_pos_spec,
		     cmp_no(size_k),background_spec)){
    fprintf(stderr,"sort_biggest-error\n");
  }

  if(1!=sort_array(max_pos_spec,cmp_no(size_k))){
    fprintf(stderr,"sort_array-error\n");
  }
  
  cnt_spec_comb=size_k*give_n_over_k(cmp_no(size_k),size_k);
  
  if(NULL==(max_pos_spec_comb=malloc(sizeof(int)*cnt_spec_comb))){
    fprintf(stderr,"Malloc-error\n");
  }
  
  if(cnt_spec_comb!=(cnt=give_comb_k(max_pos_spec,max_pos_spec_comb,
				     size_k,cmp_no(size_k)))){
    fprintf(stderr,"give_comb_k-error\n");
  }
   
  rel=rel_abw_pos5(max_pos_ref,max_pos_spec_comb,a_b_gut,k_l_gut,
		   cnt_spec_comb,size_k);

  if(rel<X){
    kl=k_l_gut[1];
    if(flag_F==0){
      pos_ref[0]=give_center(ref_spec,max_pos_ref[0],background_ref);
      pos_ref[1]=give_center(ref_spec,max_pos_ref[size_k-1],background_ref);
      pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
      pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],background_spec);
      give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
    }
    if(flag_F==1){
      pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
      pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],background_spec);
      pos_ref[0]=man_pos_ref_float[0];
      pos_ref[1]=man_pos_ref_float[size_k-1];
      give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
    }
  }
 
  if(rel>=X){
    for(j=1;size_k>4;j++){
      size_k=NBSEG-j;
      cnt_ref_comb=size_k*give_n_over_k(NBSEG,size_k);

      if(NULL==(max_pos_ref_comb=malloc(sizeof(int)*cnt_ref_comb))){
	fprintf(stderr,"Malloc-error\n");
      }

      cnt_spec_comb=size_k*give_n_over_k(cmp_no(size_k),size_k);

      if(NULL==(max_pos_spec_comb=malloc(sizeof(int)*cnt_spec_comb))){
	fprintf(stderr,"Malloc-error\n");
      }
      
      if(cnt_ref_comb!=(cnt=give_comb_k(max_pos_ref,max_pos_ref_comb,
					size_k,NBSEG))){
	fprintf(stderr,"give_comb_k-error\n");
      }

      if(NULL==(max_pos_spec=malloc(sizeof(int)*cmp_no(size_k)))){
	fprintf(stderr,"Malloc-error\n");
      }
      
      if(1!=sort_biggest(spec,peak_pos_spec,cnt_spec,max_pos_spec,
			 cmp_no(size_k),background_spec)){
	fprintf(stderr,"sort_biggest-error\n");
      }
      
      if(1!=sort_array(max_pos_spec,cmp_no(size_k))){
	fprintf(stderr,"sort_array-error\n");
      }

      if(cnt_spec_comb!=(cnt=give_comb_k(max_pos_spec,max_pos_spec_comb,
					 size_k,cmp_no(size_k)))){
	fprintf(stderr,"give_comb_k-error\n");
      }
    
      for(i=0;i<cnt_ref_comb;i+=size_k){
	rel_tmp=rel_abw_pos5(&max_pos_ref_comb[i],max_pos_spec_comb,
			     a_b_gut_tmp,k_l_gut,cnt_spec_comb,size_k);
	if(rel_tmp<rel){
	  rel=rel_tmp;
	  kl_ref=i;
	  kl=k_l_gut[1];
	  a_b_gut[0]=a_b_gut_tmp[0];
	  a_b_gut[1]=a_b_gut_tmp[1];
	}
      }
      if(rel<X){
	if(flag_F==0){
	  pos_ref[0]=give_center(ref_spec,max_pos_ref_comb[kl_ref],
				 background_ref);
	  pos_ref[1]=give_center(ref_spec,max_pos_ref_comb[kl_ref+size_k-1],
				 background_ref);
	  pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
	  pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],
			     background_spec);
	  give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
	  break;
	}
	if(flag_F==1){
	  pos[0]=give_center(spec,max_pos_spec_comb[kl],background_spec);
	  pos[1]=give_center(spec,max_pos_spec_comb[kl+size_k-1],
			     background_spec);
	  for(i=kl_ref;i<kl_ref+size_k;i++){
	    if(max_pos_ref_comb[kl_ref]==(int)man_pos_ref_float[i]){
	      pos_ref[0]=man_pos_ref_float[i];
	    }
	    if(max_pos_ref_comb[kl_ref+size_k-1]==(int)man_pos_ref_float[i]){
	      pos_ref[1]=man_pos_ref_float[i];
	    }
	  }
	  give_a_b_prec(pos_ref[0],pos_ref[1],pos[0],pos[1],a_b_gut);
	  break;
	}
      }
    }
  }
  
  if(plt==0){
    *shift_koef_a=a_b_gut[0];
    *shift_koef_b=a_b_gut[1];
    *mean_least_square=rel; 

    if(size_k==NBSEG){
      
      return(1);
    }

    if(size_k<NBSEG && size_k>4){
      
      return(-(NBSEG-size_k));
      
      /* Hier gibt die Funktion an, welche Peaks sie nicht gefunden hat.

      j=kl_ref;
      for(i=0;i<NBSEG;i++){
	if(max_pos_ref[i]!=max_pos_ref_comb[j]){
	  printf("%d  ",max_pos_ref[i]);
	  j--;
	}
	j++;
      }

      */

    }

    if(size_k==4){
      if(rel<X-1.0){
	
	return(-4);

      }
      if(rel>X-1.0){
	
	return(-10);

      }
    } 
  }
}




