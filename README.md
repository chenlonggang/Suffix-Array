Suffix-Array
============

lightwight suffix array construction  
we get the ds code from [here](http://math.cmu.edu/~lleung/project/Alfy_1.5/ExternSrc).  
and get the div code from [here](https://code.google.com/p/libdivsufsort/downloads/list?q=label:Featured),for 64-bit OS,click [here](https://code.google.com/p/libdivsufsort/issues/detail?id=1)  
Div is faster than DS.You can find more details in README of div

#How to Build 
##ds
just run make,then ds-ssort.a will be there.
##div
1.delete Makefile and CMakeCache.txt if exist.  
2.run "cmake .",libdivsufsort.so for 32-bit OS will be created by default.  
3.if you want to handle large file(>2GB),run "cmake -DBUILD_DIVSUFSORT64:BOOL=ON ."  
  libdivsufsort64.so will be created for 64-bit OS.  
4.if you want use .a lib,not .so,and "-DBUILD_SHARED_LIBS:BOOL=OFF"  

