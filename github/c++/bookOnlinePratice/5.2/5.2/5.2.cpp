// 5.2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#define cin fin

int daysEachMonth[]={31,28,31,30,31,30,31,31,30,31,30,31};



int getDistance(int curMonth,int curDay , int dstMonth,int dstDay)
{
	int days=0;
	for(int i = curMonth;i<dstMonth;i++)
	{
		days+=daysEachMonth[i-1];
	}
	days-=curDay;
	days+=dstDay;

	return days;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int n = 0;
	int curMonth = 0,curDay=0,curNub= 0,dstMonth=0,dstDay=0;
	int distance=0;
	ifstream fin("c:/test.txt");
	cin>>n;
	while(n-->0)
	{
		cin>>curMonth>>curDay>>curNub>>dstMonth>>dstDay;
		distance = getDistance(curMonth,curDay,dstMonth,dstDay);
		cout<<pow((float)2,distance)*curNub<<endl;
	}
	system("pause");
	return 0;
}

