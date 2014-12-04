#ifndef BWT_AUX_H
#define BWT_AUX_H

// before including this file, first include common.h!!


#define _BW_ALPHA_SIZE ALPHABET_SIZE // already defined ALPHABET_SIZE 256 in common.h

// already defined in common.h
//typedef unsigned char uchar;
//typedef unsigned char uint8;
//typedef int int32;

// ---- struct containing the (uncompressed) bwt 
typedef struct {
  UChar *bwt;
  //int size;
  //int eof_pos;
  long size;
  long eof_pos;
} bwt_data;


// prototypes of bwt procedures defined in bwtlcp.a
//void _bw_sa2bwt(uchar *t, int32 n, int32 *sa, bwt_data *b);
//
//int32 _bw_bwt2ranknext(bwt_data *b, int32* occ, int32 *rank_next);
//int32 _bw_sa2ranknext(uchar *t,int32 n,int32 *sa,int32 *occ,int32 *rank_next);
//void _bw_ranknext2t(int32 *rank_next, int32 r0, bwt_data *b, uchar *t);
//void _bw_ranknext2sa(int32 *rank_next, int32 r0, int32 *sa);
//
//int32 _bw_bwt2rankprev(bwt_data *b, int32* occ, int32 *rank_prev);
//int32 _bw_sa2rankprev(uchar *t,int32 n,int32 *sa,int32 *occ,int32 *rank_prev);
//void _bw_rankprev2t(int32 *rank_prev, int32 rn1, bwt_data *b, uchar *t);
//void _bw_rankprev2sa(int32 *rank_prev, int32 n, int32 rn1, int32 *sa);

// 64 bit versions
void _bw_sa2bwt(UChar *t, long n, long *sa, bwt_data *b);

long _bw_bwt2ranknext(bwt_data *b, long* occ, long *rank_next);
long _bw_sa2ranknext(UChar *t, long n, long *sa, long *occ, long *rank_next);

void _bw_ranknext2t(long *rank_next, long r0, bwt_data *b, UChar *t);
void _bw_ranknext2sa(long *rank_next, long r0, long *sa);

long _bw_bwt2rankprev(bwt_data *b, long* occ, long *rank_prev);
long _bw_sa2rankprev(UChar *t, long n, long *sa, long *occ, long *rank_prev);
void _bw_rankprev2t(long *rank_prev, long rn1, bwt_data *b, UChar *t);
void _bw_rankprev2sa(long *rank_prev, long n, long rn1, long *sa);

#endif //BWT_AUX_H
