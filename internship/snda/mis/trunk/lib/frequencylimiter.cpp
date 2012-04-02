#include "memcachedst.h"
#include "frequencylimiter.h"
#include "md5.h"
#include <stdio.h>

MISFrequencyLimiter::MISFrequencyLimiter() {
	m_existed_value = "1";
}

MISFrequencyLimiter::~MISFrequencyLimiter() {
}

void MISFrequencyLimiter::addLimiter(const char* prefix, int type, time_t timeout, int times) {
	addChannelLimiter(prefix,type,timeout,times);
	addInterfaceLimiter(prefix,type,timeout,times);
}

void MISFrequencyLimiter::add(MISInput& input) {
	addInterfaceTimes(input);
	addChannelTimes(input);
}

void MISFrequencyLimiter::clear() {
	m_limiter.clear();
	interface_limiter.clear();
}

// check pass return true
// check fail return false
bool MISFrequencyLimiter::check(MISInput& input) 
{
	return (checkChannel(input) && checkInterface(input) );
}

void MISFrequencyLimiter::addChannelLimiter(const char* prefix, int type, time_t timeout, int times) 
{
	if (type != FREQTYPE_IP && type != FREQTYPE_CONTENT) return;
	limiter_item item;
	item.prefix = prefix;
	item.type = type;
	item.timeout = timeout;
	item.times = times;
	m_limiter.push_back(item);
}

void MISFrequencyLimiter::addInterfaceLimiter(const char * prefix, int  type, time_t timeout, int times)
{
	if (type != FREQTYPE_INTERFACE) return;
	limiter_item   item;
	item.prefix = prefix;
	item.type = type;
	item.timeout = timeout;
	item.times = times;
	interface_limiter.insert(make_pair(prefix,item));
}


void MISFrequencyLimiter::addInterfaceTimes(MISInput& input) {
	string key = input.content;
	Interface_Limiter::iterator iter = interface_limiter.find(key);
	if(iter == interface_limiter.end())return;
	if (iter->second.times <= 1) {
		MISMemcachedST::set(key, m_existed_value, iter->second.timeout);
	} else {
		MISMemcachedST::addTime(key, iter->second.times, iter->second.timeout);
	}
}

void MISFrequencyLimiter::addChannelTimes(MISInput& input) {
	if (m_limiter.empty()) return;
	string key;
	for (limiter::iterator iter = m_limiter.begin(); iter != m_limiter.end(); ++iter) {
		switch (iter->type) {
			case FREQTYPE_IP:
				if (!input.ip.empty()) {
					key = iter->prefix;
					key += input.user_id;
					key += ".";
					key += input.ip;
					if (iter->times <= 1) {
						MISMemcachedST::set(key, m_existed_value, iter->timeout);
					} else {
						MISMemcachedST::addTime(key, iter->times, iter->timeout);
					}
				}
				break;
			case FREQTYPE_CONTENT:
				if (!input.content.empty()) {
					key = iter->prefix;
					key += input.user_id;
					key += ".";
					key += input.content;
					MD5::md5(key, key);
					if (iter->times <= 1) {
						MISMemcachedST::set(key, m_existed_value, iter->timeout);
					} else {
						MISMemcachedST::addTime(key, iter->times, iter->timeout);
					}
				}
				break;
		}
	}
}

bool MISFrequencyLimiter::checkChannel(MISInput& input) {
	if (m_limiter.empty()) return true;
	string key;
	for (limiter::iterator iter = m_limiter.begin(); iter != m_limiter.end(); ++iter) {
		switch (iter->type) {
			case FREQTYPE_IP:
				if (!input.ip.empty()) {
					key = iter->prefix;
					key += input.user_id;
					key += ".";
					key += input.ip;
					if (iter->times <= 1) {
						if (MISMemcachedST::check(key)) {
							return false;
						}
					} else {
						if (MISMemcachedST::getTimes(key, iter->timeout) >= iter->times) {
							return false;
						}
					}
				}
				break;
			case FREQTYPE_CONTENT:
				if (!input.content.empty()) {
					key = iter->prefix;
					key += input.user_id;
					key += ".";
					key += input.content;
					MD5::md5(key, key);
					if (iter->times <= 1) {
						if (MISMemcachedST::check(key)) {
							return false;
						}
					} else {
						if (MISMemcachedST::getTimes(key, iter->timeout) >= iter->times) {
							return false;
						}
					}
				}
				break;
		}
	}
	return true;
}

bool MISFrequencyLimiter::checkInterface(MISInput& input) {
	if(interface_limiter.empty())return true;
	string key= input.content;
	Interface_Limiter::iterator iter = interface_limiter.find(key);
	if(iter == interface_limiter.end())return false;
	if (iter->second.times <= 1) {
		if (MISMemcachedST::check(key)) {
			return false;
		}
	} else {
		if (MISMemcachedST::getTimes(key, iter->second.timeout) >= iter->second.times) {
			return false;
		}
	}
	return true;
}

#ifdef _RUN_FREQUENCYLIMITER_
int main(int argc, char **argv) {
	bool ret;
	ret = MISMemcachedST::init("localhost");
	printf("%s\n", ret ? "memcached ok" : "memcached false");
	MISFrequencyLimiter limiter;
	limiter.addLimiter("twitter.1.", FREQTYPE_CONTENT, 10, 5);

	MISInput input;
	input.user_id = "100";
	input.channel = "twitter";
	input.content = "fl爱妹taobao妹g测flg6422试";
	input.record_id = "0123456789abcdef0123456789abcdef";

	ret = limiter.check(input);
	printf("%s\n", ret ? "check ok" : "check false");
	limiter.add(input);
	ret = limiter.check(input);
	printf("%s\n", ret ? "check ok" : "check false");


	return 0;
}
#endif
