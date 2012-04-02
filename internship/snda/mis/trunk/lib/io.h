#ifndef _MIS_IO_H_
#define _MIS_IO_H_

#include <string>
#include <vector>
using namespace std;

#define MISRESULT_BAD					1
#define MISRESULT_PASS					2
#define MISRESULT_QUEUE				3
#define MISRESULT_FREQ_LIMIT		4
#define MISRESULT_BAD_REQUEST	99

#define FREQTYPE_IP 0
#define FREQTYPE_CONTENT 1
#define FREQTYPE_INTERFACE 2

typedef pair<int /* dict_id */, string /* matched */> MISMatchedPattern;
typedef vector<MISMatchedPattern> MISMatchedResult;

struct MISInput {
	string user_id;
	string channel;
	string ip;
	string content;
	string record_id;
};

class MISOutput {
public:
	MISOutput() {m_result = MISRESULT_BAD; m_matched_result = NULL;};
	~MISOutput() {if (m_matched_result) delete m_matched_result;};

public:
	int m_result;
	MISMatchedResult* m_matched_result;
};

#endif
