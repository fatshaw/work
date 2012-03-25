#include "common.h"
#include "service_router.h"

Svc_handler_map ServiceManager::svc_handler_map;

int ServiceManager::handlerRequest(Request* request)
{
	Svc_handler_map::iterator it_func = svc_handler_map.find(request->getServiceName());
	if(it_func == svc_handler_map.end())
	{
		LOG_WARN("CAN'T FIND match handler for %s",
					request->getServiceName().c_str());
		return RET_FAIL;
	}

	ServiceHandlerFunc handlerFunc = it_func->second;
	int ret = handlerFunc(request);
	return ret;
};

int ServiceManager::registerServiceHandler(const char* seviceName,
									ServiceHandlerFunc handlerFunc)
{
	Svc_handler_map::iterator it_func = svc_handler_map.find(seviceName);
	if(it_func != svc_handler_map.end())
	{
		LOG_WARN("handler for %s already exist", seviceName);
		return RET_FAIL;
	}

	svc_handler_map[seviceName] = handlerFunc;

	LOG_INFO("ServiceHandler for %s registered.", seviceName);
	return RET_OK;
}

int ServiceManager::unregisterServiceHandler(const char* seviceName)
{
	Svc_handler_map::iterator it_func = svc_handler_map.find(seviceName);
	if(it_func == svc_handler_map.end())
	{
		LOG_WARN("handler for %s can't find", seviceName);
		return RET_FAIL;
	}

	svc_handler_map.erase(seviceName);

	LOG_INFO("ServiceHandler for %s unregistered.", seviceName);
	return RET_OK;
}


