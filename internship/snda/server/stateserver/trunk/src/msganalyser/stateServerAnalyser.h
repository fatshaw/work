#ifndef _STATE_SERVER_ANALYSER_H_
#define _STATE_SERVER_ANALYSER_H_

#include <string>
#include <libconfig/libconfig.h++>
#include "../msg/stateServerMsg.h"
#include <sys/time.h>
#include "msgAnalyserUtil.h"
#include "memcachedUtil.h"
#include "mongoUtil.h"
#include "define.h"
using namespace std;
namespace ac_lib
{
    class Message;
}

namespace StateServer{

class StateServerMsgAnalyser
{
	private:
		static string tokenDbName;
public:
	static int Open(libconfig::Setting & setting);
	static int makeToken(  StateServerRequest* reqMessage ,StateServerResponse* respMessage );
   	static int getToken(  StateServerRequest* reqMessage ,StateServerResponse* respMessage,bool isCheck);
	static int  storeToken(string & sdid, string & token, string & expired_time) ;
	static int checkToken(StateServerRequest* reqMessage ,StateServerResponse* respMessage );
	static int OnKeepAlive( StateServerRequest* reqMessage ,StateServerResponse* respMessage );
	static int checkMemResult(string & sdid,string & memResult,bool isCheck,string & result,string &  token);
};
}
#endif

