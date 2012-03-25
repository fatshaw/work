#ifndef _SERVICE_ROUTER_H
#define _SERVICE_ROUTER_H

#include "phpservice_handler.h"
#include "log.h"

using namespace ApiPool;

typedef int (*ServiceHandlerFunc)(Request* request);

typedef map<string, ServiceHandlerFunc> Svc_handler_map;



class ServiceManager
{
public:
	static int registerServiceHandler(const char* seviceName,
										ServiceHandlerFunc handlerFunc);
	
	static int unregisterServiceHandler(const char* seviceName);

	static int handlerRequest(Request* request);

private:
	static Svc_handler_map svc_handler_map;
};



#endif  /* _SERVICE_ROUTER_H */
