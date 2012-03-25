// 9.4.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>

using namespace  std;
/************************************************************************/
/* 逆波兰式：自左向右输入表达式，遇见运算符则表示运算符后面的两个表示式进行计算。
				运算符后的表达式可能是常量也可能是需要计算的表达式。
	
   递归的思想在于大方向上进行递归，而不是细节。
*/
/************************************************************************/
double exp()
{
	char a[10];
	cin>>a;
	if(a[0]=='+')return exp()+exp();
	if(a[0]=='-')return exp()-exp();
	if (a[0]=='*')
	{
		return exp()*exp();
	}
	if(a[0]=='/')return exp()/exp();
	return atof(a);
}

int _tmain(int argc, _TCHAR* argv[])
{
	double r = exp();
	cout<<r;
	system("pause");
	return 0;
}

