#ifndef _MIS_FREQUENCYLIMITER_H_
#define _MIS_FREQUENCYLIMITER_H_

#include "io.h"
#include <string>
#include <vector>

using namespace std;

struct limiter_item {
	string prefix;
	int type;
	int times;
	time_t timeout;
};

typedef vector<limiter_item> limiter;

#define FREQTYPE_IP 0
#define FREQTYPE_CONTENT 1

class MISFrequencyLimiter {
	public:
		MISFrequencyLimiter();
		~MISFrequencyLimiter();

	public:
		void addLimiter(const char* channel, int type, time_t timeout, int times);
		void clear();

		bool check(MISInput& input);
		void add(MISInput& input);

	protected:
		limiter m_limiter;
		string m_existed_value;
};

#endif
