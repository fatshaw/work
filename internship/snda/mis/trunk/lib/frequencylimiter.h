#ifndef _MIS_FREQUENCYLIMITER_H_
#define _MIS_FREQUENCYLIMITER_H_

#include "io.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

struct limiter_item {
	string prefix;
	int type;
	int times;
	time_t timeout;
};

typedef vector<limiter_item> limiter;
typedef map<string , limiter_item> Interface_Limiter;

class MISFrequencyLimiter {
	public:
		MISFrequencyLimiter();
		~MISFrequencyLimiter();

	public:
		void addLimiter(const char* channel, int type, time_t timeout, int times);
		void clear();
		bool check(MISInput& input);
		void add(MISInput& input);
	private:	
		void	addInterfaceLimiter(const char* channel, int type, time_t timeout, int times);
		void	addChannelLimiter(const char* channel, int type, time_t timeout, int times);
		bool checkChannel(MISInput& input);
		bool checkInterface(MISInput& input);
		void addChannelTimes(MISInput& input);
		void addInterfaceTimes(MISInput& input) ;
	protected:
		limiter m_limiter;
		Interface_Limiter interface_limiter;
		string m_existed_value;
};

#endif
