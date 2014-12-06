#include<stdlib.h>
#include<cstdio>
#include<sys/time.h>
#include"divsufsort64.h"
#include<iostream>
using namespace std;
void Getfile(char *filename,unsigned char **Str,long *length) ;

int main(){

	struct timeval start;
	struct timeval end;

	unsigned char * T;
	long  int n;
	long  int *sa;
	char * file=(char *)"main.cpp";
	Getfile(file,&T,&n);
	sa=new  long int[n];
	gettimeofday(&start,NULL);
	divsufsort64(T, sa, n);
	gettimeofday(&end,NULL);
	cout<<(int)(n/(1024.0*1024))<<" build time "<<(end.tv_sec-start.tv_sec)<<"s"<<endl;

/*	for(int i=0;i<n;i++)
		cout<<sa[i]<<endl;
*/	
	return 0;
}

void Getfile(char *filename,unsigned char **T,long int *length)
{
	FILE *fp=fopen(filename,"r+");
	if(fp==NULL)
	{
		cout<<"Be sure that the file is available"<<endl;
		exit(0);
	}
	fseek(fp , 0, SEEK_END);
	(*length) = ftell(fp);
	(*T)=new unsigned char[(*length)];
	fseek(fp , 0, SEEK_SET);
	long  int e=0;
	long  int num=0;
	while((e=fread((*T)+num,sizeof(unsigned char),(*length)-num,fp))!=0)
		num=num+e;
	if(num!=(*length))
		exit(0);
	fclose(fp);
}
