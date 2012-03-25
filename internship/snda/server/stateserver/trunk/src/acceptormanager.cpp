#include <cstring>
#include <libconfig/libconfig.h++>
#include <ac/log/log.h>
#include <ac/util/stream.h>
#include <ac/network/sockaddr.h>
#include <ac_lib/translator.h>
#include <ac_lib/translator/remotecachetranslator.h>
#include <ac_lib/networkconnection.h>
#include <ac_lib/connectionmanager.h>
#include <ac_actor/actor.h>
#include <ac_actor/coroutine.h>
#include "acceptormanager.h"

int AcceptorManager::parseTranslator(int protocol, const std::string& serviceName, ac_lib::NetworkOption& option)
{
    if (protocol == ac::PROTOCOLBUF_PROTOCOL)
    {
        if (serviceName == "")
        {
            _AC_ERROR("only support PROTOCOLBUF_PROTOCOL serviceName is empty");
            return -1;
        }
        ac_lib::translator::Translator* p = new ac_lib::translator::ProtocolBufTranslator;
        ac_lib::translator::ServiceTranslator* sp = new ac_lib::translator::ServiceTranslator(p);
        sp->SetServiceName(serviceName);
        vTranslators_.push_back(p);
        vTranslators_.push_back(sp);
        option.translator = sp ; 
        return 0;
    }
    else if(protocol == ac::STAND_PROTOCOL)
    {
        option.translator = ac_lib::translator::StandTranslator::Instance();
        return  0;
    }
    else
    {
        _AC_ERROR("only support PROTOCOLBUF_PROTOCOL and ac::STAND_PROTOCOL");
        return -1;
    }
}

int AcceptorManager::Open(libconfig::Setting & AcceptorConfig)
{
    for ( int i=0; i<AcceptorConfig.getLength(); ++i ) 
    {

        AC_INFO("open acceptor %s", (const char*)AcceptorConfig[i]["Address"]);
        
        ac_lib::NetworkOption stNetworkOption;
        stNetworkOption.address = ac::ParseAddress(AcceptorConfig[i]["Address"]);

        if ( AcceptorConfig[i].exists("Protocol") ) 
        {
            stNetworkOption.protocol = ac::ParseProtocol(AcceptorConfig[i]["Protocol"]);
            if(0 != parseTranslator(stNetworkOption.protocol, AcceptorConfig[i]["ServiceName"], stNetworkOption))
            {
                return -1;
            }
        }
        if ( AcceptorConfig[i].exists("Timeout") ) 
        {
            stNetworkOption.timeout = AcceptorConfig[i]["Timeout"];
        }
        if ( AcceptorConfig[i].exists("FirstTimeout") ) 
        {
            stNetworkOption.firstTimeout = AcceptorConfig[i]["FirstTimeout"];
        }
        if ( AcceptorConfig[i].exists("SendBufSize") ) 
        {
            stNetworkOption.sendBufSize = AcceptorConfig[i]["SendBufSize"];
        }
        if ( AcceptorConfig[i].exists("RecvBufSize") ) 
        {
            stNetworkOption.recvBufSize = AcceptorConfig[i]["RecvBufSize"];
        }

       
        if ( AcceptorConfig[i].exists("HeartBeat") ) 
        {
                stNetworkOption.heartbeat = AcceptorConfig[i]["HeartBeat"];
        }
        if ( AcceptorConfig[i].exists("Name") ) 
        {
            stNetworkOption.name = (const char*)AcceptorConfig[i]["Name"];
        }
        
        if ( AcceptorConfig[i].exists("LimitAddresses") ) 
        {
            for ( int j=0; j<AcceptorConfig[i]["LimitAddresses"].getLength(); ++j ) 
            {
                stNetworkOption.limitAddresses.push_back(AcceptorConfig[i]["LimitAddresses"][j]);
            }
        }

        if ( ac_lib::ConnectionManager::Instance()->AddAcceptor(stNetworkOption) != 0 ) 
        {
            AC_ERROR("open acceptor %s failed", (const char*)AcceptorConfig[i]["Address"]);
            return -1;
        }
    }

    return 0;
}

void AcceptorManager::Close()
{
    for ( std::vector<ac_lib::translator::Translator*>::iterator pos=vTranslators_.begin(); pos!=vTranslators_.end(); ++pos) 
    {
        delete *pos;
    }
    vTranslators_.clear();
}

