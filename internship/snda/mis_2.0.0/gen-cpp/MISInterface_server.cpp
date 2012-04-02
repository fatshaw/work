// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "MISInterface.h"

#include "../lib/helper.h"
#include "../lib/service.h"

#include<stdio.h>
#include<unistd.h>


#include <protocol/TBinaryProtocol.h>
#include <server/TNonblockingServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <server/TSimpleServer.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;


class MISInterfaceHandler : virtual public MISInterfaceIf {
 public:
	MISInterfaceHandler(MISConfig& config) : m_service(config) {
	}

	bool init() {
		m_service.createQueue();
		return m_service.loadFromDB();
	}

	void check(MISResponse& _return, const MISRequest& request) {
		MISInput input;
		MISOutput output;
		input.user_id = request.user_id;
		input.channel = request.channel;
		input.ip = request.ip;
		input.content = request.content;
		input.record_id = request.record_id;
		m_service.check(input, output);

		_return.result = output.m_result;
		if (output.m_matched_result) {
			char temp[24];
			bool started = false;
			for (MISMatchedResult::iterator iter = output.m_matched_result->begin(); iter != output.m_matched_result->end(); ++iter) {
				if (started) {
					sprintf(temp, "\n%d:", iter->first);
				} else {
					started = true;
					sprintf(temp, "%d:", iter->first);
				}
				_return.reason += temp;
				_return.reason += iter->second;
			}
		}
	}

	void add2Q(const MISRequest& request, const std::string& reason) {
		MISInput input;
		input.user_id = request.user_id;
		input.channel = request.channel;
		input.ip = request.ip;
		input.content = request.content;
		input.record_id = request.record_id;
		m_service.addToQueue(input, reason);
	}

	void reload() {
		m_service.loadFromDB();
	}

	int8_t ping() {
		return 1;
	}

	void quit()
	{
		m_service.destroy();
	//	kill(0,MYSIGNAL);
		exit(-1);
	}

	int32_t getCurMsgNum()
	{
		return m_service.getCurMsgNum();
	}

	protected:
		MISService m_service;
};

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("usage: %s [config file]\n", argv[0]);
		return 0;
	}

	map<string, string> config;
	if (!MISHelper::readConfig(argv[1], config)) {
		printf("bad config file: %s\n", argv[1]);
		return 0;
	}

	string portstr = config["server.port"];
	int port = 9090;
	if (!portstr.empty()) port = atoi(portstr.c_str());

	shared_ptr<MISInterfaceHandler> handler(new MISInterfaceHandler(config));
	if (!handler->init()) {
		printf("init server failed, please check your config file\n");
		return 0;
	}

	close(0);
	close(1);
	close(2);

	shared_ptr<TProcessor> processor(new MISInterfaceProcessor(handler));
	//shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	//shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	//shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	TNonblockingServer server(processor, port);
	/*TSimpleServer server(processor,
                       serverTransport,
                       transportFactory,
                       protocolFactory);*/
	printf("server started at:%d\n", port);
	server.serve();
	return 0;
}
