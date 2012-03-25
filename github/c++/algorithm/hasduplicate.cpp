#include <iostream>
#include "testc.h"
using namespace std;

int hasDuplicate(int * a ,int n)
{
	for(int i = 0 ; i < n ; ++i)
	{
		if(a[i] == i || a[i] == -1)continue;
		if(a[i] == a[a[i]])return 1;
		else
		{
			int c = i;
			int r = a[c];
			int t = r;
			int last = a[c];
			a[i] = -1;
			while(1)
			{		
				r = t;
				if(a[r] == r)return 1;
				t = a[r];
				a[r] = last;
				c = r;
				last = t;
				if(t == -1)break;
			}
		}
	}
	return 0;
}


int hasDuplicate2(int * a ,int n)
{
	for(int i = 0;i < n;i++)
	{
		while(a[i]!=-1 && a[i] != i)
		{
				if(a[a[i]] == -1)return 1;
				int t =  a[a[i]];
				a[a[i]] =-1;
				a[i] = t;
			
		}
		a[i] =-1;
	}
	return 0;
}

/*
find duplicate number in the array whose elements are within range [0-n-1]
*/
int hasDuplicate3(int * a ,int n)
{
	int sum = 0;
	for(int i = 0 ; i < n;++i){
		sum += a[i];
	}

	int s_sum = (0+n-1)*(n)/2;
	if(s_sum == sum)return 0;
	return 1;
}

int main()
{
	int a[] = {0,2,1,2};
	cout<<hasDuplicate3(a,4)<<endl;
	for(int i = 0;i<4;i++){
		cout<<"a["<<i<<"] = "<<a[i]<<endl;
	}
	return 0;
}
