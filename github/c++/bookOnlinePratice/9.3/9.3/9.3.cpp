// 9.3.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;
int common(int x, int y)
{
	if(x==y)return x;
	else if(x>y)return common(x/2,y);
	else return common(x,y/2);
}

int _tmain(int argc, _TCHAR* argv[])
{
	int x,y;
	cin>>x>>y;
	int res= common(x,y);
	cout<<res;
	system("pause");
	return 0;
}

