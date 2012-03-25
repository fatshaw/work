// 8.1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
using namespace  std;
#define cin fin
int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin("d:/test.txt");
	int p,e,i,d;
	int res=0;
	while(true)
	{
		res = 0;
		cin>>p>>e>>i>>d;
		if(p==-1 && e==-1 && i==-1 && d==-1)
			break;
		for(int j = d+1;j<=21252;j++)
		{
			if((j-p)%23==0)
			{
				if((j-e)%28==0)
				{
					if((j-i)%33==0)
					{
						res = j-d;
						break;
					}
				}
			}
		}
		cout<<res<<endl;
	}


	return 0;
}

