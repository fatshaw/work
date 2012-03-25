#include "stateServerAnalyser.h"
#include <ac/log/log.h>
#include <ac/util/stringutil.h>
#include <json/json.h>
#include <libconfig/libconfig.h++>
#include <assert.h>

namespace StateServer
{

	string StateServerMsgAnalyser::tokenDbName;
	 int StateServerMsgAnalyser::Open(libconfig::Setting &setting)
	{
		tokenDbName = (const char *)(setting["MongoDbname"]);
		AC_DEBUG("[StateServerMsgAnalyser::Open]TOKEN DB NAME = %s\n",tokenDbName.c_str());
		return 0;
	}
	
	int StateServerMsgAnalyser::getToken(  StateServerRequest* reqMessage ,StateServerResponse* respMessage ,bool isCheck)
	{
		assert(reqMessage);
		string sdid = reqMessage->paramList["sdid"];
		string token; 
		string result;
		string memResult;
		string errmsg;
		if(isCheck)
		{
			token = reqMessage->paramList["token"];
			result = "false";
		}
		if(MemcachedUtil::getMemcacheValue(sdid,memResult)  == 0||MongoUtil::getMongValue(tokenDbName,sdid,PROP_SDID,memResult,PROP_SDID_VALUE) == 0)
		{
			checkMemResult(sdid, memResult,isCheck,result,token);
		}
		else
		{
			AC_ERROR("[StateServerMsgAnalyser::getToken]");
			result  =returnPack(-1,"Service is not Available for now,Please wait","","");
		}
		respMessage->paramList.insert(make_pair("result",result));
		return 0;
	}

	 int StateServerMsgAnalyser::makeToken(StateServerRequest* reqMessage ,StateServerResponse* respMessage )
	{
		assert(reqMessage);
		string expired_time  =  reqMessage->paramList["expired_time"];
		string sdid = reqMessage->paramList["sdid"];
		string site = reqMessage->paramList["site"];
		string platform = reqMessage->paramList["platform"];
		string token;
		string errmsg;
		respMessage->apiname = "returnToken";
		encrypt(sdid,site,platform,token);
		if(storeToken(sdid,token,expired_time) == -1 ){
			token = "";
			errmsg = "STOEN TOKEN ERROR";
		}
		string result = returnPack(0,errmsg,token,"");
		respMessage->paramList.insert(make_pair("result",result));
		return 0;
	}

	 int StateServerMsgAnalyser::storeToken(string & sdid, string & token, string & expired_time) 
	{		
		Json::Value res;
		res[PROP_TOKEN] =token;
		res[PROP_LAST_ACT_TIME] = (uint32_t)time(NULL);
		res[PROP_EXPIRED_TIME] = expired_time;
		Json::FastWriter writer;
		string memvalue = writer.write(res);
		if(MemcachedUtil::setMemcacheValue(sdid,memvalue,EXPIRE_TIME) == 0 || MongoUtil::setMongValue(tokenDbName,sdid,PROP_SDID,memvalue,PROP_SDID_VALUE) == 0){
			AC_DEBUG("[StateServerMsgAnalyser::storeToken]sdid:%s, value:%s memcache store success\n",sdid.c_str(),memvalue.c_str());
		}
		else
		{
			AC_ERROR("[StateServerMsgAnalyser::storeToken]SET TOKEN ERROR,sdid:%s, value:%s\n",sdid.c_str(),memvalue.c_str());
			return -1;
		}
		return 0;
	}
	 
	int StateServerMsgAnalyser::OnKeepAlive( StateServerRequest* reqMessage ,StateServerResponse* respMessage )
	{
		AC_INFO("[StateServerMsgAnalyser::OnKeepAlive]\n");
		respMessage->seq = reqMessage->seq;
		respMessage->cmd  = reqMessage->cmd;
		respMessage->apiname = "KEEP_ALIVE";
		return 1;
	}

	int StateServerMsgAnalyser::checkMemResult(string & sdid ,string & memResult,bool isCheck,string & result,string &  token)
	{
		Json::Reader reader;
		Json::Value json_result;
		reader.parse(memResult,json_result);
		time_t now = time(NULL);
		int ret = 0;
		string memToken = json_result.get(PROP_TOKEN,Json::Value::null).asString();
		if(isCheck == false ||memToken==token)
		{
			long lastacttime = json_result.get(PROP_LAST_ACT_TIME,Json::Value::null).asUInt();
			long expired_time = atol( json_result.get(PROP_EXPIRED_TIME,Json::Value::null).asString().c_str());
			if(lastacttime +  expired_time < now)
			{
				ret = -1;
				AC_DEBUG("[StateServerMsgAnalyser::checkToken] MISMemcachedST sdid:%s token expired,expired_time = %d,lastacttime=%d\n",sdid.c_str(),expired_time,lastacttime);
				result = returnPack(ret,"Your Token has expired , please apply for a new token again",result,"");
			}
			else
			{
				if(isCheck)
				{
					result = returnPack(ret,"","true","");		
				}
				else{
					result = returnPack(ret,"",memToken,"");
				}
				/*decrypt(token,decrptyedText);
				reader.parse(decrptyedText,json_result);
				AC_DEBUG("in token sdid = %s, site = %s,platform = %s\n",json_result.get("sdid",Json::Value::null).asString().c_str(),json_result.get("site",Json::Value::null).asString().c_str(),json_result.get("platform",Json::Value::null).asString().c_str());*/
			}
		}
		else
		{
			ret = -1;
			AC_DEBUG("[StateServerMsgAnalyser::checkToken] MISMemcachedST sdid:%s input a wrong token \n",sdid.c_str());
			result = returnPack(ret,"Your Input Token is not A Correct Token",result,"");
		}
		return ret;
	}
}

