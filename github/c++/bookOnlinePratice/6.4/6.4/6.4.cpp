// 6.4.cpp : 定义控制台应用程序的入口点。
//

/*
排列题：
排列问题是个递归的问题。将n个数的排列分解为2个数的排列。
以此递归处理n个数的排列。
*/
#include "stdafx.h"

#define  cin fin

int compare(const void * e1,const void * e2)
{
	return(*(int*)e1 - *(int*)e2);
}

void swap(int &a ,int &b)
{
	int temp  =a ; 
	a = b;
	b = temp;
}

void printArray(int a[],int length)
{
	for(int i = 0 ; i < length;i++)
	{
		cout<<a[i]<<" ";
	}
	cout<<endl;
}

void perm(int a[],int start,int end,int length)
{
	if(end - start <=1)
	{
		printArray(a,length);
		return ;
	}
 	for(int i = start;i<end;i++)
	{
		swap(a[start],a[i]);
		perm(a,start+1,end,length);
		swap(a[start],a[i]);
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	int a[3]={1,2,3};
	perm(a,0,3,3);
	/*
	ifstream fin("d:/test.txt");
	int m,n,k,cp=0,length=0,isDownSort=0;
	cin>>m;
	for(int i =0 ; i< m;i++)
	{
		cin>>n>>k;
 		int * p = new int[n];
		for(int l = 0;l<n;l++)
		{
			cin>>p[l];
		}
		length = n;
		for(int j =0;j<k;j++)
		{
			isDownSort=0;
			for (int l=0;l<length-1;l++)
			{
				if(p[l] - p[l+1] < 0)
				{
					isDownSort = 1;
					break;
				}
			}
			if(isDownSort == 0)
			{
				for (int l =0; l <length;l++)
				{
					p[l] = l+1;
				}
				continue;
			}

			//find change position
			for(int l=length-1;l>0;l--)
			{
				if(p[l]>p[l-1])
				{
					cp = l;
					break;
				}
			}

			//find index of minimal number  from the right side of the change position found above
			int index = cp;
			for(int l =cp+1;l<length;l++)
			{
				if(p[l] - p[cp-1] > 0)
				{
					if(p[l]<p[index])
					{
						index = l;
					}
				}
			}
			
			//change the number left of the change position and minimal number just found.
			int temp = p[cp-1];
			p[cp-1] = p[index];
			p[index] = temp;

			//sort the array from the right side of change position
			qsort(p+cp,length-cp,sizeof(int),compare);
		}
		for(int l= 0 ; l<length;l++)
		{
			cout<<p[l]<<" ";
		}
		cout<<endl;
		delete[]p;
	}
	*/
	system("pause");
	return 0;
}

