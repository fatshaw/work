// 9.6.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#define  cin fin
using namespace std;
char matrix[100][100];
int trace[100][100];

void count(int x,int y,int & cn)
{
	bool moved = false;
	if(matrix[x][y-1]=='.')
	{
		if(trace[x][y-1]==0)
		{
			trace[x][y-1]=1;
			cn++;
			count(x,y-1,cn);
			moved =true;
		}
	}

	if(matrix[x][y+1]=='.')
	{
		if(trace[x][y+1]==0)
		{
			trace[x][y+1]=1;
			cn++;
			count(x,y+1,cn);
			moved =true;
		}
	}

	if(matrix[x-1][y]=='.')
	{
		if(trace[x-1][y]==0)
		{
			trace[x-1][y]=1;
			cn++;
			count(x-1,y,cn);
			moved =true;
		}
	}

	if(matrix[x+1][y]=='.')
	{
		if(trace[x+1][y]==0)
		{
			trace[x+1][y]=1;
			cn++;
			count(x+1,y,cn);
			moved =true;
		}
	}
	if(moved == false)
		return;
}


int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin("d:/test.txt");
	int n,m,cn=1;
	cin>>n>>m;
	memset(matrix,0,sizeof(matrix));
	memset(trace,0,sizeof(trace));
	int x=0,y=0;
	for(int i =1 ;i <=m;i++)
	{
		for(int j =1;j<=n;j++)
		{
			cin>>matrix[i][j];
			if(matrix[i][j]=='@')
			{
				x=i;y=j;
			}
		}
	}
	count(x,y,cn);
	cout<<cn<<endl;
	system("pause");
	return 0;
}

