

#include "admin_handler.h"
#include "dispatcher.h"
#include "options.h"

#include "log.h"


// class AdminUdpHandler
pfnUserCmdHandler AdminUdpHandler::user_cmd_handler;

AdminUdpInfo AdminUdpHandler::admin_udp_info;

int AdminUdpHandler::ADMIN_RECONNECT_INTERVAL;


void AdminUdpHandler::NetEventHandler(Net_Event& net_event)
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
		LOG_FATAL("AdminUdpHandler::NetEventHandler error, unknown net_event_type:%d", net_event.net_event_type);
		break;
	}
}

void AdminUdpHandler::TimerTickHandler(time_t current_time)
{
	// 处理连接状态
	if ((0 != admin_udp_info.reconnect_time) && (current_time >= admin_udp_info.reconnect_time))
	{
		start();
	}
}

int AdminUdpHandler::initialize()
{
	LOG_INFO("AdminUdpHandler::initialize, ip:%s, port:%d", Options::admin_server.admin_ip.c_str(), Options::admin_server.admin_port);

	admin_udp_info.net_id = 0;
	admin_udp_info.admin_ip = Options::admin_server.admin_ip;
	admin_udp_info.admin_port = Options::admin_server.admin_port;
	admin_udp_info.reconnect_time = 0;

	ADMIN_RECONNECT_INTERVAL = Options::admin_server.reconnect_interval;

	return 0;
}

void AdminUdpHandler::setUserCmdHandler(pfnUserCmdHandler handler)
{
	user_cmd_handler = handler;
}

int AdminUdpHandler::start()
{
	LOG_INFO("AdminUdpHandler::start begin");

	if (0 != admin_udp_info.net_id) {			// 通道已经建立
		return 0;
	}

	Net_Manager *net_manager = Dispatcher::getNetManager();
	admin_udp_info.net_id = net_manager->create_udp(admin_udp_info.admin_ip.c_str(), admin_udp_info.admin_port,
					NetEventHandler);
	if (0 == admin_udp_info.net_id) {			// 创建网络通道失败
		admin_udp_info.reconnect_time = time(NULL) + ADMIN_RECONNECT_INTERVAL;	// 设置重新建立通道时间
		LOG_FATAL("AdminUdpHandler::start error, create_udp error");
		return -1;
	}

//		Dispatcher::registerNetEventHandler(admin_udp_info.net_id, NetEventHandler);

	admin_udp_info.reconnect_time = 0;

	LOG_INFO("AdminUdpHandler::start ok, net_id:%d", admin_udp_info.net_id);
	return 0;
}

int AdminUdpHandler::stop()
{
	LOG_INFO("AdminUdpHandler::stop begin, net_id:%d", admin_udp_info.net_id);

	if (0 == admin_udp_info.net_id) {		// 没有通道
		return 0;
	}

	Dispatcher::getNetManager()->delete_net(admin_udp_info.net_id);
	Dispatcher::removeNetEventHandler(admin_udp_info.net_id);

	admin_udp_info.net_id = 0;
	admin_udp_info.reconnect_time = 0;

	LOG_INFO("AdminUdpHandler::stop ok");
	return 0;
}

void AdminUdpHandler::onNetError(Net_Event& net_event)
{
	LOG_FATAL("AdminUdpHandler::onNetError, net_id:%d", admin_udp_info.net_id);

	Dispatcher::removeNetEventHandler(admin_udp_info.net_id);

	admin_udp_info.net_id = 0;
	admin_udp_info.reconnect_time = time(NULL) + ADMIN_RECONNECT_INTERVAL;
}

void AdminUdpHandler::onNetData(Net_Event& net_event)
{
	Net_Packet *net_packet = new Net_Packet;
	int result_len = 0;

	user_cmd_handler(net_event.packet.ptr(), net_event.packet.length(),
		net_packet->ptr(), net_packet->max_packet_length(), result_len);

	net_packet->length(result_len);

    Net_Manager *net_manager = Dispatcher::getNetManager();
    int rc = net_manager->send_packet(net_event.id, net_packet, net_event.remote_addr);
    if (0 != rc) {
        delete net_packet;
        LOG_FATAL("AdminUdpHandler::onNetData error, send_packet error");
    }

	return;
}




// class AdminTcpListener
pfnUserCmdHandler AdminTcpListener::user_cmd_handler;

AdminTcpInfo AdminTcpListener::admin_tcp_info;

int AdminTcpListener::ADMIN_RECONNECT_INTERVAL;

Line_Splitter AdminTcpListener::m_packet_splitter;


void AdminTcpListener::NetEventHandler(Net_Event& net_event)
{
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
}

void AdminTcpListener::TimerTickHandler(time_t current_time)
{
	// 处理连接状态
	if ((0 != admin_tcp_info.reconnect_time) && (current_time >= admin_tcp_info.reconnect_time))
	{
		start();
	}
}

int AdminTcpListener::initialize()
{
	LOG_INFO("AdminTcpListener::initialize, ip:%s, port:%d", Options::admin_server.admin_ip.c_str(), Options::admin_server.admin_port);

	admin_tcp_info.net_id = 0;
	admin_tcp_info.admin_ip = Options::admin_server.admin_ip;
	admin_tcp_info.admin_port = Options::admin_server.admin_port;
	admin_tcp_info.reconnect_time = 0;

	ADMIN_RECONNECT_INTERVAL = Options::admin_server.reconnect_interval;

	return 0;
}

void AdminTcpListener::setUserCmdHandler(pfnUserCmdHandler handler)
{
	user_cmd_handler = handler;
}

int AdminTcpListener::start()
{
	LOG_INFO("AdminTcpListener::start begin");

	if (0 != admin_tcp_info.net_id) {			// 通道已经建立
		return 0;
	}

	Net_Manager *net_manager = Dispatcher::getNetManager();
	admin_tcp_info.net_id = net_manager->create_tcp_server(admin_tcp_info.admin_ip.c_str(), admin_tcp_info.admin_port, &m_packet_splitter,
				NetEventHandler, AdminTcpHandler::NetEventHandler);
	if (0 == admin_tcp_info.net_id) {			// 创建网络通道失败
		admin_tcp_info.reconnect_time = time(NULL) + ADMIN_RECONNECT_INTERVAL;	// 设置重新建立通道时间
		LOG_FATAL("AdminTcpListener::start error, create_udp error");
		return -1;
	}

//		Dispatcher::registerNetEventHandler(admin_tcp_info.net_id, NetEventHandler);

	admin_tcp_info.reconnect_time = 0;

	LOG_INFO("AdminTcpListener::start ok, net_id:%d", admin_tcp_info.net_id);
	return 0;
}

int AdminTcpListener::stop()
{
	LOG_INFO("AdminTcpListener::stop begin, net_id:%d", admin_tcp_info.net_id);

	if (0 == admin_tcp_info.net_id) {		// 没有通道
		return 0;
	}

	Dispatcher::getNetManager()->delete_net(admin_tcp_info.net_id);
	Dispatcher::removeNetEventHandler(admin_tcp_info.net_id);

	admin_tcp_info.net_id = 0;
	admin_tcp_info.reconnect_time = 0;

	LOG_INFO("AdminTcpListener::stop ok");
	return 0;
}

void AdminTcpListener::onNetError(Net_Event& net_event)
{
	LOG_FATAL("AdminTcpListener::onNetError, net_id:%d", admin_tcp_info.net_id);

	Dispatcher::removeNetEventHandler(admin_tcp_info.net_id);

	admin_tcp_info.net_id = 0;
	admin_tcp_info.reconnect_time = time(NULL) + ADMIN_RECONNECT_INTERVAL;
}

void AdminTcpListener::onNetAccept(Net_Event& net_event)
{
	LOG_INFO("AdminTcpListener::onNetAccept, net_id:%d, new_net_id:%d", admin_tcp_info.net_id, net_event.new_id);
//		Dispatcher::registerNetEventHandler(net_event.new_id, AdminTcpHandler::NetEventHandler);
}


// class AdminTcpHandler
void AdminTcpHandler::NetEventHandler(Net_Event& net_event)
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
	Dispatcher::removeNetEventHandler(net_event.id);
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

	Net_Manager *net_manager = Dispatcher::getNetManager();
    int rc = net_manager->send_packet(net_event.id, net_packet, net_event.remote_addr);
    if (0 != rc) {
        delete net_packet;
        LOG_FATAL("AdminTcpHandler::onNetData error, send_packet error");
    }

	return;
}

