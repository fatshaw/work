#ifndef MYCLASS
#define MYCLASS
#include "stdafx.h"

class MyClass
{
private:
	char map[27];
	char telphone[TELENUMBER][TELELENGTH];
	int counter;
	void init();
	static int compare(const void * a ,const void * b){return strcmp((char *)a,(char *)b);}
	void storeTelNumber(char * tel);
public:
	MyClass();
	void toNormalFormat(char * p);
	void sortTel();
	char * getTelPhone(int pos);
	int getCounter();
};




#endif