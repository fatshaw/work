#include "sessionAnalyser.h"
#include <ac/log/log.h>
#include <ac/util/stringutil.h>
#include <json/json.h>
#include <libconfig/libconfig.h++>

namespace StateServer
{

	extern int session_clean_interval;
	
	ListMap<SessionValue> SesssionAnalyser::sessionMap;
	pthread_mutex_t SesssionAnalyser::mutex = PTHREAD_MUTEX_INITIALIZER;
	string SesssionAnalyser::m_curDbName;
	string SesssionAnalyser::m_lastDbName;
	
	void SesssionAnalyser::Open()
	{			
		time_t now = time(NULL);
		tm * tm_time = localtime(&now);
		makeDbName(tm_time,m_curDbName);
		AC_DEBUG("[SesssionAnalyser::Open]OPEN MONGODB NAME:%s\n",m_curDbName.c_str());
	}

	void SesssionAnalyser::changeDbName(string & curDbName)
	{
		string deleteDbName = m_lastDbName;
		m_lastDbName = m_curDbName;
		m_curDbName = curDbName;
		AC_DEBUG("[SesssionAnalyser::changeDbName]changeDbName MONGODB NAME,curDbName:%s,lastDbName:%s\n",m_curDbName.c_str(),m_lastDbName.c_str());
	}
	
	int SesssionAnalyser::getSession(StateServerRequest* reqMessage ,StateServerResponse* respMessage)
	{
		string session_id = reqMessage->paramList["session_id"];
		string result;
		string errmsg;
		string errcode;
		string memresult;
		int ret ;
		if(session_id.length()==0)
		{
			ret = -1;
			errmsg = "please input sessid";
		}
		else
		{
			ret = MemcachedUtil::getMemcacheValue(session_id,memresult);
			if(ret == 0)
			{
				AC_DEBUG("[SesssionAnalyser::getSession]MEM GET SESSION ID:%s, SESSION_DATA:%s SUCCESS,\n",session_id.c_str(),memresult.c_str());
			}	
			else
			{
				AC_DEBUG("[SesssionAnalyser::getSession]NO SESSION_ID %s IN MEMCACHE",session_id.c_str());
				AC_DEBUG("[SesssionAnalyser::getSession]GET CURRENT MONGDB:%s\n",m_curDbName.c_str());
				ret = MongoUtil::getMongValue(m_curDbName,session_id,PROP_SESSION_ID,memresult,PROP_SESSION_DATA) ;
				if(ret == 0)
				{
					AC_DEBUG("[SesssionAnalyser::getSession]GET LAST MONGDB:%s\n",m_lastDbName.c_str());
					ret = MongoUtil::getMongValue(m_lastDbName,session_id,PROP_SESSION_ID,memresult,PROP_SESSION_DATA) ;
					if(ret == 0)
					{
						AC_DEBUG("[SesssionAnalyser::getSession]SESSION ID:%s NOT FOUND IN MONGO DB\n",session_id.c_str());	
					}
				}
				else if(ret == -1)
				{
					AC_ERROR("[SesssionAnalyser::getSession]MONGO GET SESSION ID:%s ERROR\n",session_id.c_str());
				}
			}
		}
		respMessage->paramList.insert(make_pair("result",memresult));
		return 0;
	}

	int SesssionAnalyser::updateSessionMap(string & sessionId, string & sessionData)
	{
		uint64_t key =__gnu_cxx::__stl_hash_string(sessionId.c_str());
		time_t updateTime = time(NULL) + session_clean_interval;
		pthread_mutex_lock(&mutex);
		SessionValue * sessionValue = sessionMap.get(key);
		if(sessionValue == NULL)
		{
			sessionValue = new SessionValue(updateTime,sessionId,sessionData);
			sessionMap.add_tail(key,sessionValue);
			AC_DEBUG("[SesssionAnalyser::updateSessionMap]CREATE SESSIONID:%s,SESSION_DATA%s,TIMESTAMP:%d\n",sessionId.c_str(),sessionData.c_str(),updateTime);
		}
		else
		{
			sessionMap.move_2_tail(key);			
			sessionValue->timestamp = updateTime;
			sessionValue->session_data = sessionData;
			AC_DEBUG("[SesssionAnalyser::updateSessionMap]UPDATE SESSIONID:%s,SESSION_DATA%s,TIMESTAMP:%d\n",sessionId.c_str(),sessionData.c_str(),updateTime);
		}
		pthread_mutex_unlock(&mutex);
		return 0;
	}

	int SesssionAnalyser::setSession(StateServerRequest* reqMessage ,StateServerResponse* respMessage)
	{
		string session_id = reqMessage->paramList["session_id"];
		string session_data = reqMessage->paramList["session_data"];
		long expired_time = atol(reqMessage->paramList["expired_time"].c_str());
		string result;
		string errmsg;
		string memresult;
		int ret = 0;
		if(session_id.length()==0)
		{
			ret = -1;
			errmsg = "please input sessid";
		}
		else
		{
			updateSessionMap(session_id,session_data);
			ret = MemcachedUtil::setMemcacheValue(session_id,session_data,(time_t)expired_time);
			if(ret == -1)
			{
				result = "false";
				errmsg = "set memcache value error";
				AC_ERROR("[StateServerMsgAnalyser::setSession]SET MEMVALUE ERROR,sessionid = %s,sessdata = %s\n,expired_time = %ld",session_id.c_str(),session_data.c_str(),expired_time);
			}
			else
			{
				result = "true";
				AC_DEBUG("[StateServerMsgAnalyser::setSession]SET MEMVALUE SUCCESS,SESSIONID:%s,SESSION_DATA:%s,EXPIRED_TIME=%ld",session_id.c_str(),session_data.c_str(),expired_time);
			}
		}
		respMessage->paramList.insert(make_pair("result",result));
		return 0;
	}
	
	void SesssionAnalyser::SessionClean(time_t curtime)
	{
		AC_DEBUG("[SesssionAnalyser::SessionClean]CURRENT TIME:%d\n",curtime);
		AC_DEBUG("[SesssionAnalyser::SessionClean]SESSION MAP SIZE:%d\n",sessionMap.size());
		int deletedCnt = 0;
		pthread_mutex_lock(&mutex);
		for(ListMap<SessionValue>::iterator it = sessionMap.begin();it!=sessionMap.end();)
		{
			SessionValue * sessionValue = sessionMap.get_by_it(it);
			AC_DEBUG("[SesssionAnalyser::SessionClean]SESSION TIMESTAMP:%d\n",sessionValue->timestamp);
			if(sessionValue->timestamp < curtime)
			{
				MongoUtil::setMongValue(m_curDbName,sessionValue->session_id,PROP_SESSION_ID,sessionValue->session_data,PROP_SESSION_DATA);
				AC_DEBUG("[StateServerService::SessionClean] DELETE SESSIONID:%ld,SESSION_DATA%s,timestamp:%ld",sessionValue->session_id.c_str(),sessionValue->session_data.c_str(),sessionValue->timestamp);
				sessionMap.erase(it);
				++deletedCnt;
				delete sessionValue;
			}
			else
			{
				break;
			}
		}
		pthread_mutex_unlock(&mutex);
		AC_DEBUG("[SesssionAnalyser::SessionClean]DELETE SESSION COUNT = %d\n",deletedCnt);
	}

}

