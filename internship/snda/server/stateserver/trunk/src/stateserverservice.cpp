#include <libconfig/libconfig.h++>
#include <ac/log/log.h>
#include <ac_actor/actor.h>
#include <ac_actor/scheduler.h>
#include <ac/util/refcountptr.h>
#include <ac_lib/client/service.h>
#include <ac_lib/client/servicemanager.h>
#include "stateserverservice.h"
#include <ac_lib/environment.h>
#include "msganalyser/define.h"

extern string configfile;

namespace StateServer{

int session_clean_interval;
int session_change_time;

int StateServerService::Open(libconfig::Setting &setting)
{
	pulseCnt = 0;
	isStop = 0;
	requestCntPerSec = 0;
	
	if((int)setting["HasThreadPool"]==0)
	{
		_threadPoolId = 0 ;	
	}
	else
	{
		_threadPoolId = 1 ;
	}
	
	if(SPAWN_REGISTER("StateServer", &StateServerService::Run, *this) == ac::coro::Pid::InvalidPid)
	{
		AC_ERROR("[StateServerService::Open] SPAWN REGISTER ERROR\n");
	    	return -1;
	}
	if(SPAWN_REGISTER("manager", &StateServerService::Manage, *this) == ac::coro::Pid::InvalidPid)
	{
		AC_ERROR("[StateServerService::Open] SPAWN REGISTER ERROR\n");
	    	return -1;
	}

	int _threadNum = setting.exists("ThreadNum") ? (int)setting["ThreadNum"]: 20;
    	int _limitQueue = setting.exists("QueueLimit") ? (int)setting["QueueLimit"]: 1000;
   	 if (HasThreadPool())
    	{
       	 	AC_DEBUG("stateserver queuelimit:%d, threadnum:%d, threadpoolid:%d", _limitQueue, _threadNum, _threadPoolId);
        	if( ac::coro::g_scheduler->StartThreadPool(_threadPoolId, _threadNum, _limitQueue) != 0 )
       	 	{
            		AC_ERROR("start thread pool [%d] error", _threadNum);
            		return -1;
        	}
        	AC_DEBUG("start thread pool [%d] succ", _threadNum);
   	 }
	if(InitConfig(setting) == -1)
	{
		AC_ERROR("[StateServerService::Open]init config error\n");
		return -1;
	}
	SesssionAnalyser::Open();
	isStop = false;
	return 0;
}


void StateServerService::Close()
{
	 MemcachedUtil::Close();
}

void StateServerService::Manage()
{
	AC_DEBUG("StateServerService::Manage\n");
	while(isStop == false)
	{	
		ac::coro::MessagePtr m = ac::coro::Receive();
		Process(m);
	}
	ac_lib::StopEnvironment();
	AC_DEBUG("[StateServerService::Manage]STOP RUNNING\n");
}

int StateServerService::Admin( StateServerRequest* reqMessage ,StateServerResponse* respMessage ) 
 {
	char buf[1024];
	string result;
 	if(reqMessage->paramList["cmd"] == "help")
 	{
		sprintf(buf ,"\nstatus\nstop\nreload\n");
 	}
	else if(reqMessage->paramList["cmd"] == "status")
	{
		AC_DEBUG("[StateServerService::Admin]DISPLAY SERVER STATUS\n");
		sprintf(buf,"server is still alive, total pulseCnt = %lu, requestCntPerSec = %lu\n",pulseCnt,requestCntPerSec);
	}
	else if(reqMessage->paramList["cmd"] == "stop")
	{
		AC_DEBUG("[StateServerService::Admin]STOP SERVER\n");
		isStop = true;
		sprintf(buf,"server stop successfully\n");
	}
	else if(reqMessage->paramList["cmd"] == "reload")
	{
		AC_DEBUG("[StateServerService::Admin]RELOAD CONFIGURE \n");
		libconfig::Config  config;
		config.readFile(configfile.c_str());
		libconfig::Setting & app = config.getRoot()["App"];
		if(InitConfig(app["StateServer"]) == 0)
		{
			AC_DEBUG("[StateServerService::Admin]reload configure file successfully \n");				
			sprintf(buf,"reload configure file successfully\n");
		}
		else
		{
			AC_ERROR("[StateServerService::Admin]reload configure file fail\n");				
			sprintf(buf,"reload configure file fail,please try it again\n");
		}
	}
	else
	{
		sprintf(buf ,"\nstatus\nstop\nreload\n");
	}
	result = returnPack(0,"",buf,"");
	respMessage->paramList.insert(make_pair("result",result));
	return 0;
 }

int StateServerService::InitConfig(libconfig::Setting & setting)
{
	_threadPoolTimeOut = (int)setting["ThreadTimeout"];
	session_clean_interval = (int)setting["SessionCleanInterval"];
	session_change_time = (int)setting["SessionChangeTime"];
	 if(MemcachedUtil::Open(setting) == -1 ||	 MongoUtil::Open(setting) == -1
	 	||StateServerMsgAnalyser::Open(setting) == -1)
	 {
	 	return -1;
	 }
	 
	AC_DEBUG("[StateServerService::InitConfig]thread pool time out :%d\n",_threadPoolTimeOut);
	AC_DEBUG("[StateServerService::InitConfig]session_clean_interval :%d\n",session_clean_interval);
	AC_DEBUG("[StateServerService::InitConfig]session_change_time :%d\n",session_change_time);
	return 0;
}

void StateServerService::CleanSession(time_t now)
{
	if(SwitchToParallel() == -1)
	{
		AC_WARNING("[StateServerService::CleanSession]CLEAN SESSION WRONG");
		return;		
	}
	SesssionAnalyser::SessionClean(now);
	AC_DEBUG("[StateServerService::CleanSession]CLEAN SESSION SUCCESS");
	SwitchToSchedule();
}
int StateServerService::checkNeedChangeDbName(time_t now,string & curDbName)
{
	static int changed  = 0;
	int res = 0;
	tm * tm_time = localtime(&now);
	int mode_res = tm_time->tm_min%session_change_time;
	if(mode_res== 0 && changed == 0)
	{
		AC_DEBUG("[StateServerService::checkNeedChangeDbName]MINUTES = %d\n",tm_time->tm_min);
		res = 1;
		changed = 1;
		makeDbName(tm_time,curDbName);
	}
	else if(mode_res != 0)
	{
		changed = 0;
	}	
	return res;
}

void StateServerService::Run()
{
	time_t lasttime = time(NULL);
	time_t curtime =lasttime;
	time_t lastclean = lasttime;
	requestCntPerSec = 0;
	string curDbName;
	while(isStop == false) 
	{
		curtime = time(NULL);
		if(checkNeedChangeDbName(curtime,curDbName))
		{
			SesssionAnalyser::changeDbName(curDbName);
		}
		if(curtime  - lastclean >= 1)
		{
			SPAWN(&StateServerService::CleanSession,*this, curtime);
			lastclean = curtime;
		}
		if(curtime - lasttime >= 1)
		{
			AC_DEBUG("[StateServerService::Run] RECV COUNT = %d\n",requestCntPerSec);
			requestCntPerSec = 0;
			lasttime = curtime;
		}
		ac::coro::MessagePtr m = ac::coro::Receive();
		if (!m) 
		{
		    AC_ERROR("[StateServerService::Run] RECEIVE MSG ERROR");
		    break;
		}
	   	requestCntPerSec++;
		++pulseCnt;
		if(pulseCnt <=0){
			pulseCnt=0;
		}
	    	SPAWN(&StateServerService::Process, *this, m);
	}
	ac_lib::StopEnvironment();
	AC_DEBUG("[StateServerService::Run]STOP RUNNING\n");
}

#define _IF_CMD_(cmd) \
    if ( pRequest->GetTypeName() == #cmd ) \
    { \
    cmd * p = dynamic_cast<cmd *>(pRequest.Get()); \
    if ( !p ) {  return; } \
        else { pResult = ProcessMsg(p); } \
    }
#define  _ELSE_IF_CMD_(cmd) \
    else _IF_CMD_(cmd)

void StateServerService::Process(ac::coro::MessagePtr pRequest)
{
	ac::coro::Message* pResult = NULL;

	_IF_CMD_(StateServer::StateServerRequest)
	else {
		AC_WARNING("CANNOT PROCESS MSG[%s]", pRequest->GetTypeName().c_str());
		return;
	}

	if ( pResult != NULL ) {
		ac::coro::Send(pRequest->sender, pResult);
	}
}

#undef _ELSE_IF_CMD_
#undef  _IF_CMD_

int StateServerService::SwitchToParallel()
{
	if ( HasThreadPool() ) 
	{
		ac::coro::CoSwitchToParallels(_threadPoolId, _threadPoolTimeOut);
		if ( !ac::coro::CoIsRunInParallels() ) 
		{
		    AC_ERROR("GET FAILED,SWITCH TO THREAD POOL ERROR");
		    return -1;
		}
		AC_DEBUG("[StateServerService::SwitchToParallel]CoSwitchToParallels\n");
	}
	return 0;
}

int StateServerService::SwitchToSchedule()
{
	if ( HasThreadPool()) 
	{
		ac::coro::CoSwitchToScheduler();        
		AC_DEBUG("[StateServerService::SwitchToSchedule]CoSwitchToScheduler\n");
	}
	return 0;
}

StateServer::StateServerResponse* StateServerService::ProcessMsg(StateServer::StateServerRequest* req)
{
	assert(req);
	StateServer::StateServerResponse* res = new StateServer::StateServerResponse;
	*res = *req;
	if(SwitchToParallel() == -1)
	{
		res->SetErrno(-1);
		return res;
	}
	AC_DEBUG("[StateServerService::ProcessMsg]PROCESS APINAME:%s,CMD:%d",req->apiname.c_str(),req->cmd);
	switch(req->cmd){
		case STATE_REQUEST:
			res->cmd = STATE_RESPONSE;
			res->seq = req->seq;
			if(req->apiname ==GET_TOKEN)
			{
				StateServerMsgAnalyser::getToken(req,res,false);
			}
			else if(req->apiname ==CHECK_TOKEN)
			{
				StateServerMsgAnalyser::getToken(req,res,true);
			}
			else if(req->apiname == MAKE_TOKEN)
			{
				StateServerMsgAnalyser::makeToken(req,res);
			}
			else if(req->apiname == MANAGER)
			{
				Admin(req,res);
			}
			else if(req->apiname == GET_SESSION)
			{
				SesssionAnalyser::getSession(req,res);
			}
			else if(req->apiname == SET_SESSION)
			{
				SesssionAnalyser::setSession(req,res);
			}
			else
			{
				AC_WARNING("[StateServerService::ProcessMsg] NOT KNOWN APINAME = %s\n",req->apiname.c_str());
			}
			break;
		case STATE_KEEP_ALIVE:
			StateServerMsgAnalyser::OnKeepAlive(req,res);
			break;
	}
	
	SwitchToSchedule();
	AC_INFO("[StateServerService::ProcessMsg] END OF StateServerService::ProcessMsg\n");
	return res;
}


}
