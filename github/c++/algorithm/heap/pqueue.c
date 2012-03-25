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

void insert(int * a,int x,int n)
{
	a[n]=x;
	heapify(a,n+1);	
}

int maximum(int * a)
{
	return a[0];
	}

int extract(int * a,int n)
{
	int temp = a[0];
	swap(a[0],a[n-1]);
	max_heap(a,0,n-1);
	return temp;
}

int main()
{
	int a[100];

	int temp = 0;
	int n = 0;
	srand(time(NULL));
	for(int i = 0 ; i < 10;++i)
	{
		temp = rand()%20;
		insert(a,temp,n++);
	}
	for(int i = 0; i< 10;++i){
		printf("%d,",extract(a,n--));
	}
}
