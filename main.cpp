#include<stdlib.h>
#include<cstdio>
#include<sys/time.h>
#include<iostream>
#include"ds_ssort.h"
using namespace std;
void Getfile(char *filename,unsigned char **Str,unsigned long  *length) ;
int main(){
	unsigned char * T;
	unsigned long  n;
	long  int * sa;
	char * file=(char *)"data";
	Getfile(file,&T,&n);
	sa=new long int[n];
	struct timeval start;
	struct timeval end;
	gettimeofday(&start,NULL);
	ds_ssort(T,sa,n);
	gettimeofday(&end,NULL);
	cout<<"build time "<<(end.tv_sec-start.tv_sec)<<"s"<<endl;
/*	
	for(long i=0;i<n;i++)
		cout<<sa[i]<<endl;	
*/	
	return 0;
}

void Getfile(char *filename,unsigned char **T,unsigned long *length)
{
	FILE *fp=fopen(filename,"r+");
	if(fp==NULL)
	{
		cout<<"Be sure that the file is available"<<endl;
		exit(0);
	}
	fseek(fp , 0, SEEK_END);
    (*length) = ftell(fp);
	long  int overshot=init_ds_ssort(500,2000);
	(*T)=new unsigned char[(*length)+overshot];
	fseek(fp , 0, SEEK_SET);

	long  int e=0;
	long  int num=0;
	while((e=fread((*T)+num,sizeof(unsigned char),(*length)-num,fp))!=0)
		num=num+e;
	if(num!=(*length))
		exit(0);
	fclose(fp);
}
