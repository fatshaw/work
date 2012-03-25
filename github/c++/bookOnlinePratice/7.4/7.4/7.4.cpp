// 7.4.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

#define cin fin
void multi(int * res ,int multer,int & length)
{
	int over = 0;
	for(int i=0;i<length;i++)
	{
		res[i]= res[i]*multer+over;
		over = res[i]/10;
	    res[i] = res[i]%10;
	}
	if(over>0){
		res[length] = over;
		length=length+1;

	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin("d:/test.txt");
	int n =0;
	cin>>n;
	int res[10000];
	memset(res,0,sizeof(res));
	res[0] =1;
	int length = 1;
	for(int i =0 ; i < n;i++)
	{
		multi(res,2,length);
	}
	cout<<length<<endl;
	for(int i = 499;i>=0;i--)
	{
		cout<<res[i];
		if(i%50==0)cout<<endl;
	}
	system("pause");
	return 0;
}

