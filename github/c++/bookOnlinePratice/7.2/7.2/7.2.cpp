// 7.2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#define  cin fin

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin("d:/test.txt");
	char num1[210];
	char num2[210];
	memset(num1,0,sizeof(num1));
	memset(num2,0,sizeof(num2));
	cin>>num1>>num2;
	int res[210];
	memset(res,0,sizeof(res));
	int length1 = strlen(num1);
	int length2= strlen(num2);
	int nnum1[210];
	int nnum2[210];
	memset(nnum1,0,sizeof(nnum1));
	memset(nnum2,0,sizeof(nnum2));
	for(int i = 0 ;i<length1;i++)
	{
		nnum1[i] = num1[length1-1- i]- '0';
	}

	for(int i = 0 ; i < length2;i++)
	{
		nnum2[i] = num2[length2- 1 - i]- '0';
	}

	for(int i = 0 ; i < length1;i++)
	{
		for(int j = 0; j < length2;j++)
		{
			res[i+j] += nnum1[i]*nnum2[j];
		}
	}
	
	int hasover = 0;
	for(int i = 0 ; i < length1+length2-1;i++)
	{
		res[i]+=hasover;
		hasover = res[i]/10;
		res[i] = res[i]%10;
	}


	if(hasover>0)
	{
		cout<<hasover;
	}

	for(int i =length2+length1-2;i>=0;i--)
	{
		cout<<res[i];
	}

	cout<<endl;
	system("pause");
	return 0;
}

