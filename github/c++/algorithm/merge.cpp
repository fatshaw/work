#include <iostream>
using namespace std;

void swap(int & a, int & b)
{
	int temp = a;
	a=b;
	b=temp;
}

int mergesort(int * a,int l, int r)
{
	if(l>=r)return 0;
	int * temp = new int[r-l+1];

	int m = (l+r)/2;
	int i = l,j = m+1 , k=0,t = 0;

	while(i<=m&&j<=r){
	while(i<=m&&j<=r&&a[i]<=a[j])
	{
		temp[k++]  = a[i++];
	}
	while(i<=m&&j<=r&&a[i]>a[j])
	{
		t++;
		temp[k++] = a[j++];
	}
	}
	if(i<=m){
		temp[k++] = a[i++];
		while(i<=m)
		{
			temp[k++] = a[i++];
			t += r- m;
		}
	}
	while(j<=r)
	{
		temp[k++] = a[j++];
	}

	for(i = l;i<=r;i++)
	{
		a[i] = temp[i-l];
	}
	delete temp;
	return t;
}

int merge(int * a , int l, int r)
{
	if(l>=r)return 0;
	if(r - l ==1)
	{
		if(a[l] > a[r])
		{
			swap(a[l],a[r]);			
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		int lv = merge(a,l,(l+r)/2);
		int rv = merge(a,(l+r)/2+1,r);
		int kv = mergesort(a,l,r);
		return lv+rv+kv;
	}
}


int main()
{
	srand(time(NULL));
	#define MAX 100 
	int a[MAX];//={1,3,2,4,6,5,8,7,0,9};
	for(int i = 0; i < MAX;++i)
	{
		a[i] = rand()%MAX;
	}
	cout<<"reverse order = "<<merge(a,0,MAX-1)<<endl;
	for(int i = 0 ;i < MAX;i++)
	{
		cout<<"a["<<i<<"] = "<< a[i]<<", ";
	}
	cout<<endl;
}
