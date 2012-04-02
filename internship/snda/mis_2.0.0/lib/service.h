#ifndef _MIS_SERVICE_H_
#define _MIS_SERVICE_H_

#include "channel.h"
#include "helper.h"
#include "io.h"
#include <vector>
#include <time.h>
#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <mysql++/mysql++.h>
#include <signal.h>

#define MQUEUEPATH  "mqueue"

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IROTH | S_IRGRP)

#define MYSIGNAL SIGUSR1

typedef vector<MISChannel*> MISChannelList;

using namespace mysqlpp;


class MISService {
	public:
		MISService(MISConfig& config);
		virtual ~MISService();
		void  destroy();

	public:
		bool loadFromDB();
		bool addToQueue(MISInput& input, MISOutput& output);
		bool addToQueue(MISInput& input, const string& reason);
		bool addToLog(MISInput& input, MISOutput& output);

	protected:
		bool _loadFromDB();
		bool _addToQueue(MISInput& input, MISOutput& output);
		bool _addToQueue(MISInput& input, const string& reason);
		bool _addToLog(MISInput& input, MISOutput& output);

	public:
		MISChannel* getChannel(const string& name, bool create = false);
		void check(MISInput& input, MISOutput& output);

	protected:
		bool checkDBConnection();
		void clear();

	protected:
		MISChannelList m_channels;
		Connection m_db_conn;
		MISConfig m_config;
		bool m_connection_error;
		unsigned long m_last_ping_time;
	public:
		void createQueue();
		bool receiveRecord();
		int getCurMsgNum();
		bool startQueue();
		bool openQueue();
		void stopQueue();
	private:
		bool sendToQueue(MISInput& input, MISOutput & output);
		bool sendToQueue(MISInput& input, const string& reason);
		bool sendToLog(MISInput& input, MISOutput& output);
		bool sendRecord(const string & data);
		bool insertToDb(const string & sql);
		void exitService();
	public:
    static void* threadHandler(void*);
		bool isStop;
	private:
		struct ThreadArgs
		{
			MISService * This;
			ThreadArgs(MISService * This):This(This){}
		};
		static pthread_mutex_t lock;
		string mqueuePath;
		mqd_t mqd;
		mq_attr attr;
		char * buf;
		int msgMax;//max number of msg in the queue
		int msgSizeMax;//max size of msg in the queue
};

#endif
