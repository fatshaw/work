// 6.2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#define cin fin

int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin("d:/test.txt");
	int m,n,k,curX=0,curY=0,maxX=0,maxY=0,max2X=0,max2Y=0,max=0,max2=0,totolPeanuts=0;
	cin>>m>>n>>k;
	int ** matrix = new int*[m+1];
	memset(matrix,0,sizeof(matrix));
	for(int i =1;i<=m;i++)
	{
		matrix[i] = new int[n+1];
		memset(matrix[i],0,sizeof(matrix[i]));
		for(int j =1;j<=n;j++)
		{
			cin>>matrix[i][j];
		}
	}
	while(true)
	{
		if(max2 ==0){
			for(int i=1;i<=m;i++)
			{
				for(int j =1;j<=n;j++)
				{
					if(matrix[i][j]>max)
					{
						max2 = max;
						max2X= maxX;
						max2Y = maxY;

						max = matrix[i][j];
						maxX = i;
						maxY = j;
					}
				}
			}
		}

		if(max == 0 && max2==0)break;

		if(max==0)
		{
			max = max2;
			maxX= max2X;
			maxY = max2Y;
			max2 = 0;
			max2X  = 0;
			max2Y = 0;
		}
		
		if(curX == 0)
		{
			curY = maxY;
		}
		if(abs((curX - maxX)) + abs((curY - maxY)) + 1 + maxX <=k)
		{
			k -=abs(curX - maxX) + abs(curY - maxY) + 1;
			curX = maxX;
			curY = maxY;
			totolPeanuts +=max;
			max =0;
			matrix[maxX][maxY] = 0;
		}
		else
		{
			break;
		}
	}
	cout<<totolPeanuts<<endl;
	system("pause");
	return 0;
}

