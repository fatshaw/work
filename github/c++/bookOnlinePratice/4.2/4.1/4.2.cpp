// 4.1.cpp : 定义控制台应用程序的入口点。
//

#include "MyClass.h"

int _tmain(int argc, _TCHAR* argv[])
{

	int carray[ASIZE];


	int n = 0 , pos = 0;
	char input[XSIZE];
	cin>>n;
	while(n-- > 0)
	{
		for(int i =0 ; i < ASIZE ; i++)
		{
			carray[i] = 0;
		}
		memset(input,0,XSIZE);
		cin>>input;
		MyClass myclass;
		myclass.sumcnumber(input,carray);
		myclass.getMaxChar(carray,&pos);
		//sumcnumber(input,carray);
		//getMaxChar(carray,&pos);
		cout<<(char)pos<<" "<<carray[pos]<<endl;
	}
	system("pause");
	return 0;
}

