#include <stdio.h>
#include <string.h>
#include <stdlib.h>

template<typename T>
void swap(T & a,T& b)
{
	T c = a;
	a = b;
	b = c;
}


/*
	partitions the array around the pivot. 

	elements in the the left part are all less than or equal to pivot while those in the right part are all greater than or equal to pivot.

	in this version of partition function, it can gurantee that the returned index is the index of pivot in the array.
	so if we want to use this version we should care about the returned index value and modify the qsort function to adapt it.
	
	@return the index of the most left element that is less than or equal to pivot.
*/

template<typename T>
int partition(T * array,int l,int r)
{
	if(l>=r)return l;
	int i = l;
	int j = r;
	int m = (l+r)/2;
	T pivot = array[m];

	while(i < j){
		while(i<j && array[i]<pivot){
			i++;
		}
		while(i<j && array[j]>pivot){
			--j;
		}

		if(i<j){
			swap(array[i],array[j]);
			++i;
			--j;		
		}
	}
	if(i == j && array[j]>pivot)
	{
		--j;
	}
	return j;
}


/*	partition function
	
	divide the array into three part. 

	The elements in the left part are all less than or equal to pivot

	The elements in the right part are all greater than or equal to pivot
	
	pivot is always in the middle part.

	@return the index of pivot which aparts the whole array.
	
*/

template<typename T>
int partition_l(T * array,int l,int r)
{
	if(l>=r)return l;
	int i = l+1;
	int j = r;
	T pivot = array[l];

	while(i < j){
		while(i<j && array[i]<pivot){
			i++;
		}
		while(i<j && array[j]>pivot){
			--j;
		}	
		if(i<j){
			swap(array[i],array[j]);
			++i;
			--j;		
		}
	}
	if(i == j && array[j]>pivot)
	{
		--j;
	}
	array[l]=array[j];
	array[j] = pivot;
	return j;
}

template<typename T>
void output(T * array,int len)
{
	printf("output\n");
	for(int i = 0; i<=len;++i){
		printf("%d\n",array[i]);
	}
}

template<typename T>
int randomized_partition(T * a,int l,int r)
{
	int i = rand()%(r-l+1);
	swap(a[l],a[i+l]);
	int pivot = a[l];
	i=l;
	int j = r;
	while(1)
	{
		while(a[j]>pivot)--j;
		while(a[i]<pivot)++i;
		if(i<j){
			swap(a[i++],a[j--]);
		}
		else{
			return j;
		}
	}
}

template<typename T>
void randomized_qsort(T * a,int l,int r)
{
	int m = 0;
	if(l<r){
		m = randomized_partition(a,l,r);
		randomized_qsort(a,l,m);
		randomized_qsort(a,m+1,r);
	}
}

template<typename T>
void qsort(T * array,int l,int r)
{
	if(l>=r)return;
	int p = partition(array,l,r);
	qsort(array,l,p);
	qsort(array,p+1,r);
}

template<typename T>
void qsort_l(T * array,int l,int r)
{
	if(l>=r)return;
	int p = partition_l(array,l,r);
	qsort(array,l,p-1);
	qsort(array,p+1,r);
}


/*
find the kthSmallestElement in the array

in this function both version of partition function is ok

first step: partitions the array into two parts.

second step: count the number of elements in the left part

third step: if the number of elements in the left part is less than k, it means the kthSmallestElement is in the right part of the array.
	    if the number of elements in the left part is greater than or equal to k, it means the kthSmallestElement is in the left part.
	
*/
template<typename T>
T kmedian(T * array,int l, int r, int k)
{
	if(l>=r)return array[l];
	int p = partition(array,l,r);
	int i = p-l+1;//count of elements in the left part.
	if(i<k)return kmedian(array,p+1,r,k-i);
	return kmedian(array,l,p,k);
}


template<typename T>
T majorElement(T * array,int n)
{
	T ret = kmedian(array,0,n-1,n/2);
	return ret;
}


template<typename T>
T mainElement(T * array, int n)
{
	T seed = array[0];
	int count = 1;
	for(int i = 1;i<n;++i)
	{
		if(seed == array[i])
		{
			count++;
		}
		else{
			if(count > 0)count--;
			else{
				seed= array[i];
			}
		}
	}

	count=0;
	for(int i = 0; i < n;++i)
	{
		if(array[i]==seed)++count;
	}
	
	if(count >=n/2)return seed;

	return -1;
}

template<typename T>
T mode(T * array,int l,int r,int n)
{
	if(l>=r)return array[l];
	int i = l;
	int j = r;
	int m = (l+r)/2;
	int c = 0;
	T pivot = array[m];

	while(i < j){
		while(i<j && array[i]<pivot){
			i++;
		}
		while(i<j && array[j]>pivot){
			--j;
		}
		if(i<j){
			if(array[i]==pivot)++c;
			if(array[j]==pivot)++c;
			swap(array[i],array[j]);
			++i;
			--j;		
		}
	}
	if(i == j && array[j] == pivot)++c;
	if(i == j && array[j]>pivot)
	{
		--j;
	}
	if(c >= n/2)return pivot;
	if(j-l+1>=n/2)return mode(array,l,j,n);
	return mode(array,j+1,r,n);
}


int main()
{
/*	int a[]={3,2,2,1,3};
	int ret = kmedian(a,0,4,1);
	printf("%d\n",ret);*/
/*	int b[] = {1,2,4,3,12,3,4,6,7,8};
	qsort(b,0,9);
	for(int i = 0; i <10;++i)
	{
		printf("%d,",b[i]);
	}*/

	int b[]={1,2,2,1,1,2,2,2,2,2,2};
//	printf("%d",majorElement(b,8));
printf("%d\n",mainElement(b,11));
	printf("\n");
	return 0;
}
