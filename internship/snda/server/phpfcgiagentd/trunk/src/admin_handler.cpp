#include "admin_handler.h"

pfnUserCmdHandler AdminTcpListener::user_cmd_handler;
AdminTcpInfo AdminTcpListener::adminTcpInfo;
int AdminTcpListener::RECONNECT_TIME_INTERVAL;
Line_Splitter AdminTcpListener::m_line_splitter;

void AdminTcpHandler::NetEventHandler(Net_Event & net_event)
{
	switch (net_event.net_event_type)
	{
		case TYPE_EXCEPTION:
		case TYPE_CLOSE:
			onNetError(net_event);
			break;

		case TYPE_DATA:
			onNetData(net_event);
			break;

		default:
			LOG_FATAL("AdminTcpHandler::NetEventHandler error, unknown net_event_type:%d", net_event.net_event_type);
			break;
	}
}
void AdminTcpHandler::onNetError(Net_Event& net_event)
{
	LOG_FATAL("AdminTcpHandler::onNetError, net_id:%d", net_event.id);
}

void AdminTcpHandler::onNetData(Net_Event& net_event)
{
	Net_Packet *net_packet = new Net_Packet;
	int result_len = 0;

	AdminTcpListener::user_cmd_handler(net_event.packet.ptr(), net_event.packet.length(),
		net_packet->ptr(), net_packet->max_packet_length() - 2, result_len);
	net_packet->ptr()[result_len] = '\r';
	net_packet->ptr()[result_len + 1] = '\n';
	net_packet->length(result_len + 2);
	event_put_mutex.acquire();	
	int rc = net_manager->send_packet(net_event.id, net_packet,net_event.remote_addr);	
	event_put_mutex.release();
	if (0 != rc) {
	   	delete net_packet;
	    	LOG_FATAL("AdminTcpHandler::onNetData error, send_packet error");
	}
	LOG_INFO("[AdminTcpHandler OnNetData Success]");
	return;
}

void AdminTcpListener::NetEventHandler(Net_Event& net_event){
	switch (net_event.net_event_type)
	{
		case TYPE_EXCEPTION:
		case TYPE_CLOSE:
			onNetError(net_event);
			break;
		case TYPE_ACCEPT:
			onNetAccept(net_event);
			break;
		default:
			LOG_FATAL("AdminTcpListener::NetEventHandler error, unknown net_event_type:%d", net_event.net_event_type);
			break;
	}
	LOG_DEBUG("OnNetEventHandler\n");
}

int AdminTcpListener::onNetAccept(Net_Event & net_event){
	LOG_INFO("CONNECT TO ADMIN SERVER SUCCESS\n");				
	return 0;
}	

void AdminTcpListener::TimerTickHandler(time_t current_time){
	if(adminTcpInfo.reconnect_time > 0 && current_time > adminTcpInfo.reconnect_time){
		start();
	}
}

int AdminTcpListener::initialize(){
	adminTcpInfo.net_id = 0;
	adminTcpInfo.adminIp = adminIp;
	adminTcpInfo.adminPort  = adminPort;
	adminTcpInfo.reconnect_time = 0;
	RECONNECT_TIME_INTERVAL = reconnectInterval;
	return 1;
}
 int AdminTcpListener::start(){
	if(adminTcpInfo.net_id != 0 ){
		return 0;
	}
	LOG_INFO("Create tcp server %s,%d\n",adminTcpInfo.adminIp.c_str(),adminTcpInfo.adminPort);
	admin_connected.net_id = 0;
	adminTcpInfo.net_id = net_manager->create_tcp_server(adminTcpInfo.adminIp.c_str(),adminTcpInfo.adminPort,&m_line_splitter,AdminTcpListener::NetEventHandler
			,AdminTcpHandler::NetEventHandler);
	if(adminTcpInfo.net_id == 0)
	{
		LOG_FATAL("Start Admin Server %s , port %d\n",adminTcpInfo.adminIp.c_str(),adminTcpInfo.adminPort);
		adminTcpInfo.reconnect_time = time(NULL) + RECONNECT_TIME_INTERVAL;
		return -1;
	}
	admin_connected.net_id = adminTcpInfo.net_id;
	strncpy(admin_connected.ip,adminTcpInfo.adminIp.c_str(),adminTcpInfo.adminIp.length());
	admin_connected.port =  adminTcpInfo.adminPort;
	adminTcpInfo.reconnect_time = 0;
	LOG_INFO("CREATE ADMIN SERVER SUCCESS,net id = %d\n",adminTcpInfo.net_id);
	return 0;
}

void AdminTcpListener::setUserCmdHandler(pfnUserCmdHandler handler){
	user_cmd_handler = handler;
}

int AdminTcpListener::stop(){
	if(adminTcpInfo.net_id == 0){
		return 0;
	}
	net_manager->delete_net(adminTcpInfo.net_id);
	adminTcpInfo.net_id = 0;
	adminTcpInfo.reconnect_time = 0;
	return 0;
}

void AdminTcpListener::onNetError(Net_Event & net_event){
	LOG_FATAL("Admin Tcp Server onNetError, ip = %s, port = %d\n",adminTcpInfo.adminIp.c_str(),adminTcpInfo.adminPort);
	adminTcpInfo.net_id = 0;
	adminTcpInfo.reconnect_time = time(NULL) + RECONNECT_TIME_INTERVAL;
}


