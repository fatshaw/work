// 6.3.cpp : 定义控制台应用程序的入口点。
//
/*  1
  ----
 |    |
2|	  |3
 |	  |
 |  4 |
  ----
 |    | 
 |    |
5|    |6
 |  7 |
  ----
  we can use these 7 lines to assemble number 0-9
*/

#include "stdafx.h"
#define  cin fin
/*
	predefine char array so that we can get what we need directly
*/
char n1[11]="- -- -----";
char n2[11]="|   ||| ||";
char n3[11]="|||||  |||";
char n4[11]="  ----- --";
char n5[11]="| |   | | ";
char n6[11]="|| |||||||";
char n7[11]="- -- -- - ";
int _tmain(int argc, _TCHAR* argv[])
{
	ifstream fin("d:/test.txt");
	int s=0,length=0,num=0;
	char n[10];

	while(true){
		memset(n,0,10);
		cin>>s>>n;
		length = strlen(n);
		if(s==0&&length==0)break;
		for(int i=0;i<length;i++)
		{
			num=n[i] - '0';
			cout<<" ";
			for(int j =0;j<s;j++)
			{
				cout<<n1[num];
			}
			cout<<"  ";
		}
		cout<<endl;

		for(int j =0;j<s;j++)
		{
			for(int i=0;i<length;i++)
			{
				num=n[i] - '0';
				cout<<n2[num];
				for(int k=0;k<s;k++)
				{
					cout<<" ";
				}
				cout<<n3[num]<<" ";
			}
			cout<<endl;
		}

		for(int i=0;i<length;i++)
		{
			num=n[i] - '0';
			cout<<" ";
			for(int j =0;j<s;j++)
			{
				cout<<n4[num];
			}
			cout<<"  ";
		}
		cout<<endl;

		for(int j=0;j<s;j++)
		{
			for(int i=0;i<length;i++)
			{
				num=n[i] - '0';
				cout<<n5[num];
				for(int k =0;k<s;k++)
				{
					cout<<" ";
				}
				cout<<n6[num]<<" ";
			}
			cout<<endl;
		}

		for(int i=0;i<length;i++)
		{
			num=n[i] - '0';
			cout<<" ";
			for(int j =0;j<s;j++)
			{
				cout<<n7[num];
			}
			cout<<"  ";
		}
		cout<<endl<<endl;
	}
	system("pause");
	return 0;
}

