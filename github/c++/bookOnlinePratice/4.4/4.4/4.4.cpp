// 4.4.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#define cin fcin
char str[100][101];
int n ,t;

int getSubStr(char * str1)
{
	int substrlen = strlen(str1);
	int sourcestrlen = strlen(str1);
	int i , j;
	bool found = true;
	char substr[101];
	char revstr[101];
	while (substrlen>0)
	{
		for(i=0;i<=sourcestrlen-substrlen;i++)
		{
			strncpy_s(substr,str1+i,substrlen);
			strncpy_s(revstr,str1+i,substrlen);
			substr[substrlen] = revstr[substrlen] = '\0';
			strrev(revstr);
			found =true;
			for(j=0;j<n;j++)
			{
				if(strstr(str[j],substr) == NULL && strstr(str[j],revstr) == NULL)
				{
					found =false;
					break;
				}
			}
			if(found == true)
			{
				return substrlen;
			}
			
		}
		substrlen --;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	fstream fcin;
	fcin.open("c:/test.txt");
	cin>>t;
	int minlength=0;
	char leaststr[101];
	memset(leaststr,0,101);
	int res =0;
	while(t-- >0)
	{
		res =0;
		minlength =100;
		cin>>n;
		for(int i =0;i<100;i++)
		{
			memset(str[i],0,101);
		}
		for(int i =0;i<n;i++)
		{
			cin>>str[i];
			if(minlength > strlen(str[i]))
			{
				strcpy(leaststr,str[i]);
				minlength = strlen(str[i]);
				leaststr[minlength] = '\0';
			}
		}
		 res= getSubStr(leaststr);
		 cout<<res<<endl;

	}
	system("pause");
	return 0;
}

