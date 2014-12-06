/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   global defintion for the ds suffix-sort algorithm 
   Giovanni Manzini 
   2-apr 2001
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
#ifndef COMMON_H
#define COMMON_H

/* ----------- use assertion if DEBUG!=0 ------------- */
#ifndef DEBUG
#define DEBUG 0   /* set DEBUG to 0 to remove assertions and extra checks */
#endif
#if !DEBUG
#define NDEBUG 1  /* do not compile assertions */
#endif
#include <assert.h>

/* ---------- types and costants ----------- */
#ifndef INT64
#define INT64

	typedef long long Int64;

#endif // INT64

#ifndef UINT64
#define UINT64

	typedef unsigned long long UInt64;

#endif // UINT64

#ifndef UCHAR
#define UCHAR
	
	typedef unsigned char UChar;

#endif // UCHAR

	
typedef int				Int32;
typedef unsigned int	UInt32;
typedef unsigned short	UInt16;
typedef char				Char;
typedef unsigned char		Bool;
#define True   ((Bool)1)
#define False  ((Bool)0)
#define Cmp_overshoot 16 // can it stay like this for 64 bit version ?? 
#define Max_thresh 30

#if !defined(LLONG_MAX)
   #define LLONG_MAX 0x7fffffffffffffffLL
#endif

#ifndef min
#define min(a, b) ((a)<=(b) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) ((a)>=(b) ? (a) : (b))
#endif


#define MIN(a, b) ((a)<=(b) ? (a) : (b))
#define MAX(a, b) ((a)>=(b) ? (a) : (b))


// constant and macro for marking groups

// #define SETMASK (1 << 30)      // used in ds.c to check the size of the suffix starting with sb combination of 2 char-s
						// 32 bit version ; text size at most 2^31 bytes
#define SETMASK (1LL << 62) // 64 bit version ; text size at most 2^63 bytes 1LL because it is of long long type
#define CLEARMASK (~(SETMASK))
#define IS_SORTED_BUCKET(sb) (ftab[sb] & SETMASK)
#define BUCKET_FIRST(sb) (ftab[sb]&CLEARMASK)
#define BUCKET_LAST(sb) ((ftab[sb+1]&CLEARMASK)-1)
#define BUCKET_SIZE(sb) ((ftab[sb+1]&CLEARMASK)-(ftab[sb]&CLEARMASK))

#define ALPHABET_SIZE 256 
// to avoid referring to the constants 256 and 65535 in the code in the case of changing the size of alphabet

//int scmp3(unsigned char *p, unsigned char *q, int *l, int maxl);
Int64 scmp3(unsigned char *p, unsigned char *q, Int64 *l, Int64 maxl); // 64 bit version
void pretty_putchar(int c);

#endif //COMMON_H
