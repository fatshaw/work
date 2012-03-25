// 3.2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

int ctoskew(char * p)
{
	int length = strlen(p);
	int cur = length;
	int curvalue = 0;
	int sum = 0;
	for(int i = 0;i<length; i++)
	{
		curvalue = (int)(p[i] - '0');
		sum += curvalue * (pow((float)2,cur--) - 1);
	}
	return sum;
}

int _tmain(int argc, _TCHAR* argv[])
{
	char n[20];
	int res = 0;
	while(true)
	{
		memset(n,0,20);
		cin>>n;
		if(n[0] == '0')break;
		res = ctoskew(n);
		cout<<res<<endl;
	}
	return 0;
}

