// 9.7.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#define cin fin
using namespace std;

#define X 8
int M[8];
int P[8];
int n;
int cn=0;
void f(int x)
{

	if(x == X)
	{
		for(int i =0 ; i < 8;i++)	
		{
			for(int j =0;j<8&&j!=i;j++)
			{
				if(abs(i-j) == abs(M[i] - M[j]))
				{
					return;
				}
			}
		}
		cn++;
		if(cn ==n)
		{
			for(int i = 0 ;i < 8 ; i++)
			{
				cout<<M[i]+1;
			}
		}
		return;
	}
	for(int i = 0; i < 8 ;i++)
	{
		if(P[i]==0)
		{
			M[x] = i;
			P[i] = 1;
			f(x+1);
			P[i] = 0;
			if(cn ==n)
				return;
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin("d:/test.txt");
	cin>>n;
 	f(0);
	system("pause");
	return 0;
}

