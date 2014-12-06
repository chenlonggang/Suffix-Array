SHELL=/bin/sh

CC=gcc

#these are for testing
#CFLAGS = -g -W -Wall -Winline -O0 -m64 -pg

#these are for maximum speed
CFLAGS=-g -O3 -fomit-frame-pointer -W -Wall -Winline \
       -DDEBUG=0 -DNDEBUG=1 -DUNIX #-m64


.PHONY: all
all : sa

sa:main.cpp ds_ssort.h ds_ssort.a
	g++ main.cpp -o sa ds_ssort.a

# deep-shallow suffix sort algorithm
#ds: suftest2.o ds_ssort.a 
#	$(CC) $(CFLAGS) -o ds suftest2.o ds_ssort.a

# archive containing the ds sort algorithm
ds_ssort.a: globals.o ds.o shallow.o deep2.o helped.o blind2.o
	ar rcs ds_ssort.a globals.o ds.o shallow.o deep2.o helped.o blind2.o

# archive containing the bwt and lcp auxiliary routines 
bwtlcp.a: bwt_aux.o lcp_aux.o
	ar rcs bwtlcp.a bwt_aux.o lcp_aux.o

# compare several linear time lcp algorithms
#testlcp: testlcp.c bwtlcp.a ds_ssort.a 
#	 $(CC) $(CFLAGS) -o testlcp testlcp.c bwtlcp.a ds_ssort.a 

# inverse bwt
#unbwt: unbwt.c
#	 $(CC) $(CFLAGS) -o unbwt unbwt.c 

# bwt using ds_ssort
#bwt: bwt.c ds_ssort.a
#	 $(CC) $(CFLAGS) -o bwt bwt.c ds_ssort.a 

# pattern rule for all objects files
%.o: %.c %.h
	$(CC) -c $(CFLAGS) $< -o $@

clean: 
	rm -f *.o *.a sa

remove: clean
	rm -f ~*
