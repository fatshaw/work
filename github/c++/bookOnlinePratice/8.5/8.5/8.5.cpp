// 8.5.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include<stdlib.h>
#include <string.h>
using namespace std;
#define  cin fin


int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin("d:/test.txt");
	int n;
	int puzzle[5][6];
	int press[5][6];
	cin>>n;
	while(n>0)
	{
		memset(puzzle,0,sizeof(puzzle));
		memset(press,0,sizeof(press));
		for(int i = 0; i < 5;i++)
		{
			for(int j =0; j < 6;j++)
			{
				cin>>puzzle[i][j];
			}
		}
		int c=0;
		while(true)
		{
			for(int i =0 ; i< 5;i++)
			{
				for(int j =0; j < 6;j++)
				{
					if(i<4){
						if(puzzle[i][j]==1&&press[i][j]==0)
						{
							puzzle[i][j]=0;
							press[i+1][j]=1;
							if(j>0)
							{
								puzzle[i+1][j-1]=puzzle[i+1][j-1]==0?1:0;
							}
							if(j<5){
								puzzle[i+1][j+1]=puzzle[i+1][j+1]==0?1:0;
							}
							puzzle[i+1][j]=puzzle[i+1][j]==0?1:0;
							if(i<3)
							{
								puzzle[i+2][j] = puzzle[i+2][j]==0?1:0;
							}
						}

					}
				}
			}
			press[0][0]++;
			bool success=true;
			for(int i=0;i<6;i++)
			{
				if(puzzle[4][i]==1)
				{
					success = false;
					break;
				}
			}
			if(success==true)
				break;
			c=0;
			while (press[0][c]>1)
			{
				press[0][c]=0;
				c++;
				press[0][c]++;
			}
		}

		for(int i =0;i<5;i++)
		{
			for(int j =0;j<6;j++)
			{
				cout<<puzzle[i][j];
			}
			cout<<endl;
		}
		n--;
	}	
	system("pause");
	return 0;
}

