#ifndef DS_SORT_H
#define DS_SORT_H
#include"common.h"
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   Prototypes for the Deep Shallow Suffix Sort routines
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */ 
#ifdef __cplusplus
extern "C"{
void ds_ssort(UChar *t, Int64 *sa, Int64 n); // 64 bit version
Int64 init_ds_ssort(Int64 adist, Int64 bs_ratio);
}
#endif
void ds_ssort(UChar *t, Int64 *sa, Int64 n); // 64 bit version
Int64 init_ds_ssort(Int64 adist, Int64 bs_ratio);
#endif //DS_SORT_H

