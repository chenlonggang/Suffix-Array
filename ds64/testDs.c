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

/* --- read proptotypes and typedef for ds_ssort --- */
#include "ds_ssort.h"
#include "common.h" 

// ----- global variables -------
// static int Check_lcp; // for later use of the lcp
static int Verbose;
static FILE *Infile;            // input file 
static FILE *Outfile;           // output file

static Int64 Infile_size;         // size input file;

// ----- prototypes ---------
void fatal_error (char *); // displays the text of the fatal error on the stderr
void out_of_mem (char *f); // displays the "out of memory" message on the stderr
double getTime ( void ); // calculates user+system time in seconds

void open_files(char *infile_name); // opens the input file
void open_output_files(char *outfile_name); // opens the output files

void ds_ssorter(); // new function that does all the sorting


// This was the previous function of the file:
/* ***************************************************************
   Sample computation of the lcp array using the deep/shallow
   suffix sorter and the routines in the bwtlcp.a
   
   This procedure reads the input parameters from the command line
   and calls lcp_file() that does all the testing.
   *************************************************************** */

// This is the CURRENT function of the file
/* ***************************************************************
   This procedure reads the input parameters from the command line
   and calls deep/shallow suffix sorter that does all the testing.
   *************************************************************** */

int main(int argc, char *argv[])
{  
  // double getTime ( void ); // already defined afore the main function
  // void open_files(char *infile_name); // moved afore the main function
  // void lcp_file(void); // moved afore the main function, that is, its counterpart ds_ssorter()
  double end, start;
  char *infile_name;
  char *outfile_name; // outfile --> array of sorted suffixes
  int c;
  extern char *optarg;
  extern int optind, opterr, optopt;

  if(argc < 3) {
    fprintf(stderr, "Usage:\n\t%s infile outfile ",argv[0]);
    // fprintf(stderr,"[-Cv]\n");
	fprintf(stderr,"[-v]\n"); // CURRENT version without the lcp option
    // fprintf(stderr,"\t-C           check lcp array (very slow!)\n");
    fprintf(stderr,"\t-v           verbose output\n\n");
    exit(0);
  }

  /* ------------- read command line options ----------- */
  Verbose = 0;                       // be quiet by default
  // Check_lcp = 0;                     // do not check lcp by default
  infile_name = NULL;
  outfile_name = NULL;
  opterr = 0;
  while ((c = getopt(argc, argv, "v")) != -1) {
    switch (c)
    {
      case 'v':
		Verbose ++; 
		break;
//      case 'C':
//	Check_lcp=1; break;
      case '?':
        fprintf(stderr,"Unknown option: %c -main-\n", optopt);
        exit(1);
    }
  }
  if(optind < argc) {
     infile_name = argv[optind];
     outfile_name = argv[optind + 1]; //
  }

  // ----- check input parameters--------------
  if(infile_name == NULL) fatal_error("The input file name is required\n");
  if(outfile_name == NULL) fatal_error("The output file name is required\n");

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
  open_output_files(outfile_name);

  // lcp_file(); 
  ds_ssorter();
  end = getTime();
  if(Verbose) {
    fprintf(stderr,"Elapsed time: %f seconds.\n", end - start);
  }

  fclose(Infile);
  fclose(Outfile);
  return 0;
}

/* ***************************************************************
   compute the suffix array using deep/shallow sufix sort algorithm
   *************************************************************** */
void ds_ssorter() {

  UChar *text;
  //int *sa, n, overshoot, i, extra_bytes;  // change type for 64-bit version
  Int64 *sa, n, overshoot, i;
  // int extra_bytes;  // for later use --> lcp

  double start, end;
  Int64 pos; // temp test variable 

  // ----- init ds suffix sort routine
  // ?? this block should be rechecked for 64-bit version  because of changing the type of variables ******************
  //overshoot = init_ds_ssort(500, 2000); // defined in globals.c
	overshoot = init_ds_ssort(500000,20000);
  if(overshoot == 0) {
    fatal_error("ds initialization failed! (lcp_file)\n");
  }
  #if 0 // debug
     printf("overshoot = %lld\n", overshoot);
     // printf("sizeof = %llu\n", (long long unsigned) sizeof * sa);

  #endif

  // ----- allocate text and suffix array
  // ?? this block should be rechecked for 64-bit version  because of mem. allocation ******************
  n = Infile_size;                                    // length of input text
  sa = malloc((n + 1) * sizeof * sa);                   // suffix array
  //if (! sa || ! text) {
	 // out_of_mem("ds_ssorter");
  //}
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
       //printf("%lld %s\n", sa[pos], text + sa[pos]); // suffixes are sorted from 0th position to (n-1)
       //printf("%lld %s\n", sa[pos] + 1, text + sa[pos]); // suffixes are sorted from 1st position to n-th
       fprintf(Outfile, "%lld %-50.50s\n", sa[pos] + 1, text + sa[pos]); // suffixes are sorted from 1st position to n-th
       if (pos >= 10) {
         // before break, print the last suffix ; that is at most 10+1 suffixes 
         fprintf(Outfile, "\n\n%lld %-50.50s\n", sa[n] + 1, text + sa[n]); // suffixes are sorted from 1st position to n-th
         break; // this is just for testing; break after first 10+1 
       }
     }
     #endif
  // ?? this block should be rechecked for 64-bit version because of mem. deallocating ******************
  free(text);
  free(sa);

}
 


/* ***************************************************************
   open output files
   *************************************************************** */
void open_output_files(char *outfile_name) {
  // FILE *fopen(const char *path, const char *mode); // ?? 

  /* ------ open input and output files ------ */
  if(outfile_name == NULL) {
    fatal_error("Please provide the output file name (open_output_files)\n");
  }
  else {
    Outfile = fopen( outfile_name, "wb"); // b is for binary: required by DOS
    if(Outfile == NULL) {
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

