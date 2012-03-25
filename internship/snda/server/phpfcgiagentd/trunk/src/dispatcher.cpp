#include <errno.h>
#include <rwlock.h>
#include "log.h"

#include "configure.h"

#include "net_manager/src/net_manager.h"
#include "net_manager/src/binary_splitter.h"

#include "definition.h"
#include "common.h"
#include "protocol.h"
#include "web_worker.h"
#include "connectd_worker.h"

#include "dispatcher.h"
#include "admin_handler.h"

using namespace utils::thread;

extern int errno;
extern utils::Configure configure;
extern Request_queue connetcd_request_queue;
extern Request_queue web_request_queue;


#define	MAX_APP_LISTEN	2			//最大监听端口数

Binary_Splitter m_packet_splitter_string;

//到外部业务系统APP的长连接与在MiddleAgent中对应Net_info的Map表
//对net_map_connectd,net_map_web的操作不需要加锁，因为只在dispatcher线程中进行操作
NetMap		net_map_connectd;	//---这里只保存需要超时检查的长连接 
//不再维护web的短连接集合，依赖目的ip+port判断合法性
NetMap		net_map_web;		//---这里保存从web到达的短连接

//ReadWriteLock rwlock_net_map_web;
//ReadWriteLock rwlock_net_map_connectd;

NetInfo net_connectd;
NetInfo net_web;
NetInfo admin_connected;

//负责网络IO
Net_Manager *net_manager;
Thread_Mutex event_put_mutex;


int Dispatcher::web_worker_num = 0;
int Dispatcher::connectd_worker_num = 0;

char fpmip[100];
uint16_t fpmport;

int shorttcp_timeout;
int longtcp_timeout;

int loadCfg()
{
	int rc = 0;
	//int i = 0;
	memset(&net_connectd, 0x0, sizeof(NetInfo) * MAX_APP_LISTEN);


	/*************************************/
	//从配置文件读取netinfo
	/*************************************/
	string val_string;

	/*********************************************************************/
	/**********************读取监听端口信息**********************/
	/*********************************************************************/

	//读取长连接端口的监听信息
	
	rc = configure.get_single("Service", "ListenIp_Long", val_string);
	if (0 != rc) 
	{
		LOG_ERROR("[Dispatcher][loadCfg]Configure error, get Service ListenIp conf error rc=%d\n", rc);
		return -1;
	}
	strncpy(net_connectd.ip, val_string.c_str(), sizeof(net_connectd.ip)-1);
	LOG_INFO("[Dispatcher][loadCfg]ListenIp_Long:%s", net_connectd.ip);

	rc = configure.get_single("Service", "ListenPort_Long", val_string);
	if (0 != rc) 
	{
		LOG_ERROR("[Dispatcher][loadCfg]Configure error, get Service ListenPort_Long conf error\n");
		return -1;
	}
	net_connectd.port = atoi(val_string.c_str()); 
	LOG_INFO("[Dispatcher][loadCfg]ListenPort_Long:%d", net_connectd.port);


	//读取短连接端口的监听信息
	rc = configure.get_single("Service", "ListenIp_Short", val_string);
	if (0 != rc) 
	{
		LOG_ERROR("[Dispatcher][loadCfg]Configure error, get Service ListenIp_Short conf error rc=%d\n", rc);
		return -1;
	}
	strncpy(net_web.ip, val_string.c_str(), sizeof(net_web.ip)-1);
	LOG_INFO("[Dispatcher][loadCfg]ListenIp_Short:%s", net_web.ip);

	rc = configure.get_single("Service", "ListenPort_Short", val_string);
	if (0 != rc) 
	{
		LOG_ERROR("[Dispatcher][loadCfg]Configure error, get Service ListenPort_Short conf error\n");
		return -1;
	}
	net_web.port = atoi(val_string.c_str()); 
	LOG_INFO("[Dispatcher][loadCfg]ListenPort_Short:%d", net_web.port);


	/**** 读取服务器主配置****/
   	 long long_var = 0;
   	 if (configure.get_single("Main", "web_worker_num", long_var) != 0)
  	  {
		LOG_ERROR("[Dispatcher][loadCfg]Configure error, get web_worker_num conf error\n");
		return -1;
   	 }
	Dispatcher::web_worker_num = (uint32_t)long_var;
	LOG_INFO("[Dispatcher][loadCfg]Configure, get connectd_worker_num=%d\n",
				Dispatcher::web_worker_num);
	
    	if (configure.get_single("Main", "connectd_worker_num", long_var) != 0)
    	{
		LOG_ERROR("[Dispatcher][loadCfg]Configure error, get connectd_worker_num conf error\n");
		return -1;
    	}
	Dispatcher::connectd_worker_num = (uint32_t)long_var;
	LOG_INFO("[Dispatcher][loadCfg]Configure, get connectd_worker_num=%d\n",
				Dispatcher::connectd_worker_num);

	/*read sock timeout configure*/
	if (configure.get_single("Main", "sock_timeout", long_var) != 0)
    	{
		LOG_ERROR("[Dispatcher][loadCfg]Configure error, get sock_timeout conf error\n");
		return -1;
    	}
	sock_timeout = (uint32_t)long_var;
	LOG_INFO("[Dispatcher][loadCfg]Configure, get sock_timeout=%d\n",
				sock_timeout);
	timeout.tv_sec = 0;
	timeout.tv_usec = sock_timeout;

	if (configure.get_single("Main", "longtcp_timeout", long_var) != 0)
    	{
		LOG_ERROR("[Dispatcher][loadCfg]Configure error, get longtcp_timeout conf error\n");
		return -1;
    	}

	longtcp_timeout = long_var;
	LOG_INFO("[Dispatcher][loadCfg]Configure, get longtcp_timeout=%d\n",longtcp_timeout);
	
	if (configure.get_single("Main", "shorttcp_timeout", long_var) != 0)
    	{
		LOG_ERROR("[Dispatcher][loadCfg]Configure error, get shorttcp_timeout conf error\n");
		return -1;
    	}

	shorttcp_timeout = long_var;
	LOG_INFO("[Dispatcher][loadCfg]Configure, get shorttcp_timeout=%d\n",shorttcp_timeout);
	
	/*read fpm config*/
	rc = configure.get_single("Fpm", "isUnixSocket", val_string);
	if (0 != rc) 
	{
		LOG_ERROR("[Dispatcher][loadCfg]Configure error, get fpm port conf error\n");
		return -1;
	}
	isUnixSocket = (uint8_t)atoi(val_string.c_str());
	LOG_INFO("[Dispatcher][loadCfg]Configure, get isUnixSocket =%d\n",
				isUnixSocket);

	if(isUnixSocket == 1){
		rc = configure.get_single("Fpm", "unixSockPath", val_string);
		if (0 != rc) 
		{
			LOG_ERROR("[Dispatcher][loadCfg]Configure error, get fpm port conf error\n");
			return -1;
		}
		strncpy(unixSockPath,val_string.c_str(),val_string.length());
		LOG_INFO("[Dispatcher][loadCfg]Configure, get unixSockPath =%s\n",
				unixSockPath);
		memset(&fpm_unix_addr,0,sizeof(struct sockaddr_un));
		fpm_unix_addr.sun_family = AF_UNIX;
		strcpy(fpm_unix_addr.sun_path , unixSockPath);

	}else if(isUnixSocket == 0){
		rc = configure.get_single("Fpm", "fpm_ip", val_string);
		if (0 != rc) 
		{
			LOG_ERROR("[Dispatcher][loadCfg]Configure error, get Fpm ip conf error rc=%d\n", rc);
			return -1;
		}
		strncpy(fpmip, val_string.c_str(), val_string.length());
		LOG_INFO("[Dispatcher][loadCfg]Configure, get fpm ip address=%s\n",
					fpmip);
		
		rc = configure.get_single("Fpm", "fpm_port", val_string);
		if (0 != rc) 
		{
			LOG_ERROR("[Dispatcher][loadCfg]Configure error, get fpm port conf error\n");
			return -1;
		}
		fpmport= atoi(val_string.c_str()); 
		LOG_INFO("[Dispatcher][loadCfg]Configure, get fpm port =%d\n",
				fpmport);
		
		memset(&fpm_addr,0,sizeof(struct sockaddr_in));
		fpm_addr.sin_family = AF_INET;
		fpm_addr.sin_port = htons(fpmport);
		fpm_addr.sin_addr.s_addr= inet_addr(fpmip);
	}

	return RET_OK;
}


int init_net_manager()
{
	//int i = 0;
	/*************************************/
	//初始化指定端口的监听
	/*************************************/
	/*net_manager = new Net_Manager();
	ret = net_manager->start();
	if(RET_OK != ret)
	{
		LOG_ERROR("[Dispatcher][init_net_manager] net_manager_app start error ret=%d", ret);
		return RET_NET_MANAGER_START_FAIL;
	}
	sleep(1);	//Wait net_manager to complete init work.
	LOG_INFO("[Dispatcher][init_net_manager] net_manager_app start success");
	*/
	net_connectd.net_id = net_manager->create_tcp_server(net_connectd.ip, net_connectd.port, &m_packet_splitter_string, 
				Dispatcher::NetEventHandler, Dispatcher::NetEventHandler);

	if (0 == net_connectd.net_id) 
	{			// 创建网络通道失败
		LOG_ERROR("[Dispatcher][init_net_manager] net_manager_app::create_tcp_server error");
		return RET_NET_LISTEN_FAIL;
	}
	net_connectd.status = NET_LISTENED;
	LOG_INFO("[Dispatcher][init_net_manager] net_manager_app::create_tcp_server ok, net_id:%d", net_connectd.net_id);

	net_web.net_id = net_manager->create_tcp_server(net_web.ip, net_web.port, &m_packet_splitter_string,
				Dispatcher::NetEventHandler, Dispatcher::NetEventHandler);

	if (0 == net_web.net_id) 
	{			// 创建网络通道失败
		LOG_ERROR("[Dispatcher][init_net_manager] net_manager_app::create_tcp_server error");
		return RET_NET_LISTEN_FAIL;
	}
	net_web.status = NET_LISTENED;
	LOG_INFO("[Dispatcher][init_net_manager] net_manager_app::create_tcp_server ok, net_id:%d", net_web.net_id);
	
	return RET_OK;
}

Dispatcher::Dispatcher()
{
	local_stop = 0;
}


void Dispatcher ::stop_worker()
{
	static int	re_exit = 0;			

	if (0 == re_exit) 
	{
		LOG_INFO("[stop_worker]Begin stop all workers re_exit=%d", re_exit);
		re_exit ++;
		{
			vector<Web_worker *>::iterator it = p_web_workers.begin ();
			for (; it != p_web_workers.end (); it ++) 
			{
				(*it)->local_stop = true;
			}
			for (it = p_web_workers.begin (); it != p_web_workers.end (); it ++) 
			{
				(*it)->wait();
				LOG_INFO("[stop_worker:p_web_workers]Thread pWorker joined");
				delete *it;
			}
			p_web_workers.clear();
		}

		{
			vector<Connectd_worker *>::iterator it = p_connectd_workers.begin();
			for (; it != p_connectd_workers.end (); it ++) 
			{
				(*it)->local_stop = true;
			}
			for (it = p_connectd_workers.begin (); it != p_connectd_workers.end (); it ++) 
			{
				(*it)->wait();
				LOG_INFO("[stop_worker:p_connectd_workers]Thread pWorker joined");
				delete *it;
			}
			p_connectd_workers.clear();
		}
		
	}
}

int Dispatcher ::init_worker()
{
	int i = 0;
	//此处启动各个工作线程

	for(i = 0; i < Dispatcher::web_worker_num; i++)
	{
		Web_worker* pWorker = new Web_worker(i);
		if(NULL == pWorker)
		{
			LOG_ERROR("New Web_worker [%d] failed:%s",
					i, strerror(errno));
			delete pWorker;
			return -1;
		}
		if(0 != pWorker->activate())
		{
			LOG_ERROR("Web_worker [%d] activate failed:%s",
					i, strerror(errno));
			delete pWorker;
			return -1;
		}
		p_web_workers.push_back(pWorker);
	}
	LOG_INFO("[All WEB Service start] Success");

    for(i = 0; i < Dispatcher::connectd_worker_num; i++)
    {
            Connectd_worker* pWorker = new Connectd_worker(i);
            if(NULL == pWorker)
            {
                    LOG_ERROR("New Connectd_worker [%d] failed:%s",
                                    i, strerror(errno));
                    delete pWorker;
                    return -1;
            }
            if(0 != pWorker->activate())
            {
                    LOG_ERROR("Connectd_worker [%d] activate failed:%s",
                                    i, strerror(errno));
                    delete pWorker;
                    return -1;
            }
            p_connectd_workers.push_back(pWorker);
    }
	LOG_INFO("[All CONNECTD Service start] Success");
	return RET_OK;
}

int Dispatcher ::init()
{
	int ret = 0;
	ret = loadCfg();
	if(RET_OK != ret)
	{
		LOG_ERROR("[Dispatcher][init]loadCfg fail ret=%d", ret);
		return ret;
	}
	
	ret = init_net_manager();
	if(RET_OK != ret)
	{
		LOG_ERROR("[Dispatcher][init]init_net_manager fail ret=%d", ret);
		return ret;
	}
	
	ret = init_worker();
	if(RET_OK != ret)
	{
		LOG_ERROR("[Dispatcher][init]init_worker fail ret=%d", ret);
		return ret;
	}
	
	return RET_OK;
}
int Dispatcher ::checkIncomingIP(Net_Event* net_event)
{
	struct in_addr connection_srcip;
	unsigned int connection_srcPort = 0;
	
	connection_srcip.s_addr = net_event->remote_addr.get_addr();
	connection_srcPort = net_event->remote_addr.get_port();

	if(net_connectd.net_id == net_event->id)
	{
		//在长连接allowiplist中找此ip
		;
	}
	else if(net_web.net_id == net_event->id)
	{
		//在短连接allowiplist中找此ip
		;
	}
		
	return RET_OK;
}

int Dispatcher ::onAPPNetClose(uint8_t net_source, Net_Event* net_event)
{
	struct in_addr connection_srcip;
	unsigned int connection_srcPort = 0;
	
	connection_srcip.s_addr = net_event->remote_addr.get_addr();
	connection_srcPort = net_event->remote_addr.get_port();

	if(net_source == NET_SOURCE_CONNECTD)
	{
		net_map_connectd.erase(net_event->id);
		LOG_WARN("[Dispatcher]::onAPPNetClose Remove netid=%d ip=%s port=%d from net_map_connectd", 
						net_event->id,inet_ntoa((struct in_addr)connection_srcip), connection_srcPort); 
	}
	else
	{
		LOG_INFO("[Dispatcher]::onAPPNetClose Remove netid=%d ip=%s port=%d from net_map_web", 
						net_event->id,inet_ntoa((struct in_addr)connection_srcip), connection_srcPort); 
	}	
	
	return RET_OK;
}

int Dispatcher ::onAPPNetError(uint8_t net_source, Net_Event* net_event)
{
	struct in_addr connection_srcip;
	unsigned int connection_srcPort = 0;
	
	connection_srcip.s_addr = net_event->remote_addr.get_addr();
	connection_srcPort = net_event->remote_addr.get_port();

	if(net_source == NET_SOURCE_CONNECTD)
	{
		net_map_connectd.erase(net_event->id);
		LOG_WARN("[Dispatcher]::onNetError Remove netid=%d ip=%s port=%d from net_map_connectd", 
						net_event->id,inet_ntoa((struct in_addr)connection_srcip), connection_srcPort); 
	}
	else
	{
		LOG_WARN("[Dispatcher]::onNetError Remove netid=%d ip=%s port=%d from net_map_web", 
						net_event->id,inet_ntoa((struct in_addr)connection_srcip), connection_srcPort); 
	}	
	
	return RET_OK;	
}

int Dispatcher ::onAPPNetData(uint8_t net_source, Net_Event* net_event)
{
	int ret = 0;
	switch(net_source)
	{
		//从event中解析出request报文，组织成request结构，将request插入到相应工作类的处理对列中
		case NET_SOURCE_CONNECTD:
		{
			LOG_HEX_PREFIX("Dispatcher ::onAPPNetData GET CONNECTD MSG:", net_event->packet.ptr(), net_event->packet.length(), utils::L_DEBUG);
			ret = Connectd_worker::add_request(net_event);
			break;
		}
		case NET_SOURCE_WEB:
		{
			LOG_HEX_PREFIX("Dispatcher ::onAPPNetData GET WEB MSG:", net_event->packet.ptr(), net_event->packet.length(), utils::L_DEBUG);
			ret = Web_worker::add_request(net_event);
			break;
		}
		case NET_ADMIN_SOURCE:
		{
			LOG_DEBUG("[Dispatcher]Admin Tcp Server On Data type\n");
			AdminTcpHandler::NetEventHandler(*net_event);
		}
		default:
		{
			break;
		}
	}
	return RET_OK;
}


int Dispatcher ::onAPPNetAccept(Net_Event* net_event)
{
	int ret = 0;

	struct in_addr connection_srcip;
	unsigned int connection_srcPort = 0;
	
	connection_srcip.s_addr = net_event->remote_addr.get_addr();
	connection_srcPort = ntohs(net_event->remote_addr.get_port());

	//接入IP合法性校验
	ret = checkIncomingIP(net_event);
	if(ret != RET_OK)
	{
		LOG_WARN("[Dispatcher]Illeage ip=%s port=%d deny it", 
							inet_ntoa((struct in_addr)connection_srcip), connection_srcPort);	
		net_manager->delete_net(net_event->new_id);
		return RET_ILLEGAL_CLIENT;
	}
	else
	{
		if(net_connectd.net_id == net_event->id)
		{
			//长连接需要保存连接信息，以备对连接超时的检查
			NetInfo tempNetInfo;
			
			tempNetInfo.net_id = net_event->new_id;
			tempNetInfo.updateTime = time(NULL);
			tempNetInfo.port = connection_srcPort;
			tempNetInfo.status = NET_CONNECTED;

			strncpy(tempNetInfo.ip, inet_ntoa((struct in_addr)connection_srcip), 
						sizeof(tempNetInfo.ip)-1); 

			net_map_connectd[net_event->new_id] = tempNetInfo;			
			
			LOG_INFO("[Dispatcher]Long tcp connect accept ip=%s port=%d COMING, net_id=%d",
							tempNetInfo.ip, tempNetInfo.port,
							tempNetInfo.net_id);	 
		}
		else if(net_web.net_id == net_event->id)
		{			
//				//短连接需要保存连接信息，以备对连接合法性检查
			//修改: 此处不判断连接有效性，只要上来了event，都认为是合法的
				NetInfo tempNetInfo;
				
				tempNetInfo.net_id = net_event->new_id;
				tempNetInfo.updateTime = time(NULL);
				tempNetInfo.port = connection_srcPort;
				tempNetInfo.status = NET_CONNECTED;
				strncpy(tempNetInfo.ip, inet_ntoa((struct in_addr)connection_srcip), 
							sizeof(tempNetInfo.ip)-1); 				
				net_map_web[net_event->new_id] = tempNetInfo;			
			LOG_INFO("[Dispatcher]Short tcp connect accept ip=%s port=%d COMING, net_id=%d", 
								inet_ntoa((struct in_addr)connection_srcip), connection_srcPort,
								net_event->new_id);			
		}
		else if(admin_connected.net_id == net_event->id)
		{
			LOG_INFO("[Dispatcher] admin Connected accept ip = %s port= %d Coming, net id = %d\n",inet_ntoa((struct in_addr)connection_srcip), connection_srcPort,
								net_event->new_id);
		}
		else 
		{
			LOG_WARN("[Dispatcher]Connect coming to unkown listen net=%d from ip=%s port=%d deny it", 
								net_event->id, inet_ntoa((struct in_addr)connection_srcip), connection_srcPort);	
			net_manager->delete_net(net_event->new_id);			
			return RET_UNKOWN_SOURCE;
		}
	}
	return RET_OK;

}


int Dispatcher ::handleEvent(Net_Event* net_event)
{
	struct in_addr connection_srcip;
	unsigned int connection_srcPort = 0;
	struct in_addr connection_local_ip;
	unsigned int connection_local_port = 0;
	int ret = RET_CONTINUE;
	
	connection_srcip.s_addr = net_event->remote_addr.get_addr();
	connection_srcPort = ntohs(net_event->remote_addr.get_port());
	connection_local_ip.s_addr = net_event->local_addr.get_addr();
	connection_local_port = ntohs(net_event->local_addr.get_port());

	//是否为连接请求
	if(net_connectd.net_id == net_event->id 
		|| net_web.net_id == net_event->id || net_event->id == admin_connected.net_id)
	{
		switch (net_event->net_event_type)
		{
		case TYPE_ACCEPT:
			LOG_INFO("[Dispatcher]::handleEvent GET TYPE_ACCEPT EVENT at listen socket %d from ip=%s port=%u.", 
				net_event->id,inet_ntoa((struct in_addr)connection_srcip), connection_srcPort);					
			onAPPNetAccept(net_event); 
			break;

		default:
			LOG_WARN("[Dispatcher]::handleEvent GET unknown net_event_type:%d at listen socket %d from ip=%s port=%u.", 
				net_event->net_event_type, net_event->id,
				inet_ntoa((struct in_addr)connection_srcip), connection_srcPort);
			break;
		}
	}
	else
	{
		NetMap::iterator it;
		uint8_t net_source = NET_SOURCE_BUTT;
		
		//检查该连接是否有效		
		if(net_map_connectd.end()
			!= (it = net_map_connectd.find(net_event->id)))
		{
			net_source = NET_SOURCE_CONNECTD;
			//该数据来自合法的connectd连接
			//该链接有数据到达，则更新该链接的时间
			it->second.updateTime = cur_time;
		}
		else if(0 == strcmp(inet_ntoa((struct in_addr)connection_local_ip), net_web.ip)  && net_web.port == connection_local_port)
		{
			//该数据来自合法的web连接		
			if(net_map_web.find(net_event->id) != net_map_web.end()){
				it->second.updateTime = cur_time;
				LOG_DEBUG("CONNECT ON WEB SOURCE\n");
				net_source = NET_SOURCE_WEB;
			}
		}
		else if(0 == strcmp(inet_ntoa((struct in_addr)connection_local_ip), admin_connected.ip) && connection_local_port == admin_connected.port)
		{
			LOG_DEBUG("CONNECT ON ADMIN SOURCE\n");
			net_source = NET_ADMIN_SOURCE;
		}
		else
		{
			//该数据来自未知连接
			LOG_WARN("[Dispatcher]::handleEvent GET event on unresonable connection, net=%d from ip=%s port=%u to ip=%s port=%d.", 
					net_event->id,	
					inet_ntoa((struct in_addr)connection_srcip), 
					connection_srcPort,
					inet_ntoa((struct in_addr)connection_local_ip), 
					connection_local_port);
		}

		if(net_source != NET_SOURCE_BUTT)
		{		
			//连接被管理，且连接状态有效，或者连接为短连接
			switch (net_event->net_event_type)
			{				
			case TYPE_CLOSE:
				LOG_INFO("[Dispatcher]::handleEvent GET TYPE_CLOSE EVENT at connected socket %d  ip=%s port=%u.", 
							net_event->id,inet_ntoa((struct in_addr)connection_srcip), connection_srcPort); 					
				onAPPNetClose(net_source, net_event);
				break;		
				
			case TYPE_DATA:
				//All check pass
				LOG_DEBUG("[Dispatcher]::handleEvent  GET DATA on socket %d ip=%s port=%u", net_event->id, inet_ntoa((struct in_addr)connection_srcip), connection_srcPort);
				//该链接有数据到达，则更新该链接的时间
				onAPPNetData(net_source, net_event);
				//仅在真正进行数据处理时返回ret_ok,此时不删除event
				ret = RET_OK;
				break;
			case TYPE_EXCEPTION:
			default:
				LOG_WARN("[Dispatcher]::handleEvent GET TYPE_EXCEPTION EVENT at connected socket %d  ip=%s port=%u.", 
				net_event->id,inet_ntoa((struct in_addr)connection_srcip), connection_srcPort); 					
				onAPPNetError(net_source, net_event);
				break;
			}
		}
	}

	if(ret != RET_OK)
	{
		delete net_event;
	}
	
	return ret;	
}

void Dispatcher::NetEventHandler(Net_Event& net_event)
{
	//DUMMY FUNCTION, don't use function pointer model
	return;
}
void Dispatcher::TimerTickHandler(time_t current_time)
{
	LOG_TRACE("[Dispatcher]TimeTickHandler \n");
	checkTimeoutUser(current_time);
}


void Dispatcher::checkTimeoutUser(time_t current_time)
{
	int count = 0;
	NetMap::iterator it = net_map_web.begin();
	for(;it!= net_map_web.end();)
	{
		if(current_time - it->second.updateTime > (uint16_t)shorttcp_timeout)
		{
			//NET INFO TIMEOUT, DISCONNECT
			LOG_WARN("PhpAgent::checkTimeoutUser shorttcp connect timeout net:%d, ip:%s, port:%d, updateTime:%ld, current_time:%ld",
					it->second.net_id, it->second.ip, it->second.port, it->second.updateTime, current_time);
			net_manager->delete_net(it->second.net_id);
			net_map_web.erase(it++);
			count++;
		}else{
			it++;
		}
	}

	it = net_map_connectd.begin();
	for(;it!= net_map_connectd.end();)
	{
		if(current_time - it->second.updateTime > (uint16_t)longtcp_timeout)
		{
			//NET INFO TIMEOUT, DISCONNECT
			LOG_WARN("PhpAgent::checkTimeoutUser longtcp_timeout connect timeout net:%d, ip:%s, port:%d, updateTime:%ld, current_time:%ld",
					it->second.net_id, it->second.ip, it->second.port, it->second.updateTime, current_time);
			net_manager->delete_net(it->second.net_id);
			net_map_connectd.erase(it++);
			count++;
		}
		else{
			it++;
		}
	}

	LOG_INFO("[Dispatcher]checkTimeoutUser: Timeout User count = %d\n",count);
}

void Dispatcher::registerHandler(pfnTimerTickHandler handler)
{
	if(timeHandlerSet.find(handler) == timeHandlerSet.end()){
		timeHandlerSet.insert(handler);
	}
}

void Dispatcher::stop()
{
	local_stop = 1;
}

int Dispatcher ::svc()
{
	int ret = 0;
	ret = init();
	time_t lastTime = time(NULL);
	if(ret != RET_OK)
	{
		LOG_ERROR("[Dispatcher]INIT RET=%d", ret);
		return ret;
	}
	LOG_DEBUG("Begin dispatcher...");

	static uint64_t g_pluse_mainthread = 1;
	while(!local_stop)
	{
		cur_time = time(NULL);
		if(cur_time - lastTime > (uint16_t)shorttcp_timeout){
			TimerTickHandler(cur_time);
			LOG_INFO("[PhpAgent Dispatcher Main Thread Pluse=%lu]\n",g_pluse_mainthread++);
			for(TimeHandlerSet::iterator it = timeHandlerSet.begin();it!=timeHandlerSet.end();++it){
				pfnTimerTickHandler timeHandler = *it;
				timeHandler(cur_time);
			}
			lastTime = cur_time;
		}
		
		/**********************************************************/
		/*********处理外部业务系统的请求**********************/
		/**********************************************************/

		//从net_manager中获取网络事件
		Net_Event* event = net_manager->get_event(); 

		if(event == NULL)
		{
			usleep(sleep_interval_ms);
		}
		else
		{
			//检查app_event，(检查请求类型，对非正常请求的app_event进行处理)
			if(RET_OK != (ret = handleEvent(event)))
			{
				LOG_DEBUG("[Dispatcher]::handleEvent return=%d.", ret);
				
			}
		}
		
	}
	LOG_DEBUG("Begin stop dispatcher...");
	stop_worker();
	LOG_DEBUG("Dispatcher stop finish.");
	return RET_OK;
}
