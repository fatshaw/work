#include <ac/log/log.h>
#include <ac/util/stringutil.h>
#include <json/json.h>
#include <libconfig/libconfig.h++>
#include "msgAnalyserUtil.h"

namespace StateServer
{
	string getRandStr(int length)
	{
		string randchar =  "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz";
	        length = randchar.length();
	        string token;
		int j = 0;
		int i = 0;
	        for( ; i < 20;i++)
	        {
	            j = rand()%length;
	            token += randchar[j];
	        }
		return token;
	}

	 string returnPack(int apperrcode , string   errorinfo ,string   appresdata, string  debuginfo)
	{
		Json::Value res;
		res["apperrcode"] =apperrcode;
		res["errinfo"] = errorinfo;
		res["appresdata"] = appresdata;
		res["debuginfo"] = debuginfo;
		Json::FastWriter writer;
		string json_result = writer.write(res);
		return json_result;
	}

	 uint64_t getTimeSpan(struct timeval tv_begin, struct timeval tv_end)
	 {	
	 	uint64_t timespan = (tv_end.tv_sec - tv_begin.tv_sec)*1000000 + (tv_end.tv_usec  - tv_begin.tv_usec);	
		return timespan;
	}

	 int encrypt(string & sdid, string & site, string & platform,string & token)
	{
		Json::Value res;
		res["sdid"] =sdid;
		res["site"] = site;
		res["platform"] = platform;
		Json::FastWriter writer;
		string input = writer.write(res);
		int encryptedLength = 0;
		char buf[1024];
		utils::XTEA::encipher(KEY,input.c_str(),input.length(),buf,1024,encryptedLength); 
		utils::Base64::encode(buf,encryptedLength,buf,1024,encryptedLength);
		token = string(buf,encryptedLength);
		return 0;
	}
	 int decrypt(string & encryptedText,string & result)
	{
		char buf[1024];
		int encryptedLength = 0;
		utils::Base64::decode(encryptedText.c_str(),encryptedText.length(),buf,1024,encryptedLength);
		utils::XTEA::decipher(KEY,buf,encryptedLength,buf,1024,encryptedLength);
		result = string(buf,encryptedLength);
		return 0;
	}

	void makeDbName(tm * tm_time,string & dbName)
	{
		char buf[100];
		sprintf(buf,"%d%d%d%d%d%d",tm_time->tm_year+1900,tm_time->tm_mon+1,tm_time->tm_mday,tm_time->tm_hour,tm_time->tm_min,tm_time->tm_sec);
		dbName = buf;
	}
}

