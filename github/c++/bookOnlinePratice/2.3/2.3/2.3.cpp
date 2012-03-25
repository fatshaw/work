// 2.3.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int nL;
	int nM;
	int nSrc=0;
	int nDst=0;
	int nCnt=0;
	cin>>nL;
	cin>>nM;
	int* pL=new int[nL];
	for(int i=0;i<nL;i++)
		pL[i]=1;
	for(int i=0;i<nM;i++)
	{
		cin>>nSrc>>nDst;
		if(nSrc>nDst)continue;
		else if(nDst>=nL)continue;
		for(int j=nSrc;j<=nDst;j++)
		{
			pL[j]=0;
		}
	}
	for(int i=0;i<nL;i++)
	{
		if(pL[i]==1)nCnt++;
	}
	cout<<nCnt<<endl;
	system("pause");
	return 0;

}

