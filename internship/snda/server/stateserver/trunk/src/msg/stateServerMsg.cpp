#include "ac/log/logdispatcher.h"
#include <ac/log/ostreamlogger.h>
#include <ac/log/log.h>
#include <ac_lib/environment.h>
#include <ac_lib/connectionmanager.h>
#include <ac_actor/scheduler.h>
#include <ac/encoding/xcode.h>
#include <ac/util/buffer.h>
#include <ac/log/log.h>
#include <ac/log/ostreamlogger.h>
#include <ac/util/scope_guard.h>
#include <ac_actor/scheduler.h>
#include <ac_actor/factory.h>
#include <ac_actor/dist.h>
#include <ac_lib/environment.h>
#include <ac_lib/connectionmanager.h>
#include <ac_lib/networkconnection.h>
#include <ac_lib/translator.h>
#include <signal.h>
#include <iostream>
#include <cassert>
#include <ac_actor/actor.h>
#include <ac_actor/factory.h>
#include <ac/encoding/xcode.h>
#include "stateServerMsg.h"
#include "pmsg/tokenMessage.pb.h"

namespace StateServer{

StateServerMessage::StateServerMessage(void):Message(0,ac_lib::CT_INT)
{
}

StateServerMessage::~StateServerMessage(void)
{
}


int StateServerMessage::ProtocolBufEncode(char* buf, size_t len) const
{
	StateServer::TokenMessage tokenMessage;
	tokenMessage.set_seq(seq);
	tokenMessage.set_cmd(cmd);
	tokenMessage.set_apiname(apiname);
	for(ParamList::const_iterator it = paramList.begin(); it != paramList.end(); ++it) 		
	{					
		StateServer::TokenMessage_Paramlist * param = tokenMessage.add_params(); 				
		param->set_key(it->first);				
		param->set_value(it->second); 		
	}

    return (tokenMessage.SerializeToArray(buf,len)) ? (tokenMessage.ByteSize()) : -1 ;
}

int StateServerMessage::ProtocolBufDecode(const char* buf, size_t len)
{
	StateServer::TokenMessage tokenMessage;
	if(!tokenMessage.ParseFromArray(buf, len))
	{
	    return -1;
	}
	seq = tokenMessage.seq();
	cmd =tokenMessage.cmd();
	apiname = tokenMessage.apiname();
	for(int i = 0 ; i < tokenMessage.params_size();++i){	
		paramList.insert(make_pair(tokenMessage.params(i).key(), tokenMessage.params(i).value()));							
	}
	return 0;
}

 std::list<std::string> StateServerMsgFactor::KnownTypes() const
{
	std::list<std::string> result;
	result.push_back("StateServer::StateServerRequest");
	result.push_back("StateServer::StateServerResponse");
	result.push_back("StateServer::StateServerMessage");
    	return result;
}

 ac::coro::Message* StateServerMsgFactor::Create(const char* type)
{
	if(strcmp(type,"StateServer::StateServerRequest") == 0){
            return new StateServer::StateServerRequest;
	}
	else 	if(strcmp(type,"StateServer::StateServerResponse") == 0){
            return new StateServer::StateServerResponse;
	}
	else 	if(strcmp(type,"StateServer::StateServerMessage") == 0){
            return new StateServer::StateServerMessage;
	}
	return NULL;
}
 

void StateServerMsgFactor::Destroy(const ac::coro::Message* m)
{
	delete m;
}
}
