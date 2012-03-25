#include "MyClass.h"

void MyClass::sumcnumber(char * p , int * carray)
{
	int length = strlen(p);
	for(int i = 0 ; i < length ;i++)
	{
		carray[p[i]]++;
	}
}

void MyClass::getMaxChar(int * carray , int * pos)
{
	int max = 0;
	for(int i = 0 ; i < ASIZE ; i++)
	{
		if(carray[i] > carray[max])
		{
			max = i;
		}
	}
	*pos = max;
}