// 2.1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int nCnt=0;
	cin>>nCnt;
	for(int i=0;i<nCnt;i++)
	{
		int a=0;
		cin>>a;
		if(a%2!=0)
		{
			cout<<"0 0"<<endl;
		}
		else
		{
			if(a%4==0)
			{
				cout<<a/4<<" "<<a/2<<endl;
			}
			else
			{
				cout<<a/4+1<<" "<<a/2<<endl;
			}
		}
	}
	system("pause");
	return 0;
}

