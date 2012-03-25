// 4.5.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#define cin fcin
char * encrypt(char * str)
{
	char res[100];
	memset(res,0,100);
	char * p=res;
	int length = strlen(str);
	int temp = 0;
	for(int i =0;i<length;i++)
	{
		if(str[i] >='A' && str[i] <='Z')
		{		
			temp =((int)(str[i]-'A') - 5) %26;
			if(temp < 0)temp+=26;
			*p = (char)temp+'A';
			p++;
		}
		else
		{
			*p++ = str[i];
		}
	}
	return res;
}

int _tmain(int argc, _TCHAR* argv[])
{
	char input[100];
	char res[100];
	fstream fcin("c:/test.txt");
	while (true)
	{
		memset(input,0,100);
		memset(res,0,100);
		cin.getline(input,100);
		if(strcmp(input,"ENDOFINPUT") == 0)
		{
			break;
		}
		if(strcmp(input,"START") == 0){
			cin.getline(input,100,'\n');
			//cout<<encrypt(input)<<endl;
			strcpy(res,encrypt(input));
			cout<<res<<endl;
			cin>>input;
			if(strcmp(input,"END")==0)
			{
				continue;
			}
			else
			{
				cout<<"error format"<<endl;
				break;
			}
		}
	}
	system("pause");
	return 0;
}

