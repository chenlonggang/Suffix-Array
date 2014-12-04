#ifndef DS_SORT_H
#define DS_SORT_H

#include"common.h"
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   Prototypes for the Deep Shallow Suffix Sort routines
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */ 

#ifdef __cplusplus
extern "C"{
//void ds_ssort(unsigned char *t, int *sa, int n);
void ds_ssort(UChar *t, long *sa, long n); // 64 bit version
//int init_ds_ssort(int adist, int bs_ratio);
long init_ds_ssort(long adist, long bs_ratio);
}
#endif
void ds_ssort(UChar *t, long *sa, long n);
long init_ds_ssort(long adist, long bs_ratio);
#endif //DS_SORT_H

