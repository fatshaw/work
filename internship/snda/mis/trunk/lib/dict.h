#ifndef _MIS_DICT_H_
#define _MIS_DICT_H_
#include <string>
#include "io.h"

using namespace std;

#define DICT_FOUND 1
#define DICT_NOT_FOUND 0

#define DICTMODE_EXTRACT 0
#define DICTMODE_MATCH 1
#define DICTMODE_ENGLISH 2

class MISDict {
	public:
		MISDict(int id, int mode) {m_id = id; m_mode = mode;};
		virtual ~MISDict() {};
	
	public:
		virtual void addPattern(const string& pattern) = 0;
		virtual void removePattern(const string& pattern) = 0;
		virtual int match(const string& text, MISMatchedResult** result = NULL) = 0;

	public:
		int m_id;
		int m_mode;
};

#endif
