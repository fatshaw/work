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
	if (type != FREQTYPE_IP && type != FREQTYPE_CONTENT) return;
	limiter_item item;
	item.prefix = prefix;
	item.type = type;
	item.timeout = timeout;
	item.times = times;
	m_limiter.push_back(item);
}

void MISFrequencyLimiter::clear() {
	m_limiter.clear();
}


void MISFrequencyLimiter::add(MISInput& input) {
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

// true 通过
// false 不通过
bool MISFrequencyLimiter::check(MISInput& input) {
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
