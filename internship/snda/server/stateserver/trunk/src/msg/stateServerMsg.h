#ifndef STATE_SERVER_MSG_H_
#define STATE_SERVER_MSG_H_

#include <string>
#include <vector>
#include <map>
#include <ac_lib/message.h>
#include <ac_actor/factory.h>

namespace StateServer{

enum STATE_SERVER_CMD
{
	STATE_KEEP_ALIVE= 0,
	STATE_REQUEST = 1,
	STATE_RESPONSE = 2,
};

class StateServerMessage : public ac_lib::Message
{
public:
	typedef std::map< std::string , std::string > ParamList;

    StateServerMessage(void);
    virtual ~StateServerMessage(void);

public:
    virtual int ProtocolBufEncode(char* buf, size_t len) const;
    virtual int ProtocolBufDecode(const char* buf, size_t len);

public:
	uint32_t seq;
	uint32_t cmd;
	std::string apiname;
	ParamList paramList;
};

class StateServerRequest : public StateServerMessage
{
	
};

class StateServerResponse : public StateServerMessage
{
	public:
		StateServerResponse & operator=(const StateServerMessage & stateServerMessage)
		{
			this->seq = stateServerMessage.seq;
			this->cmd = STATE_RESPONSE;
			this->apiname = stateServerMessage.apiname;
			return *this;
		}
};

class StateServerMsgFactor: public ac::coro::MsgFactory
{
	virtual std::list<std::string> KnownTypes() const;
    	virtual ac::coro::Message* Create(const char* type);
    	virtual void Destroy(const ac::coro::Message* m);
};
}

#endif
