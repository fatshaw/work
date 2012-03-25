#include <pwd.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <string>
#include <memory>
#include <ac/log/log.h>
#include <ac/log/datefilelogger.h>
#include <ac/log/udplogger.h>
#include <ac/log/nulllogger.h>
#include <libconfig/libconfig.h++>
#include <ac_lib/environment.h>
#include <ac_actor/actor.h>
#include "acceptormanager.h"
#include "ginxutil.h"
#include "stateserverservice.h"
#include "msg/stateServerMsg.h"
#include <sys/time.h>
#include "memcachedst.h"
#include "list_map.h"
#include "msganalyser/msgAnalyserUtil.h"

using namespace std;
using namespace ac;
using namespace libconfig;
using namespace StateServer;

 uint64_t getTimeSpan1(struct timeval tv_begin, struct timeval tv_end)
 {	
 	uint64_t timespan = (tv_end.tv_sec - tv_begin.tv_sec)*1000000 + (tv_end.tv_usec  - tv_begin.tv_usec);	
	return timespan;
}

void testMem(char *argv[])
{
	memcached_st  * ps = NULL;
	ps  = MISMemcachedST::init(ps,argv[1]);	
	string key = "hello world";
	string result;
	string value = "hello worldhello world\nhello world\nhello world\nhello world\nhello world\n\n";
	struct timeval begin;
	struct timeval end;
	for(int i = 0 ; i < 10;++i)
	{
		gettimeofday(&begin,NULL);
		MISMemcachedST::set(ps ,key,value,24*3600*10);
		gettimeofday(&end,NULL);
		printf("set USE TIME = %lu\n",getTimeSpan1(begin,end));
	}
	
	for(int i = 0 ; i <  10;i++)
	{
		gettimeofday(&begin,NULL);
		MISMemcachedST::getValue(ps ,key,result);
		gettimeofday(&end,NULL);
		printf("get USE TIME = %lu\n",getTimeSpan1(begin,end));
	}
}


int testListMap()
{
	printf("[TEST LIST MAP BEGIN]");
	ListMap<SessionValue>sessionMap;
	vector<string>randomKey;
	for(int i = 0 ; i < 10;i++)
	{
		randomKey.push_back(getRandStr(10));
	}
	int j = 0 ; 
	for(int i  = 0;i< 100;++i)
	{
		usleep(100000);
		j = rand()%10;
		string sessid = randomKey.at(j);
		uint64_t key =__gnu_cxx::__stl_hash_string(sessid.c_str());
		printf("hash key = %ld\n",key);
		SessionValue * sessionValue = sessionMap.get(key);
		if(sessionValue == NULL)
		{
			sessionValue = new SessionValue(time(NULL),sessid,sessid);
			sessionMap.add_tail(key,sessionValue);
		}
		else
		{
			sessionMap.move_2_tail(key);			
			sessionValue->timestamp = time(NULL);
			sessionValue->session_data = sessid;
		}

	}

	for(ListMap<SessionValue>::iterator it = sessionMap.begin();it!=sessionMap.end();++it)
	{
		SessionValue * sessionValue = sessionMap.get_by_it(it);
		printf("KEY = %s, TIME = %ld\n",sessionValue->session_id.c_str(),(long)sessionValue->timestamp);
		delete sessionValue;
	}
	printf("[TEST LIST MAP END]");
	return 0;
}

int main(int argc, char *argv[])
{
    try {
        if ( argc == 1 ) {
            ShowUsage(argv[0]);
            return 0;
        }  
		Config config;
        InitConfig(argc, argv, config);

        Setting & app = config.getRoot()["App"];

        auto_ptr<Logger> pLogger(CreateLogger(app["Log"]));
        if ( pLogger.get() != NULL ) 
        {
            AC_SET_DEFAULT_LOGGER(pLogger.get());
        }

        if ( (bool)app["RuntimeDebug"] == false ) 
        {
            AC_SET_LOGGER(LP_DEBUG | LP_AC_DEBUG , NullLogger::Instance());
        }

        if ( (bool)app["Daemon"] ) 
        {
            if (daemon(1, 0) != 0)
            {
                AC_ERROR("Change to daemon mode fail: errno=%d errmsg=%s", errno, strerror(errno));
                return -1;
            }
        }

        std::string sUser((const char*)app["User"]);
        if ( !sUser.empty() )
        {
            if (SwitchUser(sUser.c_str()) != 0) 
            {
                return -1;
            }
        }

        signal(SIGPIPE, SIG_IGN);
        signal(SIGINT, HandleExit);
        signal(SIGTERM, HandleExit);

        AC_INFO("initialize environment");
        if (ac_lib::InitializeEnvironment(argc, argv) != 0) 
        {
            AC_ERROR("initialize environment fail");
            return -1;
        }

       	AC_DEBUG("open StateServerService\n");
	StateServerService stateServerService;
	if(stateServerService.Open(app["StateServer"]) !=0)
	{
		AC_ERROR("open StateServerService");
		return -1;
	}
	StateServerMsgFactor factory;
	ac::coro::GetGlobalMsgFactory()->RegisterFactory(&factory);
        AC_INFO("open acceptor manager\n");
        AcceptorManager oAcceptorManager;
        if ( oAcceptorManager.Open(app["NetworkAcceptors"]) != 0 ) 
        {
            AC_ERROR("open acceptor manager fail\n");
            return -1;
        }

        AC_INFO("StateServer  starts running .....");
        ac_lib::RunEnvironment();

        AC_INFO("close acceptor manager");
        oAcceptorManager.Close();

        AC_INFO("close StateServer services");
        stateServerService.Close();

        AC_INFO("close environment");
        ac_lib::UninitializeEnvironment();
        
        AC_INFO("State Server  is stopped completely");    
        return 0;
     }
    catch ( SettingException & e ) 
    {
        printf("ConfigException: %s\n", e.getPath());
    }
    catch ( ParseException & e ) 
    {
        printf("ParseException: %s in line %d\n", e.getError(), e.getLine());
    }
    catch ( FileIOException & e ) 
    {
        printf("FileIOException\n");
    }
    catch ( ConfigException & e ) 
    {
        printf("ConfigException\n");
    }
    catch ( std::exception & e ) 
    {
        printf("%s\n", e.what());
    }

    return -1;
}

