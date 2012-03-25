#include "StreamOperation.h"

int StreamOperation::readStream(ifstream& in, const string & fileName, vector<
		string> & lines, pos lineCnt) {
 //in.close();
  in.clear();
 //in.open(fileName.c_str());
	string line;
	int cnt = 0;
	if(!in)return 0;
	if(in.eof() == true)return 0;
	for(pos i = 0; i != lineCnt; ++i) {
		if(in.eof() == false) {
			getline(in,line);
			lines.push_back(line);
			++cnt;
		}
		else{
			break;
		}
	}
	return cnt;
}

void StreamOperation::writeStream(ofstream & out, const string & fileName,
																				const vector<string>&lines,	pos lineCnt = 0) {
	if (lineCnt > lines.size())
		return;
	//out.clear();
	//out.close();
	//out.open(fileName.c_str());
	if(!out)return;

	if (lineCnt == 0) {
		lineCnt = lines.size();
	}
	for (vector<string>::const_iterator it = lines.begin(); it != lines.end(); ++it) {
		if (lineCnt-- == 0)break;
		out << *it;
	}
}



