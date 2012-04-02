#ifndef _MIS_EXTRACTDICT_H_
#define _MIS_EXTRACTDICT_H_

#include "dict.h"
#include "string_hash.h"

class MISExtractDict : MISDict {
	public:
		MISExtractDict(int id);
		~MISExtractDict();

	public:
		void addPattern(const string& pattern);
		void removePattern(const string& pattern);
		int match(const string& text, MISMatchedResult** result = NULL);

	protected:
		string_hash_set m_patterns;
};

#endif
