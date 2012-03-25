#ifndef _STATE_SERVERSERVICE_H__
#define _STATE_SERVERSERVICE_H__

#include <ac/util/refcountptr.h>
#include "msganalyser/stateServerAnalyser.h"
#include "msganalyser/sessionAnalyser.h"
#include "msg/stateServerMsg.h"
#include "msganalyser/msgAnalyserUtil.h"
namespace libconfig
{
    class Setting;
}

namespace ac
{
    namespace coro
    {
        class Message ;
    }
}

namespace StateServer{



class StateServerService
{
public:
public:
    int Open(libconfig::Setting &);

    void Close();

    void Run();
	
	void Manage();
private:
	int Admin( StateServerRequest* reqMessage ,StateServerResponse* respMessage) ;
	int InitConfig(libconfig::Setting &);
	void CleanSession(time_t now);
	int SwitchToParallel();
	int SwitchToSchedule();
	int checkNeedChangeDbName(time_t now,string & curDbName);
	void Process(ac::RefCountPtr<ac::coro::Message> req);
	StateServerResponse *  ProcessMsg(StateServerRequest* pRequest);

	bool HasThreadPool() { return _threadPoolId > 0;}
private :
	uint64_t pulseCnt;
	bool isStop;
	uint64_t requestCntPerSec;
	int _threadPoolId;
	int _threadPoolTimeOut;
};
}
#endif

