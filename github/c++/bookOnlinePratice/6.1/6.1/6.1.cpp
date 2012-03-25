// 6.1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

/*Josephus Problem
do loop for n times
and each time we need count for m times.in the loop the last counted monkey will be out
at the last circle we can find the monkey king.
*/
int _tmain(int argc, _TCHAR* argv[])
{
	int n =0, m = 0,curPos=1;
	while(true)
	{
		cin>>n>>m;
		curPos=1;
		if(n==0 && m==0)break;
		int * outNumber = new int[n+1];
		memset(outNumber,0,sizeof(outNumber));
		for (int j = 1;j<=n;j++)
		{
			for(int i = 0 ;i<m;i++)
			{
				while(outNumber[curPos]==1)
				{
					curPos=(curPos)%n+1;
				}
				if(i<m-1){
					curPos=(curPos)%n+1;
				}
			}
			outNumber[curPos]=1;
		}
		cout<<curPos<<endl;
		delete[]outNumber;
	}
	system("pause");
	return 0;
}

