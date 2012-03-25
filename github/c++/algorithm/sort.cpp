#include <iostream>

using namespace std;

void bubble(int * a, int n)
{
	for(int i = 0; i< n;++i)
	{
		for(int j = 0;j<n-i;j++)
		{
			if(a[j]>a[j+1])
			{
				int temp = a[j];
				a[j] = a[j+1];
				a[j+1] = temp;
			}
		}
	}
}

void select(int * a, int n)
{
	int max = 0;
	for(int i = 0;i < n;i++)
	{
		max = 0;
		for(int j = 0; j< n-i;j++)	
		{
			if(a[j] > a[max])
			{
				max = j;
			}
		}
		int t = a[max];	
		a[max] = a[n-i - 1];
		a[n-i - 1] = t;
	}
}

void select2(int * a , int n)
{
	int min= 0;
	for(int i = 0; i< n;i++)
	{
		min = i;
		for(int j = i+1;j<n;j++)
		{
			if(a[j] < a[min])
			{
				min = j;
			}
		}	
		if(min != i)
		{
			int  t = a[min];
			a[min] = a[i];
			a[i] = t;
		}
	}
}

void insertsort(int * a , int n)
{
	for(int i = 1;i < n;i++)
	{
		for(int j = 0;j<i;j++)
		{
			if(a[j]<=a[i])continue;
			int t= a[i];
			for(int k = i;k>j;k--)
			{
				a[k] = a[k-1];
			}
			a[j] = t;
		}
	}
}

void qsort(int * a, int n)
{
	if(n<=1)return;
	int i = 0,j = n-1;
	int pivot = a[0];
	while(i < j)
	{
		while(i<j && a[j] > pivot)j--;
		if(i < j)
		{
			a[i]=a[j];
			i++;
		}

		while(i < j&& a[i] <=pivot)i++;
		if(i<j)
		{
			a[j] = a[i];
		}
	}
	a[i] = pivot;
	qsort(a,i);
	qsort(a+i+1,n-1-i);	
}

#define MAX(a,b) a>b?a:b
#define F(a) a 
int main()
{
	int a[]={1,6,5,4,3};
//	bubble(a,5);
//	select(a,5);
//	select2(a,5);
//	insertsort(a,5);
	qsort(a,5);
	F(1);
	int max = MAX(1,2);
	for(int i = 0;i<5;i++)
	{
		cout<<"a["<<i<<"] = "<<a[i]<<",";
	}
	cout<<endl;
} 
