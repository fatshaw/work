// 3.1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "userdefine.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int T = 0 ; 
	int pres  = 0, qres = 0 ,rres = 0 ;
	char p[20];
	char q[20];
	char r[20];
	memset(p,0,20);
	memset(q,0,20);
	memset(r,0,20);

	cin>>T;
	int i = 0;
	while (T--)
	{
		memset(p,0,20);
		memset(q,0,20);
		memset(r,0,20);
		cin>>p>>q>>r;
		for(i = 2; i <= 16 ; i++)
		{
			//if(changeSystem(p,i) * changeSystem(q,i) == changeSystem(r,i))
			pres = b2ten(p,i);
			qres = b2ten(q,i);
			rres = b2ten(r,i);
			if(pres == -1 || qres == -1 || rres == -1)continue;
			if(pres * qres == rres)
			{
				cout<<i<<endl;
				break;
			}
		}
		if(i>16)cout<<0<<endl;
	}
	return 0;
}

