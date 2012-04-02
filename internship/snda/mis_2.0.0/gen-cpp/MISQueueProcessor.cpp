#include "../lib/helper.h"
#include "../lib/io.h"
#include "../lib/service.h"
#include "MQueueInterface.h"

#include<stdio.h>
#include<unistd.h>

#include <protocol/TBinaryProtocol.h>
#include <server/TNonblockingServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <server/TSimpleServer.h>


using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using boost::shared_ptr;


class MQueueInterfaceHandler : virtual public MQueueInterfaceIf
{

	public:
		MQueueInterfaceHandler(MISConfig & config):m_service(config){}
	public:
		bool init(){return m_service.openQueue();}
		void startQueue(){m_service.startQueue();}
		void quit()
		{
			m_service.stopQueue();
		}

		int32_t getCurMsgNum()
		{
			return m_service.getCurMsgNum();
		}

	private:
		MISService m_service;
};


int main(int argc,char ** argv){
	if (argc == 1) {
		printf("usage: %s [config file]\n", argv[0]);
		return 0;
	}

	map<string, string> config;
	if (!MISHelper::readConfig(argv[1], config)) {
		printf("bad config file: %s\n", argv[1]);
		return 0;
	}

	shared_ptr<MQueueInterfaceHandler> handler(new MQueueInterfaceHandler(config));
	string portstr = config["mqueue.port"];
	int port = 9090;
	if (!portstr.empty()) port = atoi(portstr.c_str());

	if(!handler->init()){
		cerr<<"open queue error"<<endl;
		return 1;
	}

	handler->startQueue();

	/*close(0);
	close(1);
	close(2);*/

	shared_ptr<TProcessor> processor(new MQueueInterfaceProcessor(handler));
	/*shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());*/

	TNonblockingServer server(processor, port);

	/*
	int workerCount = 5;
	shared_ptr<ThreadManager> threadManager =ThreadManager::newSimpleThreadManager(workerCount);
	shared_ptr<PosixThreadFactory> threadFactory =shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
	threadManager->threadFactory(threadFactory);

	threadManager->start();
	TThreadPoolServer server(processor,  serverTransport, transportFactory, protocolFactory, threadManager);
	*/
	//TThreadedServer server(processor,  serverTransport,   transportFactory, protocolFactory);

	/*TSimpleServer server(processor,
                       serverTransport,
                       transportFactory,
                       protocolFactory);*/
	printf("server started at:%d\n", port);
	server.serve();
	return 0;
}
