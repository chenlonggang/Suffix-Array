/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   test1.c 
   Ver 1.0  14/03/20008
   Test algorithms for computing the ds suffix sort. 

   Copyright (C) 2004 Giovanni Manzini (manzini@mfn.unipmn.it)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

   See COPYRIGHT file for further copyright information	   
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <assert.h>

//#include <mcheck.h> 

/* --- read proptotypes and typedef for ds_ssort --- */
#include "common.h" 
#include "ds_ssort.h"
#include "bwt_aux.h"
#include "lcp_aux.h"


// ----- global variables -------
static int Check_lcp;           // for later use of the lcp
static int Verbose;
static FILE *Infile;               // input file 
//static FILE *Outfile;              // output file for the SA
//static FILE *OutfileLCP;           // output file for the LCP array

static Int64 Infile_size;       // size input file;

/*********** new global variables; added because of LCP ****************/
UChar *text; // input text
Int64 *sa;   // suffix array
Int64 n;     // number of char-s in the input text
Int64 *lcp, *lcp9; // lcp and lcp9 array
//int *lcp, *lcp9; // lcp and lcp9 array

// ----- prototypes ---------
void fatal_error (char *); // displays the text of the fatal error on the stderr
void out_of_mem (char *f); // displays the "out of memory" message on the stderr
double getTime ( void ); // calculates user+system time in seconds

void open_files(char *infile_name); // opens the input file
void open_output_files(char *outfile_name, FILE **Output); // opens the output files

void ds_ssorter(FILE *Outfile); // new function that does all the sorting
void lcp_file(FILE *OutfileLCP); // a function that calculates the lcp array


static void printLCP(char *lcpType, FILE *OutfileLCP); // static - because it should be used only in this file
//static void printLCP6(FILE *OutfileLCP);

/* ***************************************************************
   Sample computation of the lcp array using the deep/shallow
   suffix sorter and the routines in the bwtlcp.a
   
   This procedure reads the input parameters from the command line
   and calls lcp_file() that does all the testing.
   *************************************************************** */


int main(int argc, char *argv[])
{  
  // double getTime ( void ); // already defined afore the main function
  // void open_files(char *infile_name); // moved afore the main function
  //void lcp_file(void); // moved afore the main function, that is, its counterpart ds_ssorter()
  double end, start;
  char *infile_name;
  char *outfile_name;     // outfile --> array of sorted suffixes
  char *outfile_lcp_name; // outfile_lcp --> lcp array
  int c;
  extern char *optarg;
  extern int optind, opterr, optopt;

  FILE *Outfile;              // output file for the SA
  FILE *OutfileLCP;           // output file for the LCP array

  if(argc < 3) {
    fprintf(stderr, "Usage:\n\t%s [-Cv] infile outfile outfile_lcp",argv[0]);
    // current version with the lcp option
	  // fprintf(stderr,"[-v]\n"); // previous version without the lcp option
    fprintf(stderr,"\t-C           check lcp array (very slow!)\n");
    fprintf(stderr,"\t-v           verbose output\n\n");
    exit(0);
  }

  /* ------------- read command line options ----------- */
  Verbose = 0;                       // be quiet by default
  Check_lcp = 0;                     // do not check lcp by default
  infile_name = NULL;
  outfile_name = NULL;
  outfile_lcp_name = NULL;
  opterr = 0;
  while ((c = getopt(argc, argv, "Cv")) != -1) {
    //fprintf(stdout,"option: %c\n", optopt);
    switch (c)
    {
      case 'v':
		    Verbose ++; 
      case 'C':
	      Check_lcp=1; 
        break;
      case '?':
        fprintf(stderr,"Unknown option: %c -main-\n", optopt);
        exit(1);
    }
  }
  if(optind < argc) {
     infile_name = argv[optind];
     outfile_name = argv[optind + 1]; 
     outfile_lcp_name = argv[optind + 2]; 
  }

  // ----- check input parameters--------------
  if(infile_name == NULL) fatal_error("The input file name is required\n");
  if(outfile_name == NULL) fatal_error("The SA output file name is required\n");
  if(outfile_lcp_name == NULL) fatal_error("The LCP output file name is required\n");

  // ?? different values of Verbose variable indicate differences in the verbose output, 
  // that is: how many details will be displayed
  if(Verbose > 2) {
    fprintf(stderr,"\n*****************************************************");
    fprintf(stderr,"\n             testlcp  Ver 1.2\n");
    fprintf(stderr,"Created on %s at %s from %s\n",__DATE__,__TIME__,__FILE__);
    fprintf(stderr,"*****************************************************\n");
  }
  if(Verbose > 1) {
    fprintf(stderr,"Command line: ");
	for(c = 0; c < argc; c++) {
      fprintf(stderr,"%s ",argv[c]);
	}
    fprintf(stderr,"\n");
  }

  // ---- do the work ---------------------------
  start = getTime();
  open_files(infile_name);
  open_output_files(outfile_name, &Outfile);
  open_output_files(outfile_lcp_name, &OutfileLCP);

  ds_ssorter(Outfile); // previous version
  lcp_file(OutfileLCP);   // current version --> uses sa produced in ds_ssorter

  free(text); // moved from ds_ssorter function, because text and sa are needed in lcp_file as well
  free(sa);

  end = getTime();
  if(Verbose) {
    fprintf(stderr,"Elapsed time: %f seconds.\n", end - start);
  }

  fclose(Infile);
  fclose(Outfile);
  fclose(OutfileLCP);
  return 0;
}

/* ***************************************************************
   compute the suffix array using deep/shallow sufix sort algorithm
   *************************************************************** */
void ds_ssorter(FILE *Outfile) {

  //int *sa, n, overshoot, i, extra_bytes;  // change type for 64-bit version
  
  //UChar *text; // now a global var
  // Int64 *sa, n; // now global variables 

  Int64 overshoot, i;
  double start, end;
  Int64 pos; // temp test variable 

  // ----- init ds suffix sort routine
  // ?? this block should be rechecked for 64-bit version  because of changing the type of variables ******************
  overshoot = init_ds_ssort(500, 2000); // defined in globals.c
	//overshoot = init_ds_ssort(500000,20000); // defined in globals.c
  if(overshoot == 0) {
    fatal_error("ds initialization failed! (lcp_file)\n");
  }
  #if 0 // debug
     printf("overshoot = %lld\n", overshoot);  
  #endif

  // ----- allocate text and suffix array
  // ?? this block should be rechecked for 64-bit version  because of mem. allocation ******************
  n = Infile_size;                                    // length of input text
  sa = malloc((n + 1) * sizeof * sa);                   // suffix array
  if (!sa) {
	  out_of_mem("ds_ssorter - sa");
  }

  text = malloc((n + overshoot) * sizeof * text);            // text
  if (!text) {
	  free(sa); // free sa, because it was correctly allocated, and out_of_mem doesn't do the deallocation
	  out_of_mem("ds_ssorter - text");
  }

  // ----- read text and build suffix array
  rewind(Infile); 
  // ?? this block should be rechecked for 64-bit version because of size_t ******************
  i = fread(text, (size_t) 1, (size_t) n, Infile); // reads the whole text at once
  if(i != n) {
	  fatal_error("Error reading the input file!");
  }
  fprintf(stdout,"File size: %lld\n", n);

  // ----- build suffix array ----------------
  // ?? this block should be rechecked for 64-bit version because it uses sa and n ******************
  start = getTime();
  ds_ssort(text, sa + 1, n); // sort suffixes; defined in ds.c
  end = getTime();
  fprintf(stdout, "Suffix array construction: %.2f\n", end - start);

  // control output
  #if 1 // debug
     for (pos = 1; pos <= n; pos ++ ) { // write first 50 char-s of suffix
       fprintf(Outfile, "%lld %-50.50s\n", sa[pos] + 1, text + sa[pos]); // suffixes are sorted from 1st position to n-th
       if (pos >= 10) {
         // before break, print the last suffix ; that is at most 10+1 suffixes 
         fprintf(Outfile, "\n\n%lld %-50.50s\n", sa[n] + 1, text + sa[n]); // suffixes are sorted from 1st position to n-th
         break; // this is just for testing; break after first 10+1 
       }
     }
  #endif
  //free(text); // moved to main
  //free(sa);
}
 

/* ***************************************************************
   compute the lcp array using different linear time algorithms
   *************************************************************** */
void lcp_file(FILE *OutfileLCP)
{
  //void cmp_lcp_array(int *lcp1, char *name1, int *lcp2, char *name2, int n);
  //void check_lcp_array(uint8 *t, int n, int *sa, int *lcp);
  void cmp_lcp_array(Int64 *, char *, Int64 *, char *);
  void check_lcp_array();

  //UChar *text;
  //int *sa, n, overshoot, i, extra_bytes;
  //int occ[ALPHABET_SIZE], *lcp9, *lcp=NULL;
  //bwt_data b;
  //double start, end;
  //uint64 tot_lcp;

  //Int64 *sa, n, overshoot; // sa and n are now globals and overshoot is used in ds_sorter
  Int64 i; //, extra_bytes;
  Int64 occ[ALPHABET_SIZE]; //*lcp9, *lcp=NULL; --> lcp-s are now global variables
  //bwt_data b;
  double start, end;
  UInt64 tot_lcp;
  
  // ---- compute lcp using 9n algorithm  
  lcp=NULL;
  lcp9=NULL;

  start = getTime();
  for(i=0;i<ALPHABET_SIZE;i++) occ[i]=0;
  for(i=0;i<n;i++) occ[text[i]]++;
  lcp9 = _lcp_sa2lcp_9n(text,n,sa,occ);
  if (lcp9 == NULL) {
    out_of_mem("_lcp_sa2lcp_9n");
  }
  end=getTime();
  fprintf(stdout,"\nlcp9 construction: %.2f\n",end-start);
  // check (very slowly) the correctness of lcp array
  
  /*
	if(Check_lcp) check_lcp_array();
	*/
  // print lcp9 in the output file
  #if 1 // debug
    printLCP("lcp9", OutfileLCP);
  #endif
  //free(lcp9); // not now, because the comparison to other lcp algorithms should be done

  // --- compute lcp using 13n algorithm 
/*
	start = getTime();
  lcp = _lcp_sa2lcp_13n(text, n, sa);
  end=getTime();
  fprintf(stdout,"lcp13 construction: %.2f\n",end-start);
  // ---- check lcp13 vs lcp9
  cmp_lcp_array(lcp, "lcp13", lcp9, "lcp9");

  // print lcp13 in the output file
  #if 1 // debug
    printLCP("lcp13", OutfileLCP);
  #endif
  free(lcp);

  // ---- compute lcp using 9.125 n algorithm (Makinen+jk)  
  start = getTime();
  lcp = _lcp_vmjk_9125n(text,n,sa);
  end=getTime();
  fprintf(stdout,"lcp9125vmjk construction: %.2f\n",end-start);
  // ---- check lcp9125 vs lcp9
  cmp_lcp_array(lcp, "lcp9125vmjk", lcp9, "lcp9");

  // print lcp9.125 in the output file
  #if 1 // debug
    printLCP("lcp9.125", OutfileLCP);
  #endif
  free(lcp);
*/
  // ---- compute lcp using 6n algorithm
  //start = getTime();
  //if( (b.bwt = (UChar *) malloc(n+1)) == NULL) {
  //  out_of_mem("lcp_file");
  //}
  //_bw_sa2bwt(text, n, sa, &b);
  //for(i=0;i<ALPHABET_SIZE;i++) occ[i]=0;
  //for(i=0;i<n;i++) occ[text[i]]++;
  //extra_bytes = _lcp_sa2lcp_6n(text,&b,sa,occ);
  //end=getTime();
  //fprintf(stdout,"lcp6 construction: %.2f\n",end-start);
  //fprintf(stdout,"Total memory for lcp6: %.2fn bytes\n",6+(4.0*extra_bytes)/n);
  //free(b.bwt);

  //// ---- check lcp6 vs lcp9
  //cmp_lcp_array(sa, "lcp6", lcp9, "lcp9");

  // ---- compute average lcp (which is now in sa[])
  tot_lcp=0;
  //for(i=1;i<n;i++) 
//  for(i=2;i<n;i++) // exclude lcp[1] because it is undefined!!
//    tot_lcp += sa[i]; // but this only if sa is used for lcp, otherwise the lcp array should be used
// lcp cannot be used because it's freed by now, only lcp9

	//for(i=2;i<n;i++) // exclude lcp[1] because it is undefined!!
 //   tot_lcp += lcp9[i]; // but this only if sa is used for lcp, otherwise the lcp array should be used
 // fprintf(stdout,"Average lcp: %.2lf\n",((double) tot_lcp)/(n-1));
 // 
  // print lcp6 in the output file
  #if 0 // debug
    //printLCP6(OutfileLCP); // cannot use printLCP becuase the lcp6 has run over the sa
  #endif
  
  free(lcp9);
}



/* *******************************************************************
   compare the lcp arrays cmputed by algs. name1, name2
   ******************************************************************* */ 
//void cmp_lcp_array(int *lcp1, char *name1, int *lcp2, char *name2, int n)
void cmp_lcp_array(Int64 *lcp1, char *name1, Int64 *lcp2, char *name2)
{
  //int i, diff;
  Int64 i, diff;
  
  diff=0;
  for(i=2;i<=n;i++)
    if(lcp1[i]!=lcp2[i]) { 
      diff++; 
      if(Verbose>1)
	      //fprintf(stdout,"%s[%d]=%d  %s[%d]=%d\n", name1,i,lcp1[i],name2,i,lcp2[i]);
        fprintf(stdout,"%s[%lld]=%lld  %s[%lld]=%lld\n", name1,i,lcp1[i],name2,i,lcp2[i]);
    }
  
  if(diff>0)
    //fprintf(stdout,"FATAL ERROR! %s/%s differences: %d\n",name1,name2,diff);
    fprintf(stdout,"FATAL ERROR! %s/%s differences: %lld\n",name1,name2,diff);
}

/* *******************************************************************
   check the correctness of a lcp array with a (very slow) 
   character by character comparison of consecutive suffixes
   ******************************************************************* */ 
//void check_lcp_array(uint8 *t, int n, int *sa, int *lcp)
void check_lcp_array()
{
  //int i,j,k,h;
  Int64 i,j,k,h;

  for(i=2;i<n;i++) {
    j = sa[i-1]; k=sa[i];
    for(h=0;j+h<n && k+h<n;h++)
      if(text[j+h]!=text[k+h]) break;
    if(lcp9[i]!=h) {
      //fprintf(stdout,"FATAL ERROR! Incorrect LCP value: lcp[%d]=%d!=%d\n",i,lcp[i],h);
      fprintf(stdout,"FATAL ERROR! Incorrect LCP value: lcp[%lld]=%lld!=%lld\n",i,lcp9[i],h);
      return;
    }
  }
  fprintf(stdout,"LCP array OK!\n");
}



/* ***************************************************************
   open output files
   *************************************************************** */
void open_output_files(char *outfile_name, FILE **Outfile) {
  
  /* ------ open input and output files ------ */
  if(outfile_name == NULL) { // already checked in main
    fatal_error("Please provide the output file name (open_output_files)\n");
  }
  else {
    *Outfile = fopen( outfile_name, "wb"); // b is for binary: required by DOS
    if(*Outfile == NULL) {
      perror(outfile_name);
      exit(1);
    }
  }
}

/* ***************************************************************
   open input and output files; initializes Outfile, Infile, Infile_size
   *************************************************************** */
void open_files(char *infile_name) {
  // FILE *fopen(const char *path, const char *mode); // ?? 

  /* ------ open input and output files ------ */
  if(infile_name == NULL) {
    fatal_error("Pleaase provide the input file name (open_files)\n");
  }
  else {
    Infile = fopen( infile_name, "rb"); // b is for binary: required by DOS
    if(Infile == NULL) {
      perror(infile_name);
      exit(1);
    }
  }
  // ---- store input file length in Infile_size
  if(fseek(Infile, 0, SEEK_END)!= 0) {
    perror(infile_name); 
	exit(1);
  }
  Infile_size = ftell(Infile);
  if (Infile_size == -1) {
    perror(infile_name); 
	exit(1);
  } 
  if (Infile_size == 0) {
	  fatal_error("Input file empty (open_files)\n");
  }
}

/* ***************************************************************
   displays the text of the fatal error on the stderr
   *************************************************************** */
void fatal_error(char *s) {
  fprintf(stderr, "%s", s);
  exit(1);
}

/* ***************************************************************
   displays the "out of memory" message on the stderr
   *************************************************************** */
void out_of_mem(char *f) {
  fprintf(stderr, "Out of memory in function %s!\n", f);
  exit(1);
}

/* ***************************************************************
   calculates user+system time in seconds
   *************************************************************** */
double getTime ( void ) {
  double usertime,systime;
  struct rusage usage;
  // ?? this block should be rechecked for 64-bit version because of getrusage ******************
  	
  getrusage ( RUSAGE_SELF, &usage );
  usertime = (double)usage.ru_utime.tv_sec + (double)usage.ru_utime.tv_usec / 1000000.0;
  systime = (double)usage.ru_stime.tv_sec + (double)usage.ru_stime.tv_usec / 1000000.0;

  return(usertime + systime);
}

/***** control output - prints the length of the lcp (from the lcp array) and the lcp itself ***/
void printLCP(char *lcpType, FILE *OutfileLCP) {
  
  void printLine(FILE *);

  Int64 pos;
  //Int64 *lcpPrint; // local pointer to the beginning of the array lcp or lcp9, depending on the lcpType
  int *lcpPrint; // local pointer to the beginning of the array lcp or lcp9, depending on the lcpType
  char lcpText[50 + 1]; // a string containing at most first 50 char-s of the lcp starting at the position pos
  int lcpLength; // the length of the lcp starting at the position pos  

  if (strcmp(lcpType, "lcp9") == 0) {
    lcpPrint = (int *)lcp9;
  } 
  else {
    lcpPrint = (int *)lcp;
  }

  fprintf(OutfileLCP, " ============================================= %s ==============================================================\n", lcpType);

  for (pos = 1; pos <= n; pos ++ ) { // write at most the first 50 char-s of the lcp; pos == 1 is undefined ??
    // if lcpLength == 0, then there is no lcp between the suffix at the position pos and his predecessor
    if (pos > 1 && lcpPrint[pos] > 0) {
      lcpLength = lcpPrint[pos] < 50 ? lcpPrint[pos] : 50;
      strncpy(lcpText, (char *)(text + sa[pos]), lcpLength);
      *(lcpText + lcpLength)='\0';
    }
    else { // for lcp[1] which is undefined and when there is no lcp between 2 consequent suffixes
      *lcpText = '\0';
    }
    fprintf(OutfileLCP, "%12d %-50.50s %-50.50s\n", lcpPrint[pos], text + sa[pos], lcpText); // lcp-s are sorted from 1st position to n-th
    printLine(OutfileLCP); // printh the separating line
    if (pos >= 10) {
      // before break, print the last lcp; that is at most 10+1 lcp-s
      if (lcpPrint[n] > 0) {
        lcpLength = lcpPrint[n] < 50 ? lcpPrint[n] : 50;
        strncpy(lcpText, (char *)(text + sa[n]), lcpLength);
        *(lcpText + lcpLength)='\0';
      }
      else {
        *lcpText = '\0';
      }      
      fprintf(OutfileLCP, "\n%12d %-50.50s %-50.50s\n\n", lcpPrint[pos], text + sa[n], lcpText); 
      printLine(OutfileLCP); // printh the separating line
      break; // this is just for testing; break after first 10+1 
     }
  }
}

void printLine(FILE *OutfileLCP) {
  int i;
  // this is done for separating the lines
  for (i = 0; i < 114; i ++) {
    fprintf(OutfileLCP, "-");
  }
  fprintf(OutfileLCP, "\n");
}

/** control output - only for the LCP6 algorithm: prints the length of the lcp (from the lcp array) and the lcp itself **/
//void printLCP6(FILE *OutfileLCP) {
//  
//  void printLine(FILE *);
//
//  Int64 pos;
//  Int64 *lcpPrint; // local pointer to the beginning of the array lcp or lcp9, depending on the lcpType
//
//  lcpPrint = sa;
//  fprintf(OutfileLCP, " ============================================= lcp6 ==============================================================\n");
//  for (pos = 1; pos <= n; pos ++ ) { // write at most the first 50 char-s of the lcp; pos == 1 is undefined ??
//    fprintf(OutfileLCP, "%12lld\n", lcpPrint[pos]); // lcp-s are sorted from 1st position to n-th
//    printLine(OutfileLCP); // printh the separating line
//    if (pos >= 10) {
//      // before break, print the last lcp; that is at most 10+1 lcp-s
//      fprintf(OutfileLCP, "\n%12lld\n\n", lcpPrint[pos]); 
//      printLine(OutfileLCP); // printh the separating line
//      break; // this is just for testing; break after first 10+1 
//     }
//  }
//}
