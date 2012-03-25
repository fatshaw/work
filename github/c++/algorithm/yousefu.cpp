#include <iostream>
#include <fstream>
using namespace std;

//#define fin cin


int y(int n, int m)
{
	int s  = 0 ;
	for(int i = 2;i<=n;++i)
	{
		s=(s+m)%i;
//	cout<<"s="<<s<<endl;
	}
	return s+1;
}

int yy(int n , int m)
{
	if(n == 1)return 1;
	int t = (yy(n-1,m)+m)%n;
	cout<<"t"<<t<<endl;
	return t;
}

int main()
{
	fstream fin("./a",ios::in);
	int n = 0,m = 0 ;
	while(1)
	{
		cin>>n;
		if(n == 0)break;
		for(m =1;;)
		{				
			if(y(n-1,m) == 1)	
			{
				cout<<m<<endl;
				break;
			}	
			m++;
		}
	}
}
