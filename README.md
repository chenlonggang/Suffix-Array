#Suffix Array
##Where it from
we get the code from here(http://math.cmu.edu/~lleung/project/Alfy_1.5/ExternSrc/DeepShallow64),and change integer type from 
long long to long,which can save space when running on 
32-bit OS and can deal big file when running a 64-bit 
os.
##How to use it
1:include ds_ssort.h in you code  
2:call overloop=init_ds_ssort(500,2000);  
3:call ds_ssort(unsigned char * T,long * sa,long n)  
	where  
    T:an array of length (n+overloop) which in T[0...n-1]  
	  contains the input string.  
	sa:an array of length n  
	n:the length of the text.  
4:it's you duty to new/delete the space for T and sa  

