#include "matchdict.h"
#include <stdio.h>


MISMatchDict::MISMatchDict(int id) : MISDict(id, DICTMODE_MATCH)  {
	m_min_length = 0;
	m_max_length = 0;
}

MISMatchDict::~MISMatchDict() {
}

void MISMatchDict::addPattern(const string& pattern) {
	if (m_min_length == 0) m_min_length = pattern.length();
	else m_min_length = min(m_min_length, pattern.length());
	if (m_max_length == 0) m_max_length = pattern.length();
	else m_max_length = max(m_max_length, pattern.length());
	m_patterns.insert(pattern);
}

void MISMatchDict::removePattern(const string& pattern) {
	string_hash_set::iterator iter = m_patterns.find(pattern);
	if (iter != m_patterns.end()) {
		m_patterns.erase(iter);
		if (m_patterns.empty()) {
			m_max_length = m_min_length = 0;
		} else {
			size_t len = pattern.length();
			if (len == m_min_length || len == m_max_length) {
				iter = m_patterns.begin();
				m_max_length = m_min_length = iter->length();
				for (++iter; iter != m_patterns.end(); ++iter) {
					m_min_length = min(m_min_length, iter->length());
					m_max_length = max(m_max_length, iter->length());
				}
			}
		}
	}
}

int MISMatchDict::match(const string& text, MISMatchedResult** presult) {
	if (m_min_length == 0 || m_min_length == 0) return DICT_NOT_FOUND;
	int ret = DICT_NOT_FOUND;
	size_t start = 0;
	size_t size = 0;
	size_t len = text.length();
	int xstep = 3;
	string sub;
	string_hash_set::iterator iter;
	while (start < len) {
		size = 0;
		while (size < m_min_length && start + size < len) { // 找到最小的字符串
			size += text[start + size] < 0 ? xstep : 1;
		}
		if (size == 0) break; // 找不到最小字长

		while (size <= m_max_length && start + size <= len) {
			sub = text.substr(start, size);
			iter = m_patterns.find(sub);
			if (iter != m_patterns.end()) {
				ret = DICT_FOUND;
				if (presult) {
					if (!*presult) *presult = new MISMatchedResult();
					(*presult)->push_back(MISMatchedPattern(m_id, sub));
				} else {
					return ret;
				}
			}
			if (start + size == len) break; // 结束
			size += text[start + size] < 0 ? xstep : 1;
		}
		start += text[start] < 0 ? xstep : 1;
	}
	return ret;
}

#ifdef _RUN_MATCHDICT_
int main(int argc, char **argv) {
	MISMatchDict dict;
	string s1("法轮功");
	dict.addPattern(s1);
	string s2("flg");
	dict.addPattern(s2);
	dict.removePattern(s2);

	string txt("aa这是一个法轮flg功例子");
	printf("%d\n", dict.match(txt));
	return 0;
}
#endif
