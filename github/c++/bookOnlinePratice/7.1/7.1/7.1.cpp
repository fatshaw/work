// 7.1.cpp : 定义控制台应用程序的入口点。
//
/************************************************************************/
/*	big number problem
	store big number in the array 
	and then we can add them bit by bit
*/
/************************************************************************/
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>

using namespace std;
#define  cin fin

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin("d:/test.txt");
	int temp = 0,hasOver=0;
	char num1[210];
	char num2[210];
	memset(num1,0,210);
	memset(num2,0,210);
	cin>>num1;
	cin>>num2;
	int nnum1[210];
	int nnum2[210];
	memset(nnum1,0,sizeof(nnum1));
	memset(nnum2,0,sizeof(nnum2));
	int length1 = strlen(num1);
	int length2=strlen(num2);
	int length = max(length1,length2);
	for (int i = 0 ; i < length1;i++)
	{
		nnum1[i] = num1[i] - '0';
	}

	for (int i = 0 ; i <length2;i++)
	{
		nnum2[i] = num2[i] - '0';
	}

	for(int i = length-1;i>=0;i--)
	{
		temp = 0;
		temp+=hasOver+nnum1[i]+ nnum2[i];
		if(temp >= 10)
		{
			hasOver =1;
			temp -= 10;
		}
		else
		{
			hasOver =0;
		}
		nnum1[i] = temp;
	}
	
	for(int i = 0 ;i<length;i++)
	{
		cout<<nnum1[i];
	}
	cout<<endl;
	system("pause");
	return 0;
}


