// 7.3.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#define cin fin

int minus(int * num1,int * num2)
{
	int i;
	for(i = 200-1;i>=0;i--)
	{
		if(num1[i] > num2[i])
			break;
		else if(num1[i]  == num2[i])
			continue;
		else if(num1[i] < num2[i])
			return -1;
	}
	for(i = 0; i < 200;i++)
	{
		num1[i] = num1[i] - num2[i];
		if(num1[i] < 0)
		{
			num1[i] += 10;
			num1[i+1]--;
		}
	}
	return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin("d:/test.txt");
	int n,length1=0,length2=0;
	cin>>n;
	char num1[210];
	char num2[210];
	int nnum1[210];
	int nnum2[210];
	int res[210];
	for(int i= 0 ; i < n; i++)
	{
		memset(num1,0,sizeof(num1));
		memset(num2,0,sizeof(num2));
		memset(nnum1,0,sizeof(nnum1));
		memset(nnum2,0,sizeof(nnum2));
		memset(res,0,sizeof(res));
		cin>>num1>>num2;
		length1 = strlen(num1);
		length2 = strlen(num2);
		for(int i = 0 ;i < length1;i++)
		{
			nnum1[i] = num1[length1 - 1- i] - '0';
		}

		for(int i = 0 ;i < length2;i++)
		{
			nnum2[i] = num2[length2 - 1- i] - '0';
		}
		int index = 0;
		while(minus(nnum1,nnum2)==1)
		{
			index=0;
			res[index]++;
			while(res[index]>=10)
			{
				res[index]-=10;
				res[++index]++;
			}
		}
		bool startOutput= false;
		for(int i = 200;i>=0;i--)
		{
			if(startOutput == true)
				cout<<res[i];
			else
			{
				if(res[i] > 0)
				{
					startOutput =true;
					cout<<res[i];
				}
			}
		}
		if(startOutput == false)
			cout<<"0";
		cout<<endl;
	}

	system("pause");
	return 0;
}

