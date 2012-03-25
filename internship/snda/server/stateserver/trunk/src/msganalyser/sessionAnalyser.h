#ifndef _SESSION_ANALYSER_H_
#define _SESSION_ANALYSER_H_

#include <string>
#include <libconfig/libconfig.h++>
#include "../msg/stateServerMsg.h"
#include <sys/time.h>
#include <assert.h>
#include "msgAnalyserUtil.h"
#include "define.h"
#include "../list_map.h"
#include "memcachedUtil.h"
#include "mongoUtil.h"
#include <ext/hash_fun.h>
using namespace std;
namespace ac_lib
{
    class Message;
}

namespace StateServer{

class SesssionAnalyser
{
public:
	static void Open();
	
	static void SessionClean(time_t curtime);
	
	static int updateSessionMap(string & sessionId, string & sessionData);
	
	static void changeDbName(string & curDbName);
	
	static int getSession( StateServerRequest* reqMessage ,StateServerResponse* respMessage );
	
	static int setSession( StateServerRequest* reqMessage ,StateServerResponse* respMessage );
	
	static int OnKeepAlive( StateServerRequest* reqMessage ,StateServerResponse* respMessage );
private:
	static ListMap<SessionValue>sessionMap;
	static pthread_mutex_t mutex;
	static string m_curDbName;
	static string m_lastDbName;
};
}
#endif

