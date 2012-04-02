#include "channel.h"
#include "helper.h"
#include <stdio.h>

#define NORMALIZE(content) \
	transform(content.begin(), content.end(), content.begin(), ::tolower); \
	MISHelper::erase(content, ' '); \
	MISHelper::erase(content, '\t'); \
	MISHelper::erase(content, '\0'); \
	MISHelper::erase(content, '\r'); \
	MISHelper::erase(content, '\n'); \
	MISHelper::erase(content, "　"); \

MISChannel::MISChannel(const char* name) {
	if (name) m_name = name;
}

MISChannel::~MISChannel() {
	clear();
}

void MISChannel::clear() {
	for (MISDictList::iterator iter = m_white_dicts.begin(); iter != m_white_dicts.end(); ++iter) {
		delete *iter;
	}
	m_white_dicts.clear();
	for (MISDictList::iterator iter = m_black_dicts.begin(); iter != m_black_dicts.end(); ++iter) {
		delete *iter;
	}
	m_black_dicts.clear();
	for (MISDictList::iterator iter = m_queue_dicts.begin(); iter != m_queue_dicts.end(); ++iter) {
		delete *iter;
	}
	m_queue_dicts.clear();
	m_freq_limiter.clear();
}

void MISChannel::addFrequencyLimiter(int id, int type, time_t timeout, int times) {
	char prefix[200];
	sprintf(prefix, "%s.%d.", m_name.c_str(), id);
	m_freq_limiter.addLimiter(prefix, type, timeout, times);
}

MISDict* MISChannel::getDict(int dict_id) {
	for (MISDictList::iterator iter = m_white_dicts.begin(); iter != m_white_dicts.end(); ++iter) {
		if (dict_id == (*iter)->m_id) {
			return *iter;
		}
	}
	for (MISDictList::iterator iter = m_black_dicts.begin(); iter != m_black_dicts.end(); ++iter) {
		if (dict_id == (*iter)->m_id) {
			return *iter;
		}
	}
	for (MISDictList::iterator iter = m_queue_dicts.begin(); iter != m_queue_dicts.end(); ++iter) {
		if (dict_id == (*iter)->m_id) {
			return *iter;
		}
	}
	return NULL;
}

MISDict* MISChannel::addDict(int dict_id, int type, int mode) {
	if (type != DICTTYPE_WHITE && type != DICTTYPE_BLACK && type != DICTTYPE_QUEUE) return NULL;
	if (mode != DICTMODE_EXTRACT && mode != DICTMODE_MATCH && mode != DICTMODE_ENGLISH) return NULL;

	if (getDict(dict_id)) return NULL;

	MISDict* pdict;
	switch (mode) {
	case DICTMODE_EXTRACT:
		pdict = (MISDict*)(new MISExtractDict(dict_id));
		break;
	case DICTMODE_MATCH:
		pdict =  (MISDict*)(new MISMatchDict(dict_id));
		break;
	case DICTMODE_ENGLISH:
		pdict =  (MISDict*)(new MISEnglishDict(dict_id));
		break;
	}

	switch (type) {
	case DICTTYPE_WHITE:
		m_white_dicts.push_back(pdict);
		break;
	case DICTTYPE_BLACK:
		m_black_dicts.push_back(pdict);
		break;
	case DICTTYPE_QUEUE:
		m_queue_dicts.push_back(pdict);
		break;
	}
	return pdict;
}

void MISChannel::removeDict(int dict_id) {
	for (MISDictList::iterator iter = m_white_dicts.begin(); iter != m_white_dicts.end(); ++iter) {
		if (dict_id == (*iter)->m_id) {
			delete *iter;
			m_white_dicts.erase(iter);
			return;
		}
	}
	for (MISDictList::iterator iter = m_black_dicts.begin(); iter != m_black_dicts.end(); ++iter) {
		if (dict_id == (*iter)->m_id) {
			delete *iter;
			m_black_dicts.erase(iter);
			return;
		}
	}
	for (MISDictList::iterator iter = m_queue_dicts.begin(); iter != m_queue_dicts.end(); ++iter) {
		if (dict_id == (*iter)->m_id) {
			delete *iter;
			m_queue_dicts.erase(iter);
			return;
		}
	}
}

void MISChannel::addPattern(int dict_id, string& pattern) {	
	MISDict* pdict = getDict(dict_id);
	if (pdict) {
		NORMALIZE(pattern);
		pdict->addPattern(pattern);
	}
}


void MISChannel::removePattern(int dict_id, string& pattern) {	
	MISDict* pdict = getDict(dict_id);
	if (pdict) {
		NORMALIZE(pattern);
		pdict->removePattern(pattern);
	}
}

void MISChannel::check(MISInput& input, MISOutput& output) {
	// 白名单
	int ret;
	for (MISDictList::iterator iter = m_white_dicts.begin(); iter != m_white_dicts.end(); ++iter) {
		ret = (*iter)->match(input.user_id, &output.m_matched_result);
		if (ret == DICT_FOUND) {
			output.m_result = MISRESULT_PASS;
			return;
		}
	}
	
	string content = input.content;
	NORMALIZE(content);

	// 小写内容，英文包含需要
	string lower_content = input.content;
	transform(lower_content.begin(), lower_content.end(), lower_content.begin(), ::tolower);
	
	// 黑名单
	for (MISDictList::iterator iter = m_black_dicts.begin(); iter != m_black_dicts.end(); ++iter) {
		if (DICTMODE_ENGLISH == (*iter)->m_mode) {
			ret = (*iter)->match(lower_content, &output.m_matched_result);
		} else {	
			ret = (*iter)->match(content, &output.m_matched_result);
		}
		if (ret == DICT_FOUND) {
			output.m_result = MISRESULT_BAD;
			return;
		}
	}

	// 频率限制检查
	if (!m_freq_limiter.check(input)) {
		output.m_result = MISRESULT_FREQ_LIMIT;
		return;
	}

	// 审核队列
	output.m_result = MISRESULT_PASS;
	for (MISDictList::iterator iter = m_queue_dicts.begin(); iter != m_queue_dicts.end(); ++iter) {
		if (DICTMODE_ENGLISH == (*iter)->m_mode) {
			ret = (*iter)->match(lower_content, &output.m_matched_result);
		} else {	
			ret = (*iter)->match(content, &output.m_matched_result);
		}
		if (ret == DICT_FOUND) {
			output.m_result = MISRESULT_QUEUE;
		}
	}

	// 加入频率限制
	// 如果通过，则加入频率限制
	// 否则如果是进入队列，则在record_id不为空的时候加入
	//                                        record_id为空的时候等待下一次调用时候加入
	if ((output.m_result == MISRESULT_PASS) || !input.record_id.empty()) {
		m_freq_limiter.add(input);
	}
}
