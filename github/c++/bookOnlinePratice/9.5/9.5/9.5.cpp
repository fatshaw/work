// 9.5.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
using namespace std;
#define  cin fin
int tn,tm;
int cn;
void f(int n,int m,int * r)
{
	if(m<=0 || n<=1){
		r[n]=m;
		for(int i=tn;i>=1;i--)
		{
			cout<<r[i];
		}
		cout<<endl;
		cn++;
		return;
	}
	for(int i = 1; i <=m;i++)
	{
		r[n]=i;
		f(n-1,m-i,r);
	}
}

int count(int x, int y)
{
	if(y==1 || x==0)return 1;
	if(x<y)return count(x,x);
	return count(x,y-1) + count(x-y,y);
}

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin("d:/test.txt");
	int c=0;
	cin>>c;
	while(c-->0)
	{
		cin>>tm>>tn;
		int * t = new int[tn+1];
		memset(t,0,4*(tn+1));
		f(tn,tm,t);
		cout<<cn<<endl;
		cout<<count(tm,tn)<<endl;
	}
	system("pause");
	return 0;
}

