CC=gcc
# with profiling & debugging:
#CFLAGS= -O0 -m64 -g -Wall -Wshadow -pedantic -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -std=c99#-mcmodel=medium
# without profiling & debugging:
CFLAGS= -O3 -Wall -Wshadow -pedantic -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -std=c99 -DUNIX #-m64 
#CFLAGS= -g -Wall -Wshadow -pedantic -DSTRMAT -DSTATS
#CFLAGS= -O3 -DSTRMAT -DSTATS

 .PHONY: all
 all : sa  ds_ssort.a bwtlcp.a 

sa:main.cpp ds_ssort.h ds_ssort.a
	g++ main.cpp -o sa ds_ssort.a

#
# The source files, object files, libraries and executable name.
#
#SRCFILES= testDs.c globals.c ds.c shallow.c helped.c deep2.c blind2.c lcp_aux.c bwt_aux.c testLcp.c
#
#OBJFILES= testDs.o globals.o ds.o shallow.o helped.o deep2.o blind2.o lcp_aux.o bwt_aux.o testLcp.o

SRCFILES= testLcp.c globals.c ds.c shallow.c helped.c deep2.c blind2.c lcp_aux.c bwt_aux.c 

OBJFILES= testLcp.o globals.o ds.o shallow.o helped.o deep2.o blind2.o lcp_aux.o bwt_aux.o

LIBS= -lm

#EXECFILE= testDs
#DIRECTORY= TestDs
EXECFILE= testLcp
DIRECTORY= TestLcp
VERSION= 0.4

#
# The make rule for the executable
#
$(EXECFILE) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $(EXECFILE) $(OBJFILES) $(LIBS)

#
# The dependencies for each of the *.o files.
#


# archive containing the ds sort algorithm
ds_ssort.a: globals.o ds.o shallow.o deep2.o helped.o blind2.o
	ar rcs ds_ssort.a globals.o ds.o shallow.o deep2.o helped.o blind2.o

# archive containing the bwt and lcp auxiliary routines 
bwtlcp.a: bwt_aux.o lcp_aux.o
	ar rcs bwtlcp.a bwt_aux.o lcp_aux.o

# compare several linear time lcp algorithms
#testLcp: testLcp.c bwtlcp.a ds_ssort.a 
#	 $(CC) $(CFLAGS) -o testLcp testLcp.c bwtlcp.a ds_ssort.a 


#
# Other Standard make rules
#

lint : 
	lint $(SRCFILES) | more

clean: 
	rm -f *.o *.a testLcp sa

remove: clean
	rm -f $(EXECFILE)


tarfile:
	mkdir $(DIRECTORY)_$(VERSION)
	cp -rf *.c *.h Makefile $(DIRECTORY)_$(VERSION)
	tar cvzfh $(EXECFILE)_$(VERSION).tgz $(DIRECTORY)_$(VERSION)
	mv $(EXECFILE)_$(VERSION).tgz ../
	/bin/rm -r $(DIRECTORY)_$(VERSION)



