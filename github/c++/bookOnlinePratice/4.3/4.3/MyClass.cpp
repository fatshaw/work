#include "MyClass.h"

void MyClass::storeTelNumber(char * tel)
{
	sprintf_s(telphone[counter++],TELELENGTH,"%s",tel);
}

MyClass::MyClass()
{
	init();
}

void MyClass::init()
{
	for(int i = 0 ; i < TELENUMBER;i++)
	{
		memset(telphone[i],0,TELELENGTH);
	}
	counter = 0 ;

	memset(map,0,27);
	sprintf_s(map,27,"%s","22233344455566677778889999");
}

void MyClass::toNormalFormat(char * p)
{
	int length = strlen(p);
	char temp[TELELENGTH];
	memset(temp,0,TELELENGTH);
	int temppos = 0;
	for(int i =0; i < length ;i++)
	{
		if(temppos == 3){
			temp[temppos++] = '-';
		}

		if(p[i] >= '0' && p[i]<='9'){
			temp[temppos++] = p[i];
		}	
		else if(p[i] == '-' || p[i] == 'Q' || p[i] == 'Z'){}
		else
		{
			temp[temppos++] = map[p[i] - 'A'];
		}
	}
	storeTelNumber(temp);
}

int MyClass::getCounter()
{
	return counter;
}

char * MyClass::getTelPhone(int pos)
{
	return telphone[pos];
}

void MyClass::sortTel()
{
	qsort(telphone,counter,TELELENGTH,MyClass::compare);
}