#ifndef _DEFINE_H_
#define _DEFINE_H_
#include <string>
using namespace std;
namespace StateServer{
	
#define GET_TOKEN "GetToken"
#define CHECK_TOKEN "CheckToken"
#define MANAGER  "Manager"
#define MAKE_TOKEN "MakeToken"
#define SET_SESSION  "SetSession"
#define GET_SESSION  "GetSession"

//#define SESSION_CHANGE_TIME 12
#define SESSION_CHANGE_TIME 2
#define SESSION_CLEAN_INTERVAL  30
#define KEY  "JETjGChMsJxLA3Go"
#define CONSISTEN_HASH_WEIGHT 1024
#define EXPIRE_TIME 24*3600*25

#define PROP_SDID "sdid"
#define PROP_SDID_VALUE "value"
#define PROP_TOKEN "token"
#define PROP_LAST_ACT_TIME "lac"
#define PROP_EXPIRED_TIME "et"
#define PROP_SESSION_ID "sessionid"
#define PROP_SESSION_DATA "sessiondata"
#define THREAD_POOL_TIMEOUT 5000
struct SessionValue
{
	time_t timestamp;
	string session_id;
	string session_data;
	SessionValue(time_t  timestamp,string &  session_id,string & session_data)
		:timestamp(timestamp),session_id(session_id),session_data(session_data){}
};

}
#endif

