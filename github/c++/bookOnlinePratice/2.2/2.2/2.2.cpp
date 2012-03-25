// 2.2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int nCnt=0;
	cin>>nCnt;
	char * cSrc=new char[10];
	char * cDst=new char[10];
	for(int i=0;i<nCnt;i++)
	{
		memset(cSrc,0,10);
		memset(cDst,0,10);
		cin>>cSrc;
		cin>>cDst;
		int nX=abs(cSrc[0]-cDst[0]);
		int nY=abs(cSrc[1]-cDst[1]);
		if(nX==0 && nY==0)
		{
			cout<<"0 0 0 0"<<endl;
		}
		else
		{
			if(nX<nY)
			{
				cout<<nY;
			}
			else
			{
				cout<<nX;
			}
			if(nX==nY || nX==0 || nY==0)
			{
				cout<<1;
			}
			else
			{
				cout<<2;
			}
			if(nX==0 || nY==0)
			{
				cout<<1;
			}
			else
			{
				cout<<2;
			}
			if(abs(nX-nY)%2==0)
			{
				if(nY==nX)
				{
					cout<<1;
				}
				else
				{
					cout<<2;
				}
			}
			else
			{
				cout<<"Inf";
			}
		}
		cout<<endl;
	}
	delete[]cSrc;
	delete[]cDst;
	return 0;
}

