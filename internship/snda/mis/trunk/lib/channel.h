#ifndef _MIS_CHANNEL_H_
#define _MIS_CHANNEL_H_

#include "extractdict.h"
#include "matchdict.h"
#include "englishdict.h"
#include "frequencylimiter.h"
#include "io.h"
#include <vector>

typedef vector<MISDict*> MISDictList;

#define DICTTYPE_WHITE 0
#define DICTTYPE_BLACK 1
#define DICTTYPE_QUEUE 2

/**
 * channel
 **/
class MISChannel {
	public:
		MISChannel(const char* name);
		virtual ~MISChannel();

	public:
		void check(MISInput& input, MISOutput& output);

	public:
		MISDict* addDict(int dict_id, int type, int mode);
		void removeDict(int dict_id);
		MISDict* getDict(int dict_id);

		void addPattern(int type, string& pattern);
		void removePattern(int type, string& pattern);
		void addInterfaceFrequencyLimiter(int type, time_t timeout,int times,string & consumer_key, string & interfaceName);
		void addFrequencyLimiter(int id, int type, time_t timeout, int times);
		void clear();

	public:
		string m_name;

	protected:
		MISDictList m_white_dicts; // 白名单
		MISDictList m_black_dicts; // 强过滤词黑名单
		MISDictList m_queue_dicts; // 先发后审词
		MISFrequencyLimiter m_freq_limiter; // 频率控制器
};

#endif
