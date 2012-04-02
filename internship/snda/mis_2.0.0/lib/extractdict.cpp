#include "extractdict.h"
#include <stdio.h>


MISExtractDict::MISExtractDict(int id) : MISDict(id, DICTMODE_EXTRACT) {
}

MISExtractDict::~MISExtractDict() {
}

void MISExtractDict::addPattern(const string& pattern) {
	m_patterns.insert(pattern);
}

void MISExtractDict::removePattern(const string& pattern) {
	string_hash_set::iterator iter = m_patterns.find(pattern);
	if (iter != m_patterns.end()) {
		m_patterns.erase(iter);
	}
}

int MISExtractDict::match(const string& text, MISMatchedResult** presult) {
	string_hash_set::iterator iter = m_patterns.find(text);
	if (iter != m_patterns.end()) {
		if (presult) {
			if (!*presult) *presult = new MISMatchedResult();
			(*presult)->push_back(MISMatchedPattern(m_id, *iter));
		}
		return DICT_FOUND;
	} else {
		return DICT_NOT_FOUND;
	}
}

#ifdef _RUN_EXTRACTDICT_
int main(int argc, char **argv) {
	MISExtractDict dict;
	string s1("法轮功");
	dict.addPattern(s1);
	string s2("flg");
	dict.addPattern(s2);

	string txt1("aa这是一个法轮flg功例子");
	printf("%d\n", dict.match(txt1));
	string txt2("flg");
	printf("%d\n", dict.match(txt2));
	return 0;
}
#endif
