#ifndef _DEFINE_
#define _DEFINE_

#define FAILURE -1
#define SUCCESS 0


#define ACOUNT_TYPE_CHEQUE "CHEQUE"
#define ACOUNT_TYPE_CODEVI "CODEVI"
#define ACOUNT_TYPE_CEL "CEL"
#define ACOUNT_TYPE_CAPITAL "CAPITAL"

#define ACCOUNT_STATEMENT_MAXCOUNT 5

#include <time.h>
#include <iostream>
using namespace std;

extern char currentTime[20];

inline char * GetCurrentTime()
{
	time_t now = time(NULL);
	tm * tm_time = localtime(&now);
	sprintf(currentTime,"%d/%d/%d",tm_time->tm_mday,tm_time->tm_mon+1,tm_time->tm_year+1900);
	return currentTime;
}

#endif