int bs_shift(unsigned long *ref_spec,unsigned long *spec,int res,int NBSEG,int flag_C,int LOW_LIMIT_C,int flag_H,int HIGH_LIMIT,int LOW_LIMIT,int CMP_FAC,float *shift_koef_a,float *shift_koef_b,float *mean_least_square);

int bs_shift_default(unsigned long *ref_spec,unsigned long *spec,int res,float *shift_koef_a,float *shift_koef_b,float *mean_least_square);

int bs_shift_F(float *man_pos_ref_float,int M,unsigned long *spec,int res,int flag_C,int LOW_LIMIT_C,int CMP_FAC,float *shift_koef_a,float *shift_koef_b,float *mean_least_square);


int bs_shift_R(int *max_pos_ref,int M,unsigned long *ref_spec,unsigned long *spec,int res,int flag_C,int LOW_LIMIT_C,int flag_H,int HIGH_LIMIT,int LOW_LIMIT,int CMP_FAC,float *shift_koef_a,float *shift_koef_b,float *mean_least_square);

int bs_shift_M(int *man_pos,int M,unsigned long *ref_spec,unsigned long *spec,int res,int NBSEG,int flag_C,int LOW_LIMIT_C,int flag_H,int HIGH_LIMIT,int LOW_LIMIT,int CMP_FAC,float *shift_koef_a,float *shift_koef_b,float *mean_least_square);
