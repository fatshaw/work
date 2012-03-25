#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LEN 75

int compare(const void * q,const void * p)
{
	int a = *(int*)q;
	int b = *(int*)p;
	if(a > b)return 1;
	if(a < b)return -1;
	return 0;
}

void swap(int & a , int & b){
	int t = a;
	a = b;
	b =t ;
}

void changeX(int * a, int p, int r, int x)
{
	for(int i = p;i<=r;++i)
	{
		if(a[i] == x){
			swap(a[i],a[p]);
			break;
		}
	}
}


int partition_l(int * a, int p ,int r)
{
	int x = a[p];
	int i = p;
	int j = p+1;
	for(;j<=r;++j)
	{
		if(a[j]<=x)
		{
			swap(a[i+1],a[j]);
			i+=1;
		}
	}
	swap(a[i],a[p]);
	return i;
}

int partition(int * a, int p,int r,int x)
{
	changeX(a,p,r,x);	
	return partition_l(a,p,r);
}

void insertionsort(int * a ,int n)
{
	int key = 0;
	int j = 0;
	for(int i = 1;i < n;++i){
		key = a[i];
		j = i-1;
		while(j>=0 && a[j]>key){
			a[j+1]=a[j];
			--j;
		}
		a[j+1] = key;
	}
}

int select(int * a, int p,int r, int k)
{
	if((r - p < 75) && (r - p +1) >=k)
	{
		qsort(a+p,r-p+1,sizeof(int),compare);
		return a[p+k-1];
	}
	for(int i = 0 ; i <(r-p+1)/5;++i)
	{
		qsort(a+p+i*5,5,sizeof(int),compare);
		swap(a[p+i],a[p+i*5+2]);
	}
	int x = select(a,p,p+(r-p+1)/5-1,(r-p+1)/10);
	int i = partition(a,p,r,x);
	int j = i - p +1;
	if(j == k)return a[p+k-1];
	if(j > k)return select(a,p,i,k);
	else return select(a,i+1,r,k-j);
}

#if 0 
int select_2(int * a , int p , int r, int k)
{
	if(r - p < 0)
	{
		qsort(a+p,r-p+1,4,compare);
		return a[p+k-1];
	}
	int rnd= rand()%(r-p+1);
	rnd = 0;
	int i = partition(a,p,r,a[p+rnd]);
	int j = i - p + 1;
	if(j == k)return a[p+k-1];
	if(j > k )return select_2(a,p,i-1,k);
	else return select_2(a,i+1,r,k-j);
}
#endif

void printA(int * a, int n ){
	printf("\n");
	for(int i = 0 ; i < n ; ++i){
		printf("%d,",a[i]);
	}
}

int main()
{
	int a[1012];
	srand(time(NULL));
	for(int i = 1; i<=1011;++i)
	{
		a[i] = rand()%2000;
	}
	int sele = select(a,1,1011,505);
	int  k = 0;
	for(int i = 1; i<1012;i++)
	{
		if(a[i]<=sele)++k;
	}
	if(k<500)
	{
		printA(a+1,1011);
	}
	printf("k=%d\n",k);
	return 0;
}
