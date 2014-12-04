#ifndef LCP_AUX_H
#define LCP_AUX_H


// prototypes of the lcp construction procedures defined in
// bwtlcp.a (source files lcp_aux.c lcp5_aux.c)
// before including this file you must first include bwt_aux.h

//int *_lcp_sa2lcp_13n(uint8 *t, int n, int *sa);
//// int *_lcp_vm_9125n(uint8 *t, int n, int *sa); // definition is not included in the project
//int *_lcp_vmjk_9125n(uint8 *t, int n, int *sa);
//int *_lcp_sa2lcp_9n(uint8 *t, int n, int *sa, int *occ);
//int _lcp_sa2lcp_6n(uint8 *t, bwt_data *b, int *sa, int *occ);
////int _lcp_sa2lcp_5125n(uint8 *t, int n, int *sa, int *occ); // definition is not included in the project!!
////int _lcp_sa2lcp_5125nme(uint8 *t, int n, int *sa, int *occ); // definition is not included in the project!!

// 64-bit version of code
long *_lcp_sa2lcp_13n(UChar *t, long n, long *sa); 

long *_lcp_jk_13n(UChar *t, long n, long *sa); //there is a definition in lcp_aux.c, so I've added a declaration in here!

long *_lcp_vmjk_9125n(UChar *t, long n, long *sa);
long *_lcp_sa2lcp_9n(UChar *t, long n, long *sa, long *occ);
//int *_lcp_sa2lcp_9n(UChar *t, long n, long *sa, long *occ);
//long _lcp_sa2lcp_6n(UChar *t, bwt_data *b, long *sa, long *occ); // bwt_data --> defined in bwt_aux.h



#endif //LCP_AUX_H

