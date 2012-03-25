// 9.8.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;
#define cin fin

int sticks[100];
int used[100];
int compare(const void * a ,const void * b)
{
	return (*(int*)b - *(int *)a);
}
/************************************************************************/
/*	totalSticks:amount of sticks
	unusedSticks:unused Sticks
	left : left len of the stick that is concatenated right now
	len : len of the original stick
*/
/************************************************************************/
bool myRecursion(int totalSticks,int unusedSticks,int left,int len)
{
	if(unusedSticks == 0 && left == 0) return true;
	if(left == 0)left = len;
	for(int i =0 ;i < totalSticks ; i++)
	{
		if(used[i] == 1)
			continue;
		if(sticks[i]>left)
			continue;
		used[i] = 1;
		if(myRecursion(totalSticks,unusedSticks - 1,left - sticks[i], len) == true)return true;
		used[i] = 0;
	}
	return false;
}

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin("d:/test.txt");
	int n;
	while(true)
	{
		cin>>n;
		if(n == 0)break;
		int sum=0;
		memset(sticks,0 ,sizeof(sticks));
		memset(used,0 , sizeof(used));
		for(int i =0 ; i< n ; i++)
		{
			cin>>sticks[i];
			sum+=sticks[i];
		}
		qsort(sticks,n,sizeof(int),compare);
		for(int i = sticks[0];i<sum;i++)
		{
			if(sum%i == 0)
			{
				if(myRecursion(n,n,0,i) == true){
					cout<<i;
					break;
				}
			}
		}
	}
	system("pause");
	return 0;
}

