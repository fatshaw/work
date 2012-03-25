#ifndef _MONGOUTIL_H_
#define _MONGOUTIL_H_

#include <string>
#include <libconfig/libconfig.h++>
#include <sys/time.h>
#include <assert.h>
#include "../mongo.h"
#include "define.h"
using namespace std;
namespace StateServer{
	
	class MongoUtil
	{
		public:
			static int Open(libconfig::Setting &setting);
			static int setMongValue(string & dbname , string & key,const char * keyname,string & value,const char * valuename);
			static int getMongValue(string & dbname , string & key,const char *keyname,string & value,const char * valuename);
		private:
			static MongoClient mongoClient;
	};
}
#endif

