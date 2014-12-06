/* *******************************************************************
   globals.c
   Ver 1.0   14-oct-02
   This file contains the definition of the global variables 
   which can be defined by the user + some relate procedures
   ******************************************************************* */
#include <stdio.h>
#include "common.h"
#include "ds_ssort.h"


/* ---- global variables (modifiable by command line options) ----- */

// *********************** 32 bit version ***********************
//int Anchor_dist;                // distance between anchors
//int Shallow_limit;              // limit for shallow_sort
//int _ds_Verbose;                // how verbose it the algorithm?
//int _ds_Word_size;              // # of bytes in word in mkqs
//int Mk_qs_thresh;               // recursion limit for mk quicksort:
//Int32 Max_pseudo_anchor_offset; // maximum offset considered when 
//                                // searching a pseudo anchor
//Int32 B2g_ratio;                // maximum ratio bucket_size/group_size
//                                // accepted for pseudo anchor_sorting
//Int32 Update_anchor_ranks;      // if!=0 update anchor ranks when determining
//                                // rank for pseudo-sorting
//Int32 Blind_sort_ratio;         // blind sort is used for groups of size 
//                                // <= Text_size/Blind_sort_ratio



// *********************** 64 bit version ***********************

Int64 Anchor_dist;							// distance between anchors 
Int64 Shallow_limit;						// limit for shallow_sort 

int _ds_Verbose;                // how verbose it the algorithm?
int _ds_Word_size;              // # of bytes in word in mkqs (1, 2, 4, 8??)

Int64 Mk_qs_thresh;            // recursion limit for mk quicksort:

Int64 Max_pseudo_anchor_offset; // maximum offset considered when 
                                // searching a pseudo anchor

Int64 B2g_ratio;                // maximum ratio bucket_size/group_size
                                // accepted for pseudo anchor_sorting

Int64 Update_anchor_ranks;      // if!=0 update anchor ranks when determining
                                // rank for pseudo-sorting

Int64 Blind_sort_ratio;         // blind sort is used for groups of size 
                                // <= Text_size/Blind_sort_ratio


int check_global_variables(void);
void set_global_variables(void);
Int64 compute_overshoot(void);

/* *******************************************************************
   procedure to be called by external program before calling ds_ssort()
   using this procedure external programs can choose
   the parameters Anchor_dist and Blind_sort_ratio.
   The procedure returns 0 if something goes wrong, otherwise 
   it returns the overshhot, that is the amount of extra space
   required at the end of the array contanining the text
   ******************************************************************** */
//int init_ds_ssort(int adist, int bs_ratio)
Int64 init_ds_ssort(Int64 adist, Int64 bs_ratio)
{
  set_global_variables();
  Anchor_dist = adist;
  Blind_sort_ratio = bs_ratio;
  Shallow_limit =  Anchor_dist + 50;
	//Shallow_limit =  Anchor_dist + 1000000; // Shallow_limit = the longest of the lcp-s!
  if (check_global_variables()) {
    return 0;
  }
  return compute_overshoot();
}


// set default values for the global variables
void set_global_variables(void)
{
  Blind_sort_ratio = 2000;
	//Blind_sort_ratio = 20000;
  Anchor_dist = 500;
	// Anchor_dist = 10000;
  Shallow_limit =  Anchor_dist + 50;
	//Shallow_limit = Anchor_dist + 1000;
  _ds_Verbose = 0;
  _ds_Word_size = 1; // the fastest
  //_ds_Word_size = 8; //since now it's a 64 bit version
  Mk_qs_thresh = 20; 
  Max_pseudo_anchor_offset = 0;
  //B2g_ratio = 1000;
	B2g_ratio = 10000;
  Update_anchor_ranks=0;
}

// check if the global variables passed as parameters
// are in the valid range
int check_global_variables(void)
{
  if((Anchor_dist < 100) && (Anchor_dist != 0)) {
    fprintf(stderr,"Anchor distance must be 0 or greater than 99\n");
    return 1;
  }
  if(Anchor_dist > 65535) { // ??????????? what about 64 bit version ??
    fprintf(stderr,"Anchor distance must be less than 65536\n");
    return 1;
  }
//  if(Anchor_dist > LLONG_MAX) { 
//    fprintf(stderr,"Anchor distance must be less than %lld\n", LLONG_MAX);
//    return 1;
//  }
  if(Shallow_limit < 2) {
    fprintf(stderr,"Illegal limit for shallow sort\n");
    return 1;
  }
  if(Mk_qs_thresh < 0 || Mk_qs_thresh > Max_thresh) {
    fprintf(stderr,"Illegal Mk_qs_thresh parameter!\n");
    return 1;
  }
  if(Blind_sort_ratio <= 0) {
    fprintf(stderr,"blind_sort ratio must be greater than 0!\n");
    return 1;
  }
  return 0;
}


// compute the amount of extra memory required 
// for the input text 
Int64 compute_overshoot(void)
{
  return 9 + (Shallow_limit + Cmp_overshoot);
}

// ----- this function prints any char in a readable form
void pretty_putchar(int c)
{
  
  if(c >= 32 && c < 127)      // printable char
    printf("  %c", c);
  else if(c == '\n')
    printf(" \\n");        // \n
  else if(c == '\t')
    printf(" \\t");        // \t
  else     
    printf(" %02x", c);      // print hex code
}


//int scmp3(unsigned char *p, unsigned char *q, int *l, int maxl)
Int64 scmp3(unsigned char *p, unsigned char *q, Int64 *l, Int64 maxl) // 64 bit version
{
   Int64 i;
   i = 0;
   while (maxl>0 && *p==*q) {
      p++; q++; i++;
      maxl--;
   }
   *l = i;
   if (maxl>0) return *p-*q;
   return q-p;
}






