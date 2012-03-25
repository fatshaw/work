
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "user_cmd_handler.h"
#include "admin_handler.h"
#include "apipoold_listener.h"
#include "phpservice_handler.h"
#include "AccessControler.h"
#include "protobuf_handler.h"

#include "apipoold_handler.h"

#include "dispatcher.h"
#include "options.h"

#include "log.h"

#include "apipoold_configure.h"

char * configureFile ="apipoold.conf";

int daemonize()
{
	pid_t pid = fork();
	if (-1 == pid)
		return -1;
	else if (0 != pid)
		
		exit(0);

	setsid();

	signal(SIGHUP, SIG_IGN);

	pid = fork();
	if (-1 == pid)
		return -1;
	else if (0 != pid)
		exit(0);

	umask(0);

	return 0;
}

int supervise()
{
	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTERM, SIG_IGN);

	for (int i = 0; i < 10; i++)
	{
		pid_t pid = fork();
		if (-1 == pid)
			return -1;

		if (pid == 0) {
			break;
		}
		else {
			int status;
			waitpid(pid, &status, 0);
		}
	}

	return 0;
}

void exit_main(int sig)
{
	LOG_FATAL("Apipoold: Got a Signal, sig:%d", sig);
    Dispatcher::stop();
}

#ifndef _TEST
int main()
{
	// 守护进程
        /*if (0 != daemonize())
	{
		perror("daemonize error");
		return -1;
	}*/

	/*
	// 监控进程
	if (0 != supervise())
	{
		perror("supervise error");
		return -1;
	}
	*/

	// 设置信号
	signal(SIGINT, exit_main);
	signal(SIGHUP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	signal(SIGTERM, SIG_IGN);

	// 初始化日志
	LOG_INIT("../log/apipoold", 200000000, utils::L_TRACE);
	LOG_OPEN();

	LOG_SET_USEC(true);
 
	// 读取配置
	int rc = LoadApipooldConfigure(configureFile);
	if (0 != rc)
	{
		LOG_FATAL("Apipoold: LoadApipooldConfigure error");
		return -1;
	}

	// 启动NetManager
	LOG_INFO("Apipoold: Start Net Manager...");
	rc = Dispatcher::getNetManager()->start();
	if (0 != rc)
	{
		LOG_FATAL("Apipoold: Start Net Manager Error");
		return -1;
	}
	LOG_INFO("Apipoold: Start Net Manager OK");

	// 初始化并启动后台各个模块
	AdminTcpListener::initialize();
	AdminTcpListener::setUserCmdHandler(UserCmdHandler);
	AdminUdpHandler::initialize();
	AdminUdpHandler::setUserCmdHandler(UserCmdHandler);
	FcgiAgentHandler::initialize();
	ProtobufServerHandler::initialize();
	AccessControler::initialize();
	AdminTcpListener::start();
	AdminUdpHandler::start();


	FcgiAgentHandler::connect();
	ProtobufServerHandler::connect();

	Dispatcher::registerTimerTickHandler(AdminTcpListener::TimerTickHandler);
	Dispatcher::registerTimerTickHandler(AdminUdpHandler::TimerTickHandler);

	// 初始化服务模块
	ApipooldListener::initialize();
	rc = ApipooldListener::start ();
	if (0 != rc)
	{
		LOG_FATAL("Apipoold: Start Apipoold Handler Error");
		goto exit_start_apipoold_error;
	}
	
	Dispatcher::registerTimerTickHandler(ApipooldListener::TimerTickHandler);
	Dispatcher::registerTimerTickHandler(ApipooldHandler::TimerTickHandler);
	Dispatcher::registerTimerTickHandler(FcgiAgentHandler::TimerTickHandler);
	Dispatcher::registerTimerTickHandler(ProtobufServerHandler::TimerTickHandler);

	// 至此服务成功启动!
	LOG_INFO("Apipoold: Start OK!!!");

	// 事件循环
	Dispatcher::run();


	LOG_INFO("Apipoold: Begin Stop...");

	// 关闭udp服务模块
	ApipooldListener::stop();
	Dispatcher::removeTimerTickHandler(ApipooldHandler::TimerTickHandler);
	Dispatcher::removeTimerTickHandler(ApipooldListener::TimerTickHandler);

exit_start_apipoold_error:

	

	// 关闭后台服务模块
	AdminTcpListener::stop();
	AdminUdpHandler::stop();

	FcgiAgentHandler::stop();
	ProtobufServerHandler::stop();

	Dispatcher::removeTimerTickHandler(AdminTcpListener::TimerTickHandler);
	Dispatcher::removeTimerTickHandler(AdminUdpHandler::TimerTickHandler);

	// 关闭NetManager
	Dispatcher::getNetManager()->stop();

	LOG_INFO("Apipoold: Stop OK.");

	// 关闭日志
	LOG_CLOSE();

	return 0;
}

#else
int test()
{

	return 0;
}
int main()
{
	test();
}
#endif
