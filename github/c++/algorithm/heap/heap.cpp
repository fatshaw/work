#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void swap(int & a, int & b){
	int t= a;
	a = b;
	b = t;
}

void max_heap(int * a,int i, int n)
{
	int left = i*2+1;
	int right = i*2+2;
	int temp = 0;
	while(i<n)
	{
		left= i*2+1;
		right = i*2+2;
		temp = i;
		if(left < n&&a[i]<a[left])i = left;
		if(right < n && a[i]<a[right])i = right;
		if(temp != i){
			swap(a[temp],a[i]);
		}
		else{
			break;
		}
	}
}

void heapify(int * a ,int n)
{
	for(int i = n/2-1;i>=0;--i){
		max_heap(a,i,n);
	}
}

void heapsort(int * a, int n)
{
	heapify(a,n);
	for(int i = 1 ; i<n ; i++)
	{
		swap(a[0],a[n-i]);
		max_heap(a,0,n-i);
	}
}
void printA(int * a, int n)
{
	for(int i = 0 ;i < n ; ++i)
	{
		printf("%d,",a[i]);
	}
	printf("\n");
}

int main()
{
	int a[10];

	srand(time(NULL));
	for(int i = 0 ; i < 10;++i)
	{
		a[i] = rand()%20;
	}
	heapsort(a,10);
	printA(a,10);
}
