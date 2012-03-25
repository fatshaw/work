#include "mongoUtil.h"
#include <ac/log/log.h>
#include <ac/util/stringutil.h>
#include <json/json.h>
#include <libconfig/libconfig.h++>

namespace StateServer
{

	MongoClient MongoUtil::mongoClient;

	 int MongoUtil::Open(libconfig::Setting &setting)
	{
		AC_DEBUG("[MongoUtil::Open] MongoUtil::Open SUCCESS \n");
		if( mongoClient.connect((const char *)(setting["MongoAddress"])) == -1 )
		{
			AC_ERROR("[MongoUtil::Open] MONGO CLIENT OPEN ERROR\n");
			return -1;
		}
		AC_DEBUG("[MongoUtil::Open]MONGO CLIENT OPEN SUCCESS\n");
		return 0;
	}

	int MongoUtil::getMongValue(string& dbname , string & key,const char *  keyname,string & value,const char *  valuename)
	{
		if(dbname.length()==0)return 0;
		BSONObj obj;
		int ret = 0 ;
		ret = mongoClient.findOne(BSON(keyname<<key),dbname,&obj);
		if(ret  == -1)
		{
			AC_ERROR("[MongoUtil::getMongValue] MONGO CLIENT FINDONE ERROR,ERRORMSG:%s\n",mongoClient.getErrorMsg().c_str());
		}
		else if(ret == 1)
		{
			value = obj.getStringField(valuename);
		}
		return ret;
	}

	int MongoUtil::setMongValue(string & dbname,string & key,const char * keyname,string & value,const char * valuename)
	{
		if(dbname.length()==0)return -1;
		BSONObjBuilder newObj;
		BSONObjBuilder oldObj;
		oldObj.append(keyname,key);
		newObj.append(keyname,key);
		newObj.append(valuename,value);
		int ret = mongoClient.update(oldObj.obj(),newObj.obj(),dbname,true);
		if(ret  == -1)
		{
			AC_ERROR("[MongoUtil::setMongValue] MONGO CLIENT UPDATE DB  ERROR\n");
		}
		return ret;
	}
	 
}

