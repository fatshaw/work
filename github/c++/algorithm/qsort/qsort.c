#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
int cnt = 0;
void printA(int * a);
void swap(int & a, int &b)
{
	int t = a;
	a = b;
	b = t;
}
int partition(int * a,int l,int r)
{
	cnt += r-l;
	int pivot = a[l];
	int i = l;
	for(int j = i+1;j<=r;++j){
		if(a[j]<=pivot){
			++i;
			swap(a[j],a[i]);
		}
	}
	swap(a[i],a[l]);
	return i;
}

void qsort(int * a,int l,int r)
{
	int m = 0;
	if(l<r){
		m = partition(a,l,r);
		qsort(a,l,m-1);
		qsort(a,m+1,r);
	}
}

int randomized_partition(int * a,int l,int r)
{
	cnt += r-l;
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

void randomized_qsort(int * a,int l,int r)
{
	int m = 0;
	if(l<r){
		m = randomized_partition(a,l,r);
		randomized_qsort(a,l,m);
		randomized_qsort(a,m+1,r);
	}
}


void printA(int * a)
{
	for(int i = 0; i< 100;++i)
	{
		printf("%d,",a[i]);
	}
	printf("\n");
}

void shuffle(int * a, int n)
{
	int j = 0;
	for(int i = 0 ; i < n ;i++){
		j = rand()%(n-i)+i;
		swap(a[i],a[j]);
	}
}

int main()
{
	int a[100];
	srand(time(NULL));
	for(int i = 0; i<100;++i){
		a[i] = 100-i;
	}
	for(int j= 0 ;j < 5;++j){
		cnt = 0;
		qsort(a,0,99);
		printf("qsort:cnt = %d\n",cnt);
		cnt = 0;
		shuffle(a,100);
		randomized_qsort(a,0,99);
		printf("randomize qsort: cnt = %d\n",cnt);
		printA(a);
	}
	return 0;
}
