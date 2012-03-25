#ifndef _MSGANALYSER_UTIL_H_
#define _MSGANALYSER_UTIL_H_
#include <string>
#include <libconfig/libconfig.h++>
#include "xtea.h"
#include "base64.h"
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include "define.h"
using namespace std;
namespace ac_lib
{
    class Message;
}

namespace StateServer{

	int encrypt(string & sdid, string & site, string & platform,string & token);
	int decrypt(string & encryptedText,string & result);
	uint64_t getTimeSpan(struct timeval tv_begin, struct timeval tv_end);
	string returnPack(int apperrcode , string   errorinfo , string   appresdata, string   debuginfo);
	string getRandStr(int length);
	void makeDbName(tm * tm_time ,string & dbName);
}
#endif

