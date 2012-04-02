/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <errno.h>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>
#include "MISInterface.h"
#include "MQueueInterface.h"


using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace boost;

int main(int argc, char** argv) {
  shared_ptr<TTransport> socket(new TSocket("localhost", 12321));
  shared_ptr<TTransport> transport(new TFramedTransport(socket));
  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  MISInterfaceClient client(protocol);

  shared_ptr<TTransport> socket1(new TSocket("localhost", 12323));
  shared_ptr<TTransport> transport1(new TFramedTransport(socket1));
  shared_ptr<TProtocol> protocol1(new TBinaryProtocol(transport1));
  MQueueInterfaceClient client1(protocol1);
  int c = 0;
  bool isMQuit = false;
  bool isMSender  =false;
  bool isMGet = false;
  bool isMPing = false;
  bool isQExit = false;
  bool isQR = false;
  while((c =getopt(argc,argv,"qsgper"))!= -1){ 
	  switch(c){
			case 'q':
				isMQuit = true;
				break;
			case 's':
				isMSender = true;
				break;
			case 'g':
				isMGet = true;
				break;
			case 'p':
				isMPing = true;
				break;
			case 'e':
				isQExit = true;
				break;
			case 'r':
				isQR = true;
				break;
	  }
  }

  try {	
	
	if(isMQuit == true || isMSender == true || isMGet == true || isMPing == true){
		transport->open();
	}

	if(isQExit == true|| isQR == true) {
		transport1->open();
	}

	if(isMPing == true){
   		client.ping();
		printf("ping()\n");
	}

	if(isMSender == true){
		MISRequest request;
		for(int i = 1; i<=10;i++){
		request.user_id = "123";
		request.record_id = "321";
		request.ip = "10.0.10.10";
		request.channel = "bbs";
		request.content  = "hello world\n";
		MISResponse response;
		client.check(response, request);
		printf("check()\n");

		client.reload();
		printf("reload()\n");

		client.add2Q(request, "abc");
		printf("add2Q\n");
		}
	}

	if(isMGet == true){
		int num = client.getCurMsgNum();
		printf("num is %d\n",num);
		printf("getCurMsgNum\n");
	}

	if(isMQuit == true){
		client.quit();
		printf("mis quit\n");
	}

	if(isQExit == true){
		client1.quit();
		printf("mqueue quit\n");
	}
	
	if(isQR == true){
		int num = client1.getCurMsgNum();
		printf("getCurMsgNum == %d\n",num);
	}

	if(isMQuit == true || isMSender == true || isMGet == true || isMPing == true){
		transport->close();
	}

	if(isQExit == true|| isQR == true) {
		transport1->close();
	}

  } catch (TException &tx) {
    printf("ERROR: %s\n", tx.what());
  }

}

