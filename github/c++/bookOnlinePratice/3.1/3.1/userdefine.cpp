#include "stdafx.h"
#include "userdefine.h"

int changeCharToInt(char c)
{
	if(c >= '0' && c<= '9')
	{
		return c-48;
	}
	return -1;
}

int changeSystem(char * number ,const int t)
{
	int length = (int)strlen(number);
	int sum = 0 ;
	int cur = 0 ;
	number = number + length - 1;
	for(int i = 0; i<length; i++)
	{
		cur = changeCharToInt(*(number--));
		if(cur >= t || cur == -1)return -1;
		sum += cur*pow((float)t,i);
	}
	return sum;
}

int b2ten(char * x , int b)
{
	int length = strlen(x);
	int ret = 0;
	for(int i = 0 ; i < length ; i++)
	{
		if(x[i] -'0' >= b) return -1;
		ret *=b;
		ret += x[i] - '0';
	}
	return ret ;
}