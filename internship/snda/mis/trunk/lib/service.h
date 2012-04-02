#ifndef _MIS_SERVICE_H_
#define _MIS_SERVICE_H_

#include "channel.h"
#include "helper.h"
#include "io.h"
#include <vector>
#include <time.h>
#include <mysql++/mysql++.h>

typedef vector<MISChannel*> MISChannelList;

using namespace mysqlpp;

class MISService {
	public:
		MISService(MISConfig& config);
		virtual ~MISService();

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
};

#endif
