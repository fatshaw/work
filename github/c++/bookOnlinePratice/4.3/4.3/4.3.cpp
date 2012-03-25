// 4.3.cpp : 定义控制台应用程序的入口点。
//

#include "MyClass.h"
#define cin fin
//#define cout fout
int _tmain(int argc, _TCHAR* argv[])
{
	int n = 0 ;
	bool hasDuplicate = false;
	char p[READLINE];

	fstream fin("c:/test.txt");
	cin>>n;
	if(n >TELENUMBER)return 1;

	MyClass myclass ;

	while(n-- > 0)
	{
		memset(p,0,READLINE);
		cin>>p;
		myclass.toNormalFormat(p);
	}
	myclass.sortTel();
	int i = 0 , j =0; 
	n = myclass.getCounter();
	while(i < n)
	{
		j = i;
		i++;
		while(i < n && strcmp(myclass.getTelPhone(i) , myclass.getTelPhone(j)) == 0 )i++;
		if(i - j > 1)
		{
			cout<<myclass.getTelPhone(j)<<" "<<i-j<<endl;
			hasDuplicate = true;
		}
	}
	if(hasDuplicate == false)
	{
		cout<<"no duplicates"<<endl;
	}

	system("pause");
	return 0;
}

