/*
 * =====================================================================================
 *
 *       Filename:  randomize.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/07/2011 06:12:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>

template<typename T>
void output(T * array,int n)
{
	for(int i = 0 ;i < n;++i)
	{
		std::cout<<"array["<<i<<"] = "<<array[i]<<",";
	}
	std::cout<<std::endl;
}

 
template<typename T>
void swap(T & a ,T & b)
{
	T c = a;
	a = b;
	b = c;
}

template<typename T>
void randomize(T * tarray ,int n)
{
	T * array = new T[n];
	std::copy(tarray,tarray+n,array);
	int i = n - 1;
	for(i;i>=0;--i)
	{
		swap(array[i],array[(rand()%(i+1))]);
	}
	output(array,n);
	delete []array;
}

int main()
{
	srand(time(NULL));
	int array[]={1,2,3,4,5,6,7,8,9,0};
	for(int i = 0 ; i < 10;i++)
	{
		randomize(array,10);		
	}
	return 0;
}
