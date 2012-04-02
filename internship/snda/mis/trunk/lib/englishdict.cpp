#include "englishdict.h"
#include <stdio.h>


MISEnglishDict::MISEnglishDict(int id) : MISDict(id, DICTMODE_ENGLISH) {
}

MISEnglishDict::~MISEnglishDict() {
}

void MISEnglishDict::addPattern(const string& pattern) {
	m_patterns.insert(pattern);
}

void MISEnglishDict::removePattern(const string& pattern) {
	string_hash_set::iterator iter = m_patterns.find(pattern);
	if (iter != m_patterns.end()) {
		m_patterns.erase(iter);
	}
}

int MISEnglishDict::match(const string& text, MISMatchedResult** presult) {
	if (m_patterns.empty()) return DICT_NOT_FOUND;
	int ret = DICT_NOT_FOUND;
	size_t start = 0;
	size_t size = 0;
	size_t len = text.length();
	string sub;
	string_hash_set::iterator iter;
	char ch;
	while (start < len) {
		// 找到一个词
		ch = text[start];
		size = 0;
		if (ch >= '0' && ch <= '9') { // 数字
			++size;
			while (start + size <= len) {
				ch = text[start + size];
				if (ch >= '0' && ch <= '9') ++size;
				else break;
			}
		} else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) { // 字母
			++size;
			while (start + size <= len) {
				ch = text[start + size];
				if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) ++size;
				else break;
			}
		} else {
			++start;
			continue;
		}

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
		start += size;
	}
	return ret;
}

#ifdef _RUN_ENGLISHDICT_
int main(int argc, char **argv) {
	MISEnglishDict dict;
	string s1("abc");
	dict.addPattern(s1);
	string s2("flg");
	dict.addPattern(s2);
	//dict.removePattern(s2);

	string txt("aa这是一个法轮flg功例子");
	printf("%d\n", dict.match(txt));
	return 0;
}
#endif
